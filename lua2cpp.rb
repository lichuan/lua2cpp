#!/usr/bin/env ruby

$decl = <<-INTRO
@author: lichuan
@qq: 308831759
@email: 308831759@qq.com
@homepage: www.lichuan.me
@date: 2013-05-11
@desc: this is the generator of binding code between lua and c++.
INTRO

def parse_namespace_class(tbl_namespace, tbl_class, full_name)
  full_name.prepend(".")
  full_name.scan(/\._([^\.]+)/) do |arr|
    namespace = arr[0]
    tbl_namespace[tbl_namespace.size] = namespace
  end
  full_name.scan(/\.([^_][^\.]*)/) do |arr|
    class_name = arr[0]
    tbl_class[tbl_class.size] = class_name
  end
end

def is_basic_type(type_str)
  ["string", "int32", "number", "uint32"].each { |basic_type| return true if type_str == basic_type }
  false
end

def parse_argument(tbl_arg, args)
  args.scan(/([^\s,]+)/) do |arr|
    arg = arr[0]
    idx = tbl_arg.size
    tbl_arg[idx] = {}
    parse_type(tbl_arg[idx], arg, false)
  end
end

def error_msg(str)
  puts "Error: #{str}"
  exit(1)
end

def parse_function_1(tbl_func, func_str)
  match_list = /static (.*)/.match(func_str)
  if match_list.nil?
    parse_function_2(tbl_func, func_str)
  else
    tbl_func["is_static"] = true
    parse_function_2(tbl_func, match_list[1])
  end
end

def parse_type(tbl_type, type_str, is_ret)
  return if type_str.empty?
  match_list = /([^\s\*\|\&]+)(\*|\&)?(\|gc\|)?/.match(type_str)
  if match_list.nil?
    error_msg("type invalid in #{type_str}")
  else
    if match_list[3] == "|gc|"
      if not is_ret
        error_msg("function parameter should not be |gc| in #{type_str}")
      end
    end
    type = match_list[1]
    tbl_type["name"] = type
    if is_basic_type type
      tbl_type["is_basic"] = true
      ["*", "|gc|", "&"].each do |v|
        error_msg("basic type does not support #{v} in #{type_str}") if match_list[2] == v or match_list[3] == v
      end
    elsif not $reg_info.has_key?(match_list[1])
      error_msg("type #{match_list[1]} is not exist in #{type_str}")
    else
      if match_list[2] == "*"
        tbl_type["is_ptr"] = true
      elsif match_list[2] == "&"
        tbl_type["is_ref"] = true
      end
      if match_list[3] == "|gc|"
        tbl_type["gc"] = true
      end
    end
  end
end

def parse_function_2(tbl_func, func_str)
  match_list = /([^\s]*?) *([^\s]+)\((.*)\)/.match(func_str)
  tbl_func["ret_type"] = {}
  if match_list.nil?
    parse_function_3(tbl_func, func_str)
  else
    func_name = match_list[2]
    if not func_name.empty?
      tbl_func["name"] = func_name
      if not tbl_func.has_key?("export_name")
        tbl_func["export_name"] = func_name
      end
      if not match_list[1].empty?
        parse_type(tbl_func["ret_type"], match_list[1], true)
      end
      parse_argument(tbl_func["arg"], match_list[3])
    end
  end
end

def parse_function_3(tbl_func, func_str)
  match_list = /(^[^\s=]*?) *([^\s=]+)=([^\s]+)/.match(func_str)
  if match_list.nil?
    match_list = /(^[^\s=]+) +([^\s]+)/.match(func_str)
    if match_list.nil?
      error_msg("function is invalid in #{func_str}")
    else
      parse_type(tbl_func["ret_type"], match_list[1], true)
      tbl_func["is_get"] = true
      tbl_func["name"] = match_list[2]
      if not tbl_func.has_key?("export_name")
        tbl_func["export_name"] = match_list[2]
      end
    end
  elsif not tbl_func.has_key?("export_name")
    error_msg("no export name assign in #{func_str}")
  else
    tbl_func["is_set"] = true
    tbl_func["name"] = match_list[2]
    parse_argument(tbl_func["arg"], match_list[3])
    parse_type(tbl_func["ret_type"], match_list[1], true)
  end
end

def parse_function(tbl_func, func_str)
  match_list = /([^\s]+) : (.+)/.match(func_str)
  if match_list.nil?
    parse_function_1(tbl_func, func_str)
  else
    tbl_func["export_name"] = match_list[1]
    parse_function_1(tbl_func, match_list[2])
  end
end

def parse_file
  $reg_info = {}
  lua_reg_content = File.open("./lua2cpp.txt").readlines().join
  match_list = /([^\{]*?)[^\n]*\n\{/.match(lua_reg_content)
  $head = match_list[1]
  body = lua_reg_content[$head.size..-1]
  body.scan(/([^\n]+)\n\{\n(.*?)\n\}/m) do |part1, part2|
    full_name = part1.match(/[^\s]*/)[0]
    tbl = {}
    tbl["namespace"] = {}
    tbl["class"] = {}
    tbl["name"] = full_name
    tbl["super"] = {}
    tbl["function"] = {}
    error_msg("#{part1} have already registered") if $reg_info.has_key?(full_name)
    $reg_info[full_name] = tbl
  end
  body.scan(/(^[^\s][^\n]*)\n\{\n(.*?)\n\}/m) do |part1, part2|
    full_name = part1.match(/[^\s]*/)[0]
    tbl = $reg_info[full_name]
    parse_namespace_class(tbl["namespace"], tbl["class"], full_name)
    if not tbl["class"].empty?
      match_list = / extends (.*)/.match(part1)
      if not match_list.nil?
        super_class_list = match_list[1]
        super_class_list.scan(/([^\s,]+)/) do |arr|
          super_class = arr[0]
          tbl["super"][tbl["super"].size] = super_class
        end
      end
    end
    func_idx = 0
    new_idx = 0
    part2.scan(/([^\s].+)/) do |arr|
      function_line = arr[0]
      tbl["function"][func_idx] = {}
      tbl["function"][func_idx]["arg"] = {}
      match_list = /^\((.*)\)/.match(function_line)
      if not match_list.nil?
        tbl["function"][func_idx]["is_new"] = true
        tbl["function"][func_idx]["export_name"] = "new" if new_idx == 0
        tbl["function"][func_idx]["export_name"] = "new#{new_idx}" if new_idx > 0
        args = match_list[1]
        parse_argument(tbl["function"][func_idx]["arg"], args)
        new_idx += 1
      else
        parse_function(tbl["function"][func_idx], function_line)
      end
      func_idx += 1
    end
  end
end

def generate_header()
  header = "/*\n"
  $decl.each_line do |line|
    header += " #{line}"
  end
  header = header.sub(/this is the generator of binding code between lua and c\+\+/, 'this is the binding code between lua and c++ generated by lua2cpp')
  header += "*/

"
  header += $head
  header += <<-HEADER
void get_global_table(lua_State *lua_state, const char *nodes_name)
{
    char buf[1024];
    memcpy(buf, nodes_name, strlen(nodes_name) + 1);
    char *p = buf;
    const char *q = p;
    int count = 0;

    while(*p != 0)
    {
        if(*p == '.')
        {
            *p = 0;

            if(count == 0)
            {
                lua_getglobal(lua_state, q);

                if(lua_isnil(lua_state, -1))
                {
                    return;
                }
            }
            else
            {
                lua_pushstring(lua_state, q);
                lua_rawget(lua_state, -2);

                if(lua_isnil(lua_state, -1))
                {
                    return;
                }
            }

            q = p + 1;
            ++count;
        }

        ++p;
    }
    
    if(count == 0)
    {
        lua_getglobal(lua_state, q);

        if(lua_isnil(lua_state, -1))
        {
            return;
        }
    }
    else
    {
        lua_pushstring(lua_state, q);
        lua_rawget(lua_state, -2);

        if(lua_isnil(lua_state, -1))
        {
            return;
        }
    }
}

void build_global_table(lua_State *lua_state, const char *nodes_name)
{
    char buf[1024];
    memcpy(buf, nodes_name, strlen(nodes_name) + 1);
    char *p = buf;
    const char *q = p;
    int count = 0;

    while(*p != 0)
    {
        if(*p == '.')
        {
            *p = 0;

            if(count == 0)
            {
                lua_getglobal(lua_state, q);

                if(lua_isnil(lua_state, -1))
                {
                    lua_newtable(lua_state);
                    lua_pushvalue(lua_state, -1);
                    lua_setglobal(lua_state, q);
                }
            }
            else
            {
                lua_pushstring(lua_state, q);
                lua_rawget(lua_state, -2);

                if(lua_isnil(lua_state, -1))
                {
                    lua_pop(lua_state,  1);
                    lua_pushstring(lua_state, q);
                    lua_newtable(lua_state);
                    lua_pushvalue(lua_state, -1);
                    lua_insert(lua_state, -4);
                    lua_rawset(lua_state, -3);
                    lua_pop(lua_state, 1);
                }
            }

            q = p + 1;
            ++count;
        }

        ++p;
    }
    
    if(count == 0)
    {
        lua_getglobal(lua_state, q);

        if(lua_isnil(lua_state, -1))
        {
            lua_newtable(lua_state);
            lua_pushvalue(lua_state, -1);
            lua_setglobal(lua_state, q);
            lua_pushstring(lua_state, "__REG_NAME__");
            lua_pushstring(lua_state, nodes_name);
            lua_rawset(lua_state, -3);
        }
    }
    else
    {
        lua_pushstring(lua_state, q);
        lua_rawget(lua_state, -2);

        if(lua_isnil(lua_state, -1))
        {
            lua_pop(lua_state,  1);
            lua_pushstring(lua_state, q);
            lua_newtable(lua_state);
            lua_pushvalue(lua_state, -1);
            lua_insert(lua_state, -4);
            lua_rawset(lua_state, -3);
            lua_pop(lua_state, 1);
            lua_pushstring(lua_state, "__REG_NAME__");
            lua_pushstring(lua_state, nodes_name);
            lua_rawset(lua_state, -3);
        }
    }

    lua_settop(lua_state, 0);
}

static std::map<std::string, std::list<std::string> > g_super_map;

static int get_super_member(lua_State *lua_state)
{
    const char *key = lua_tostring(lua_state, -1);
    lua_pop(lua_state, 1);
    lua_pushstring(lua_state, "__REG_NAME__");
    lua_rawget(lua_state, -2);

    if(lua_isnil(lua_state, -1))
    {
        return 0;
    }
    
    const char *reg_name = lua_tostring(lua_state, -1);
    std::map<std::string, std::list<std::string> >::const_iterator iter = g_super_map.find(reg_name);
    
    if(iter == g_super_map.end())
    {
        return  0;
    }
    
    const std::list<std::string> &super_list = iter->second;
    
    for(std::list<std::string>::const_iterator iter = super_list.begin(); iter != super_list.end(); ++iter)
    {
        std::string super_name = *iter;
        get_global_table(lua_state, super_name.c_str());
        lua_getfield(lua_state, -1, key);

        if(!lua_isnil(lua_state, -1))
        {
            return 1;
        }
    }

    return 0;
}

HEADER
end

def generate_ns_class_name(ns_tbl, cls_tbl)
  gen_str = ""
  ns_tbl.each_value do |ns|
    gen_str << ns << "::"
  end
  have_cls = false
  cls_tbl.each_value do |cls|
    have_cls = true
    gen_str << cls << "::"
  end
  gen_str = gen_str[0..-3] if have_cls
  return gen_str
end

def generate_ns_class_prefix(ns_tbl, cls_tbl)
  gen_str = ""
  ns_tbl.each_value do |ns|
    gen_str << ns << "::"
  end
  cls_tbl.each_value do |cls|
    gen_str << cls << "::"
  end
  return gen_str
end

def generate_new_function(tbl, func)
  gen_str = ""
  cls_name = generate_ns_class_name(tbl["namespace"], tbl["class"])
  idx = 1
  func_str = []
  func["arg"].each_value do |arg|
    if arg.has_key? "is_basic"
      case arg["name"]
      when "int32"
        gen_str += "
    int32 arg_#{idx} = lua_tointeger(lua_state, #{idx});"
        func_str << "arg_#{idx}"
      when "number"
        gen_str += "
    double arg_#{idx} = lua_tonumber(lua_state, #{idx});"
        func_str << "arg_#{idx}"
      when "uint32"
        gen_str += "
    uint32 arg_#{idx} = lua_tounsigned(lua_state, #{idx});"
        func_str << "arg_#{idx}"
      when "string"
        gen_str += "
    const char *arg_#{idx} = lua_tostring(lua_state, #{idx});"
        func_str << "arg_#{idx}"
      else        
      end
    elsif arg.has_key? "is_ptr"
      arg_cls_info = $reg_info[arg["name"]]
      arg_cls = generate_ns_class_name(arg_cls_info["namespace"], arg_cls_info["class"])
      gen_str += "
    uint32 *ud_#{idx} = (uint32*)lua_touserdata(lua_state, #{idx});
    uint32 gc_flag_#{idx} = *ud_#{idx};
    gc_flag_#{idx} = 0; /* not used in argument, only used in __gc function */
    ud_#{idx} += 1;
    #{arg_cls} *arg_#{idx} = *(#{arg_cls}**)ud_#{idx};"
      func_str << "arg_#{idx}"
    else
      arg_cls_info = $reg_info[arg["name"]]
      arg_cls = generate_ns_class_name(arg_cls_info["namespace"], arg_cls_info["class"])
      gen_str += "
    uint32 *ud_#{idx} = (uint32*)lua_touserdata(lua_state, #{idx});
    uint32 gc_flag_#{idx} = *ud_#{idx};
    gc_flag_#{idx} = 0; /* not used in argument, only used in __gc function */
    ud_#{idx} += 1;
    #{arg_cls} *arg_#{idx} = *(#{arg_cls}**)ud_#{idx};"
      func_str << "*arg_#{idx}"
    end
    idx += 1
  end
  gen_str += %Q{
    lua_settop(lua_state, 0);
    uint32 *new_udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(#{cls_name}*));
    uint32 &new_gc_flag = *new_udata;
    new_gc_flag = 1; /* need gc default */
    new_udata += 1;
    (#{cls_name}**)new_udata = new #{cls_name}(#{func_str.join(', ')});
    get_global_table(lua_state, "#{tbl["name"]}");
    lua_setmetatable(lua_state, -2);

    return 1;
}
  return gen_str
end

def generate_function(tbl, func)
  gen_str = ""
  return gen_str
end

def generate_get_function(tbl, func)
  puts func
  ns_cls_prefix = generate_ns_class_prefix(tbl["namespace"], tbl["class"])
  puts ns_cls_prefix
  gen_str = ""  
  if func.has_key? "is_static"
    if func["ret_type"].has_key? "is_basic"
      case func["ret_type"]["name"]
      when "int32"
        gen_str += "
    int32 v = #{ns_cls_prefix}#{func["name"]};
    lua_pushinteger(lua_state, v);

    return 1;
"
      when "uint32"
        gen_str += "
    uint32 v = #{ns_cls_prefix}#{func["name"]};
    lua_pushunsigned(lua_state, v);

    return 1;
"
      when "number"
        gen_str += "
    double v = #{ns_cls_prefix}#{func["name"]};
    lua_pushnumber(lua_state, v);

    return 1;
"
      when "string"
        gen_str += "
    std::string v = #{ns_cls_prefix}#{func["name"]};
    lua_pushstring(lua_state, v.c_str());

    return 1;
"
      else
      end
    elsif func["ret_type"].has_key? "is_ptr"
      if func["ret_type"].has_key? "gc"
      end
    end
  end
  return gen_str
end

def generate_set_function(tbl, func)
  gen_str = ""
  return gen_str
end

def generate_file()
  $reg_func_tbl = {}
  gen_str = generate_header
  $reg_info.each do |full_name, v|
    $reg_func_tbl[full_name] = []
    #puts v
    v["function"].each_value do|func|
      gen_str += "static int "
      gen_func_name = "lua"
      v["namespace"].each_value do |ns|
        gen_func_name += "____" + ns
      end
      v["class"].each_value do |cls|
        gen_func_name += "___" + cls
      end
      gen_func_name += "__" + func["export_name"]
      $reg_func_tbl[full_name].push([func["export_name"], gen_func_name])
      gen_str += gen_func_name + "(lua_State *lua_state)"
      gen_str += "
{"
      if func.has_key? "is_new"
        gen_str += generate_new_function(v, func)
      elsif func.has_key? "is_get"
        gen_str += generate_get_function(v, func)
      elsif func.has_key? "is_set"
        gen_str += generate_set_function(v, func)
      else
        gen_str += generate_function(v, func)
      end
      gen_str += "}

"
    end
  end
  gen_str += "
int main(){}
"
  File.open("./lua2cpp.cpp", "w").write(gen_str)
end

#parse and generate
parse_file
generate_file

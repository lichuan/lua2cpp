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

def parse_lua_reg_file
  $reg_info = {}
  lua_reg_content = File.open("./lua_register.txt").readlines().join
  match_list = /([^\{]*?)[^\n]*\n\{/.match(lua_reg_content)
  $head = match_list[1]
  body = lua_reg_content[$head.size..-1]
  body.scan(/([^\n]+)\n\{\n(.*?)\n\}/m) do |part1, part2|
    full_name = part1.match(/[^\s]*/)[0]
    tbl = {}
    tbl["namespace"] = {}
    tbl["class"] = {}
    tbl["name"] = part1
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
      puts function_line
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
  header += "*/\n\n"
end

def generate_register_table(full_name)
  reg_dict = $reg_info[full_name]
  reg_str = ""
  name_list = full_name.split(".")
  cur_full_name = ""
  #puts full_name
  name_list.each_with_index do |elem, idx|
    cur_full_name << "."  if idx > 0
    cur_full_name << elem
    #puts cur_full_name
    reg_str += %Q!lua_getglobal(L, "#{elem}");\n! if idx == 0
    reg_str += %Q!
if(lua_istable(L, -1) == 0)
{
    luaL_newmetatable(L, "#{cur_full_name}");
    lua_setglobal(L,
}
!
  end
  format_str = ""
  reg_str.each_line {|line| format_str << "    #{line}"}
  format_str
end

def generate_cpp_file()
  gen_str = generate_header
  reg_table_str = 'static int find_member_self_and_super(lua_State *L)
{
    assert(lua_istable(L, -1));
    lua_getfield(L, -1, "full_name");
}

static void register_table(lua_State *L)
{
'
  $reg_info.each do |full_name, v|
    reg_table_str += generate_register_table(full_name)
    v["function"].each_value do |func_v|
      gen_str += "static int lua_function"
      v["namespace"].each_value do |ns_v|
        gen_str += "____" + ns_v
      end
      v["class"].each_value do |cls_v|
        gen_str += "___" + cls_v
      end
      gen_str += "__" + func_v["export_name"] + "(lua_State *L)"
      gen_str += "\n{"
      if func_v["is_new_function"]
      end
      gen_str += "\n    test;"
      gen_str += "\n}\n\n"
    end
  end
  reg_table_str += "}"
  File.open("./lua_cpp.cpp", "w").write(gen_str + reg_table_str)
end

parse_lua_reg_file
generate_cpp_file


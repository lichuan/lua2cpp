# @author: lichuan
# @qq: 308831759
# @email: 308831759@qq.com
# @homepage: www.lichuan.me
# @date: 2013-05-11
# @desc: this is the generator of lua interface in c++ application.

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
  ["string", "int", "number"].each { |basic_type| return true if type_str == basic_type }
  false
end

def parse_argument(tbl_arg, args)
  args.scan(/([^\s,]+)/) do |arr|
    arg = arr[0]
    idx = tbl_arg.size
    tbl_arg[idx] = {}
    parse_type(tbl_arg[idx], arg)
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

def parse_type(tbl_type, type_str)
  match_list = /([^\s\*]+)(\*)?(\|gc\|)?/.match(type_str)
  if match_list.nil?
    error_msg("type invalid in #{type_str}")
  else
    type = match_list[1]
    tbl_type["name"] = type
    if is_basic_type type
      tbl_type["is_basic"] = true
      if match_list[2] == "*"
        error_msg("basic type does not support pointer in #{type_str}")
      end
      if match_list[3] == "|gc|"
        error_msg("basic type does not support |gc| in #{type_str}")
      end
    elsif not $reg_info.has_key?(match_list[1])
      error_msg("type #{match_list[1]} is not exist")
    else
      if match_list[2] == "*"
        tbl_type["is_pointer"] = true
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
        parse_type(tbl_func["ret_type"], match_list[1])
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
      parse_type(tbl_func["ret_type"], match_list[1])
      tbl_func["direct_get"] = true
      tbl_func["name"] = match_list[2]
      if not tbl_func.has_key?("export_name")
        tbl_func["export_name"] = match_list[2]
      end
    end
  elsif not tbl_func.has_key?("export_name")
    error_msg("no export name assign in #{func_str}")
  else
    tbl_func["direct_set"] = true
    tbl_func["name"] = match_list[2]
    parse_argument(tbl_func["arg"], match_list[3])
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
  body.scan(/([^\n]*)\n\{\n(.*?)\n\}/m) do |part1, part2|
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
  body.scan(/([^\n]*)\n\{\n(.*?)\n\}/m) do |part1, part2|
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
        tbl["function"][func_idx]["is_new_function"] = true
        new_idx += 1
        tbl["function"][func_idx]["export_name"] = "new#{new_idx}"
        args = match_list[1]
        parse_argument(tbl["function"][func_idx]["arg"], args)
      else
        parse_function(tbl["function"][func_idx], function_line)
      end
    end
  end
end

parse_lua_reg_file




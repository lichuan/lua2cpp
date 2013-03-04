local decl = 
[[ 
-- @author: lichuan 
-- @qq: 308831759 
-- @email: 308831759@qq.com
-- @homepage: www.lichuan.me
-- @date: 2012-11-29 
-- @desc: this is the generator of lua interface in c++ application.
]] 

-- local reg_path = "/sdcard/sigmascript/lua_register.txt" 

local reg_path = "lua_register.txt" 
local reg_info = {}

local function parse_namespace_class(full_name, tbl) 
    full_name = "."..full_name

    for namespace in string.gmatch(full_name, "%._([^%.]+)") do
        tbl["namespace"][#tbl["namespace"] + 1] = namespace 
    end 

    for class in string.gmatch(full_name, "%.([^%._][^%.]+)") do
        tbl["class"][#tbl["class"] + 1] = class 
    end 
end

local function is_buildin_type(obj_type)
    if obj_type == "int" or obj_type == "number" or obj_type == "string" then
        return true
    end

    return false
end

local function print_error(msg)
    print(msg)
    os.exit()
end

local function get_pointer_type(arg_str)
    local ptr_type = string.match(arg_str, "([^%s]+)%*$")

    return ptr_type
end

local function parse_arg(arg_str, tbl)
    arg_str = arg_str..","
    
    for arg in string.gmatch(arg_str, "([^%s,]+)%,.-") do
        local ptr_type = get_pointer_type(arg)
        local one_arg = {}
        
        if ptr_type then
            if is_buildin_type(ptr_type) then
                print_error("the buildin type does not support pointer argments".." arg_str: "..arg_str)
            end

            one_arg["type"] = ptr_type
            one_arg["is_ptr"] = true
        elseif not is_buildin_type(arg) then
            print_error("the non-buildin type only support pointer argments".." arg_str: "..arg_str)
        else
            one_arg["type"] = arg
        end
        
        tbl[#tbl + 1] = one_arg
    end
end

local function parse_func(func_str, tbl)
    local static_stmt, func_stmt = string.match(func_str, "%s*(static)%s*(.*)$")
    
    if static_stmt then
        tbl["is_static"] = true
    else
        func_stmt = string.match(func_str, "%s*(.*)$")
    end
    
    local direct_member = string.match(func_stmt, "%..*")

    if direct_member then
        direct_member = string.match(func_stmt, "^[^()]*$")
    end
    
    local func_ret, func_name, func_arg
    
    if direct_member then
        local set_member = string.match(func_stmt, "=")
        
        if set_member then
            func_ret, func_name, func_arg = string.match(func_stmt, "([^%s]*%**)%s*%.([^%s]+)=([^%s]+)%s*$")
            tbl["direct_set_member"] = true
            parse_arg(func_arg, tbl["arg"])
        else
            func_ret, func_name = string.match(func_stmt, "([^%s]+)%s+%.([^%s]+)%s*$")
            tbl["direct_get_member"] = true
        end
    else
        func_ret, func_name, func_arg = string.match(func_stmt, "([^%s]-)%s*([^%s]+)%((.*)%)%s*$")
        parse_arg(func_arg, tbl["arg"])
    end

    tbl["ret_type"] = func_ret
    tbl["member_name"] = func_name

    if not tbl["export_name"] then
        tbl["export_name"] = func_name
    end
end

local function generate(tbl) 
    local func_prefix = "static int lua_register" 

    -- place in 4 _ after namespace name
    for i, v in ipairs(tbl["namespace"]) do 
        func_prefix = func_prefix.."____"..v 
    end 

    func_prefix = func_prefix.."____"

    -- place in 2 _ after class name
    for i, v in ipairs(tbl["class"]) do 
        func_prefix = func_prefix..v.."__" 
    end

    for i, v in ipairs(tbl["function"]) do
        print("")
        local func_body = func_prefix..v["export_name"].."(luaState *L)\n{}"
        print(func_body)
    end
end 

local function main() 
    local reg_file = assert(io.open(reg_path, "rb")) 
    local reg_str = reg_file:read("*all") 
    local header, pos = string.match(reg_str, "^([^{]-)()[ ,%[%]%w_%.]-%s-{") 
    print(header) 
    local content = string.sub(reg_str, pos) 

    for one_reg in string.gmatch(content, ".-{.-}") do 
        local part1, part2 = string.match(one_reg, "([^{]*){([^}]*)}")
        local full_name = string.match(part1, "[^%s]+") 
        local tbl = {} 
        tbl["namespace"] = {} 
        tbl["class"] = {} 
        tbl["inherit"] = {} 
        tbl["function"] = {} 
        tbl["name"] = full_name
        
        if reg_info[full_name] then 
            print_error("register"..full_name.." failed: the name have already been registered") 
        end 
        
        reg_info[full_name] = tbl 
        parse_namespace_class(full_name, tbl) 

        if #tbl["class"] ~= 0 then 
            local inherit_part = string.match(part1, "[^%s]+.-inherit from[^\n]*%[(.*)%]")
            
            if inherit_part then
                for base_cls in string.gmatch(inherit_part, "[^%s,]+") do
                    tbl["inherit"][#tbl["inherit"] + 1] = base_cls 
                end 
            end 
        end 

        local func_idx = 1 
        local new_idx = 0 

        for line in string.gmatch(part2, ".-[^%s].-\n") do
            tbl["function"][func_idx] = {} 
            tbl["function"][func_idx]["arg"] = {}
            local new_arg = string.match(line, "^%s*%((.*)%)%s*$") 
            local export_name, func_info = string.match(line, "([%w_]+)%s*:(.*)")

            if new_arg then 
                local suffix = "" 

                if new_idx > 0 then 
                    suffix = new_idx 
                end
                
                new_idx = new_idx + 1 
                tbl["function"][func_idx]["export_name"] = "new"..suffix 
                tbl["function"][func_idx]["is_new"] = true
                parse_arg(new_arg, tbl["function"][func_idx]["arg"])
            elseif export_name and func_info then
                tbl["function"][func_idx]["export_name"] = export_name
                parse_func(func_info, tbl["function"][func_idx])
            else
                parse_func(line, tbl["function"][func_idx])
            end
            
            func_idx = func_idx + 1
        end 
    end

    for k, v in pairs(reg_info) do
        generate(v)
    end
end 

main() 

print "------start lua script--------"

dat_obj = _dat_ns.Data.new()
print(dat_obj:id()) ---------------------------------------------------------> output: 0
print(dat_obj:name()) -------------------------------------------------------> output: no name
dat_obj:set_id(92)
dat_obj:set_name("I'm old object")
print(dat_obj:id()) ---------------------------------------------------------> output: 92
print(dat_obj:name()) -------------------------------------------------------> output: I'm old object

print ""

test_lua_obj = Test_Lua.new()
print(test_lua_obj:get_data_id()) -------------------------------------------> output: 0
print(test_lua_obj:get_data_name()) -----------------------------------------> output: no name

test_lua_obj:replace_data(dat_obj)
print(test_lua_obj:get_data_id()) -------------------------------------------> output: 92
print(test_lua_obj:get_data_name()) -----------------------------------------> output: I'm old object

print ""

tmp_obj = test_lua_obj:get_data()
tmp_obj:set_name("I'm tmp object")
print(tmp_obj:name()) -------------------------------------------------------> output: I'm tmp object
print(test_lua_obj:get_data_name()) -----------------------------------------> output: I'm old object

print ""

ref_obj = test_lua_obj:get_ref_data()
ref_obj:set_name("I'm ref object")
print(ref_obj:name()) -------------------------------------------------------> output I'm ref object
print(test_lua_obj:get_data_name()) -----------------------------------------> output I'm ref object

print "------end lua script-------"


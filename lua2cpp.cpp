/*
 @author: lichuan
 @qq: 308831759
 @email: 308831759@qq.com
 @homepage: www.lichuan.me
 @date: 2013-05-11
 @desc: this is the binding code between lua and c++ generated by lua2cpp.
*/

#include <string>
#include <list>
#include <cstring>
#include <map>
#include <iostream>
#include "lua.hpp"

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

static int lua__exe_script(lua_State *lua_state)
{}

static int lua__ooopp(lua_State *lua_state)
{}

static int lua____arg___Cpp_Arg__new(lua_State *lua_state)
{
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(arg::Cpp_Arg*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default */
    udata += 1;
    *(arg::Cpp_Arg**)udata = new arg::Cpp_Arg();
    get_global_table(lua_state, "_arg.Cpp_Arg");
    lua_setmetatable(lua_state, -2);

    return 1;
}

static int lua____arg___Cpp_Arg__new1(lua_State *lua_state)
{
    int32 arg_1 = lua_tointeger(lua_state, 1);
    uint32 *ud_2 = (uint32*)lua_touserdata(lua_state, 2);
    uint32 gc_flag_2 = *ud_2;
    gc_flag_2 = 0; /* not used in argument, only used in __gc function */
    ud_2 += 1;
    engine::memory::Base2 *arg_2 = *(engine::memory::Base2**)ud_2;
    uint32 *ud_3 = (uint32*)lua_touserdata(lua_state, 3);
    uint32 gc_flag_3 = *ud_3;
    gc_flag_3 = 0; /* not used in argument, only used in __gc function */
    ud_3 += 1;
    arg::Cpp_Arg *arg_3 = *(arg::Cpp_Arg**)ud_3;
    double arg_4 = lua_tonumber(lua_state, 4);
    const char *arg_5 = lua_tostring(lua_state, 5);
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(arg::Cpp_Arg*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default */
    udata += 1;
    *(arg::Cpp_Arg**)udata = new arg::Cpp_Arg(arg_1, *arg_2, arg_3, arg_4, arg_5);
    get_global_table(lua_state, "_arg.Cpp_Arg");
    lua_setmetatable(lua_state, -2);

    return 1;
}

static int lua____arg___Cpp_Arg__hp(lua_State *lua_state)
{
    int32 v = arg::Cpp_Arg::hp;
    lua_pushinteger(lua_state, v);

    return 1;
}

static int lua____arg___Cpp_Arg__id(lua_State *lua_state)
{}

static int lua____arg___Cpp_Arg__getd(lua_State *lua_state)
{}

static int lua____arg___Cpp_Arg__set_hp(lua_State *lua_state)
{}

static int lua____arg___Cpp_Arg__get_v(lua_State *lua_state)
{}

static int lua____arg___Cpp_Arg__set_v(lua_State *lua_state)
{}

static int lua____base___Base1__new(lua_State *lua_state)
{
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(base::Base1*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default */
    udata += 1;
    *(base::Base1**)udata = new base::Base1();
    get_global_table(lua_state, "_base.Base1");
    lua_setmetatable(lua_state, -2);

    return 1;
}

static int lua____base___Base1__owerid(lua_State *lua_state)
{}

static int lua____base___Base1__r3r3r(lua_State *lua_state)
{}

static int lua____base___Base1__IJ3RJ2(lua_State *lua_state)
{}

static int lua____base___Base1__get_info(lua_State *lua_state)
{}

static int lua____engine____memory___Base2__new(lua_State *lua_state)
{
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(engine::memory::Base2*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default */
    udata += 1;
    *(engine::memory::Base2**)udata = new engine::memory::Base2();
    get_global_table(lua_state, "_engine._memory.Base2");
    lua_setmetatable(lua_state, -2);

    return 1;
}

static int lua____engine____memory___Base2__pt_world_v(lua_State *lua_state)
{}

static int lua____engine____memory___Base2__pt_world(lua_State *lua_state)
{}

static int lua____engine____memory___Base2__pt_arg_2(lua_State *lua_state)
{}

static int lua____engine____memory___Base3___ICls__get_name(lua_State *lua_state)
{}

static int lua____engine____memory____lll__name_str(lua_State *lua_state)
{}

static int lua____engine____memory____lll__get_name(lua_State *lua_state)
{}

static int lua____engine____memory___Base3__get_name(lua_State *lua_state)
{}

static int lua____engine____memory___Base3__bb1(lua_State *lua_state)
{
    gabriel::b1 *v = engine::memory::Base3::bb1;
    uint32 *udata = lua_newuserdata(lua_state, sizeof(uint32) + sizeof(gabriel::b1*));
    uint32 &gc_flag = *udata;
    gc_flag = 0;
    udata += 1;
    *(gabriel::b1**)udata = v;

    return 1;
}

static int lua____gabriel___b1__b1_id(lua_State *lua_state)
{}

static int lua____gabriel___b1__set_b1_name(lua_State *lua_state)
{}

static int lua____gabriel___b1__print_b1(lua_State *lua_state)
{}

static int lua____gabriel___b2__b2_name(lua_State *lua_state)
{}

static int lua____gabriel___b2__last_name(lua_State *lua_state)
{}

static int lua____gabriel___b2__uniqued_id(lua_State *lua_state)
{}

static int lua___Derived__new(lua_State *lua_state)
{
    int32 arg_1 = lua_tointeger(lua_state, 1);
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(Derived*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default */
    udata += 1;
    *(Derived**)udata = new Derived(arg_1);
    get_global_table(lua_state, "Derived");
    lua_setmetatable(lua_state, -2);

    return 1;
}

static int lua___Derived__b_value(lua_State *lua_state)
{}

static int lua___Derived__pt_hello_v(lua_State *lua_state)
{}

static int lua___Derived__pt_world(lua_State *lua_state)
{}

static int lua___Derived__pt_arg_2(lua_State *lua_state)
{}

static int lua___Derived__print_static(lua_State *lua_state)
{}

static int lua___Derived__b1_id(lua_State *lua_state)
{}

static int lua___Derived__set_b1_name(lua_State *lua_state)
{}

static int lua___Derived__print_b1(lua_State *lua_state)
{}

static int lua___Der2__new(lua_State *lua_state)
{
    bool arg_1 = lua_toboolean(lua_state, 1) > 0 ? true : false;
    int32 arg_2 = lua_tointeger(lua_state, 2);
    const char *arg_3 = lua_tostring(lua_state, 3);
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(Der2*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default */
    udata += 1;
    *(Der2**)udata = new Der2(arg_1, arg_2, arg_3);
    get_global_table(lua_state, "Der2");
    lua_setmetatable(lua_state, -2);

    return 1;
}

static int lua___Der2__b_value(lua_State *lua_state)
{}

static int lua___Der2__pt_hello_v(lua_State *lua_state)
{}

static int lua___Der2__pt_world(lua_State *lua_state)
{}

static int lua___Der2__pt_arg_2(lua_State *lua_state)
{}

static int lua___Der2__b1_id(lua_State *lua_state)
{}

static int lua___Der2__set_b1_name(lua_State *lua_state)
{}

static int lua___Der2__print_b1(lua_State *lua_state)
{}

static int lua___Der2__b2_name(lua_State *lua_state)
{}

static int lua___Der2__uniqued_id(lua_State *lua_state)
{}


int main(){}

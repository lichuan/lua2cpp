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

typedef int int32;
int32 ooopp = 2323;

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
    arg::Cpp_Arg *obj = new arg::Cpp_Arg}

static int lua____arg___Cpp_Arg__new1(lua_State *lua_state)
{
    arg::Cpp_Arg *obj = new arg::Cpp_Arg}

static int lua____arg___Cpp_Arg__hp(lua_State *lua_state)
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
    base::Base1 *obj = new base::Base1}

static int lua____base___Base1__r3r3r(lua_State *lua_state)
{}

static int lua____base___Base1__IJ3RJ2(lua_State *lua_state)
{}

static int lua____base___Base1__get_info(lua_State *lua_state)
{}

static int lua____engine____memory___Base2__new(lua_State *lua_state)
{
    engine::memory::Base2 *obj = new engine::memory::Base2}

static int lua____engine____memory___Base2__pt_world_v(lua_State *lua_state)
{}

static int lua____engine____memory___Base2__pt_world(lua_State *lua_state)
{}

static int lua____engine____memory___Base2__pt_arg_2(lua_State *lua_state)
{}

static int lua____engine____memory___Base3___ICls__get_name(lua_State *lua_state)
{}

static int lua____engine____memory___Base3__get_name(lua_State *lua_state)
{}

static int lua___Derived___iii__pt_hello_v(lua_State *lua_state)
{}

static int lua___Derived___iii__pt_world(lua_State *lua_state)
{}

static int lua___Derived___iii__pt_arg_2(lua_State *lua_state)
{}

static int lua___Derived___iii__print_static(lua_State *lua_state)
{}


int main(){}

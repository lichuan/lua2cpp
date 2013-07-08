/*
 @author: lichuan
 @qq: 308831759
 @email: 308831759@qq.com
 @homepage: www.lichuan.me
 @date: 2013-05-11
 @desc: this is the generator of binding code between lua and c++.
*/

#include <string>
#include <list>
#include <cstring>
#include <map>
#include <iostream>
#include "lua.hpp"
#include "lua2cpp.hpp"

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

Script_Mgr* Script_Mgr::m_instance;

Script_Mgr::Script_Mgr()
{
}

Script_Mgr* Script_Mgr::instance()
{
    if(m_instance == NULL)
    {
        m_instance = new Script_Mgr;
    }

    return m_instance;
}

bool Script_Mgr::init()
{
    m_lua_state = luaL_newstate();
    luaL_openlibs(m_lua_state);
}

lua_State* Script_Mgr::lua_state() const
{
    return m_lua_state;
}

int main()
{
}

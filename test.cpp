#include <iostream>
#include "lua.hpp"
#include <string>


using namespace std;

#include "lc.h"



void register_class(lua_State *L)
{
  
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    lua_setglobal(L, "base_cls");
    
    //luaL_register(L, NULL, reg_table1);
}

int main()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    register_class(L);
    int ret = luaL_dostring(L, "_G = {} setfenv(1, {})");
    
    if(ret != 0)
    {
        cout << lua_tostring(L, -1) << endl;
    }

    ret = luaL_dofile(L, "a.lua");

    if(ret != 0)
    {
        cout << lua_tostring(L, -1) << endl;
    }

    string sss = "ddddd"                   "fiejw";
    cout << sss << endl;
    return 0;
}

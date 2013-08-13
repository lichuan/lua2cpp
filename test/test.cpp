#include <string>
#include <cstring>
#include <list>
#include <map>
#include <iostream>
#include "lua.hpp"

using namespace std;
typedef unsigned int uint32;
typedef int int32;

namespace dat_ns
{    
    struct Data
    {
        Data()
        {
            id = 0;
            name = "no name";        
        }
    
        uint32 id;
        string name;    
    };
}


class Test_Lua
{
public:
    Test_Lua()
    {
    }

    dat_ns::Data get_data()
    {
        return m_data;
    }

    void set_data_id(uint32 id)
    {
        m_data.id = id;
    }

    void set_data_name(string name)
    {
        m_data.name = name;
    }

    string get_data_name()
    {
        return m_data.name;
    }
    
    uint32 get_data_id()
    {
        return m_data.id;
    }

    void replace_data(dat_ns::Data &data)
    {
        m_data = data;
    }    
    
private:
    dat_ns::Data m_data;
};

#include "lua2cpp.cpp"
    
int main()
{
    lua_State *lua_state = luaL_newstate();    
    luaL_openlibs(lua_state);
    register_lua(lua_state);    
    
    if(luaL_dofile(lua_state, "test.lua") != 0)
    {
        cout << "err: " << lua_tostring(lua_state, -1) << endl;
    }
}

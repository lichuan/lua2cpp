#ifndef __GAME_SCRIPT_LUA2CPP__
#define __GAME_SCRIPT_LUA2CPP__

class Script_Mgr
{
public:
    static Script_Mgr* instance();
    bool init();
    lua_State* lua_state() const;
    
private:
    Script_Mgr();
    lua_State *m_lua_state;
    static Script_Mgr* m_instance;
};

#endif

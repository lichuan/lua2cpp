/*
 author: lichuan
 qq: 308831759
 email: 308831759@qq.com
 homepage: www.lichuan.me
 github: https://github.com/lichuan/lua2cpp
 date: 2013-05-11
 desc: this is the binding code between lua and c++ generated by lua2cpp.rb
*/

static void get_global_table(lua_State *lua_state, const char *nodes_name)
{
    char buf[1024];
    strcpy(buf, nodes_name);
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

static void build_global_table(lua_State *lua_state, const char *nodes_name)
{
    char buf[1024];
    strcpy(buf, nodes_name);
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
            lua_rawset(lua_state, -3);
        }
    }

    lua_settop(lua_state, 0);
}

static int lua__exe_script(lua_State *lua_state)
{
    const char *arg_1 = luaL_checkstring(lua_state, 1);
    lua_settop(lua_state, 0);
    exe_script(arg_1);

    return 0;
}

static int lua__ooopp(lua_State *lua_state)
{
    std::string v = ooopp;
    lua_pushstring(lua_state, v.c_str());

    return 1;
}

static int lua__get_obj(lua_State *lua_state)
{
    int32 arg_1 = luaL_checkint(lua_state, 1);
    const char *arg_2 = luaL_checkstring(lua_state, 2);
    lua_settop(lua_state, 0);
    const arg::Cpp_Arg *v = &get_ns_obj(arg_1, arg_2);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(arg::Cpp_Arg*));
    uint32 &gc_flag = *udata;
    gc_flag = 0;
    udata += 1;
    *(arg::Cpp_Arg**)udata = (arg::Cpp_Arg*)v;
    luaL_setmetatable(lua_state, "_arg.Cpp_Arg");

    return 1;
}

static int lua____arg___Cpp_Arg__new(lua_State *lua_state)
{
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(arg::Cpp_Arg*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default in constructor */
    udata += 1;
    *(arg::Cpp_Arg**)udata = new arg::Cpp_Arg();
    luaL_setmetatable(lua_state, "_arg.Cpp_Arg");

    return 1;
}

static int lua____arg___Cpp_Arg__new1(lua_State *lua_state)
{
    int32 arg_1 = luaL_checkint(lua_state, 1);
    uint32 *udata_2 = (uint32*)luaL_checkudata(lua_state, 2, "_engine._memory.Base2");
    udata_2 += 1;
    engine::memory::Base2 *arg_2 = *(engine::memory::Base2**)udata_2;
    uint32 *udata_3 = (uint32*)luaL_checkudata(lua_state, 3, "_arg.Cpp_Arg");
    udata_3 += 1;
    arg::Cpp_Arg *arg_3 = *(arg::Cpp_Arg**)udata_3;
    double arg_4 = luaL_checknumber(lua_state, 4);
    const char *arg_5 = luaL_checkstring(lua_state, 5);
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(arg::Cpp_Arg*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default in constructor */
    udata += 1;
    *(arg::Cpp_Arg**)udata = new arg::Cpp_Arg(arg_1, *arg_2, arg_3, arg_4, arg_5);
    luaL_setmetatable(lua_state, "_arg.Cpp_Arg");

    return 1;
}

static int lua____arg___Cpp_Arg__hp(lua_State *lua_state)
{
    int32 v = arg::Cpp_Arg::hp;
    lua_pushinteger(lua_state, v);

    return 1;
}

static int lua____arg___Cpp_Arg__set_d(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "_arg.Cpp_Arg");
    udata += 1;
    arg::Cpp_Arg *obj = *(arg::Cpp_Arg**)udata;
    uint32 *udata_1 = (uint32*)luaL_checkudata(lua_state, 2, "_base.Base1");
    udata_1 += 1;
    base::Base1 *arg_1 = *(base::Base1**)udata_1;
    lua_settop(lua_state, 0);
    obj->d = *arg_1;

    return 0;
}

static int lua____arg___Cpp_Arg__set_hp(lua_State *lua_state)
{
    int32 arg_1 = luaL_checkint(lua_state, 1);
    lua_settop(lua_state, 0);
    arg::Cpp_Arg::hp = arg_1;

    return 0;
}

static int lua____arg___Cpp_Arg__get_v(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_arg.Cpp_Arg");
    udata_self += 1;
    arg::Cpp_Arg *obj = *(arg::Cpp_Arg**)udata_self;
    lua_settop(lua_state, 0);
    int32 v = obj->get_value();
    lua_pushinteger(lua_state, v);

    return 1;
}

static int lua____arg___Cpp_Arg__set_v(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_arg.Cpp_Arg");
    udata_self += 1;
    arg::Cpp_Arg *obj = *(arg::Cpp_Arg**)udata_self;
    int32 arg_1 = luaL_checkint(lua_state, 2);
    lua_settop(lua_state, 0);
    obj->set_value(arg_1);

    return 0;
}

static int lua____arg___Cpp_Arg__garbage_colloect(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "_arg.Cpp_Arg");
    uint32 &gc_flag = *udata;
 
    if(gc_flag == 1)
    {
        udata += 1;
        arg::Cpp_Arg *obj = *(arg::Cpp_Arg**)udata;
        delete obj;
    }

    return 0;
}

static int lua____base___Base1__new(lua_State *lua_state)
{
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(base::Base1*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default in constructor */
    udata += 1;
    *(base::Base1**)udata = new base::Base1();
    luaL_setmetatable(lua_state, "_base.Base1");

    return 1;
}

static int lua____base___Base1__owerid(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_base.Base1");
    udata_self += 1;
    base::Base1 *obj = *(base::Base1**)udata_self;
    int32 v = obj->owerid;
    lua_pushinteger(lua_state, v);

    return 1;
}

static int lua____base___Base1__r3r3r(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_base.Base1");
    udata_self += 1;
    base::Base1 *obj = *(base::Base1**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_hello_v();

    return 0;
}

static int lua____base___Base1__IJ3RJ2(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_base.Base1");
    udata_self += 1;
    base::Base1 *obj = *(base::Base1**)udata_self;
    uint32 *udata_1 = (uint32*)luaL_checkudata(lua_state, 2, "_arg.Cpp_Arg");
    udata_1 += 1;
    arg::Cpp_Arg *arg_1 = *(arg::Cpp_Arg**)udata_1;
    lua_settop(lua_state, 0);
    obj->pt_arg_1(arg_1);

    return 0;
}

static int lua____base___Base1__get_info(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_base.Base1");
    udata_self += 1;
    base::Base1 *obj = *(base::Base1**)udata_self;
    uint32 *udata_1 = (uint32*)luaL_checkudata(lua_state, 2, "_arg.Cpp_Arg");
    udata_1 += 1;
    arg::Cpp_Arg *arg_1 = *(arg::Cpp_Arg**)udata_1;
    uint32 *udata_2 = (uint32*)luaL_checkudata(lua_state, 3, "_engine._memory.Base3");
    udata_2 += 1;
    engine::memory::Base3 *arg_2 = *(engine::memory::Base3**)udata_2;
    lua_settop(lua_state, 0);
    obj->get_info(*arg_1, arg_2);

    return 0;
}

static int lua____base___Base1__garbage_colloect(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "_base.Base1");
    uint32 &gc_flag = *udata;
 
    if(gc_flag == 1)
    {
        udata += 1;
        base::Base1 *obj = *(base::Base1**)udata;
        delete obj;
    }

    return 0;
}

static int lua____engine____memory___Base2__new(lua_State *lua_state)
{
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(engine::memory::Base2*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default in constructor */
    udata += 1;
    *(engine::memory::Base2**)udata = new engine::memory::Base2();
    luaL_setmetatable(lua_state, "_engine._memory.Base2");

    return 1;
}

static int lua____engine____memory___Base2__pt_world_v(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_engine._memory.Base2");
    udata_self += 1;
    engine::memory::Base2 *obj = *(engine::memory::Base2**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_world_v();

    return 0;
}

static int lua____engine____memory___Base2__pt_world(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_engine._memory.Base2");
    udata_self += 1;
    engine::memory::Base2 *obj = *(engine::memory::Base2**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_world();

    return 0;
}

static int lua____engine____memory___Base2__pt_arg_2(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_engine._memory.Base2");
    udata_self += 1;
    engine::memory::Base2 *obj = *(engine::memory::Base2**)udata_self;
    uint32 *udata_1 = (uint32*)luaL_checkudata(lua_state, 2, "_arg.Cpp_Arg");
    udata_1 += 1;
    arg::Cpp_Arg *arg_1 = *(arg::Cpp_Arg**)udata_1;
    lua_settop(lua_state, 0);
    engine::memory::Base3 *v = new engine::memory::Base3;
    *v = obj->pt_arg_2(arg_1);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(engine::memory::Base3*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* no ptr, no ref, it's a new obj, so it need gc */
    udata += 1;
    *(engine::memory::Base3**)udata = v;
    luaL_setmetatable(lua_state, "_engine._memory.Base3");

    return 1;
}

static int lua____engine____memory___Base2__garbage_colloect(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "_engine._memory.Base2");
    uint32 &gc_flag = *udata;
 
    if(gc_flag == 1)
    {
        udata += 1;
        engine::memory::Base2 *obj = *(engine::memory::Base2**)udata;
        delete obj;
    }

    return 0;
}

static int lua____engine____memory___Base3___ICls__get_name(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_engine._memory.Base3.ICls");
    udata_self += 1;
    engine::memory::Base3::ICls *obj = *(engine::memory::Base3::ICls**)udata_self;
    int32 arg_1 = luaL_checkint(lua_state, 2);
    lua_settop(lua_state, 0);
    std::string v = obj->get_name(arg_1);
    lua_pushstring(lua_state, v.c_str());

    return 1;
}

static int lua____engine____memory___Base3___ICls__garbage_colloect(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "_engine._memory.Base3.ICls");
    uint32 &gc_flag = *udata;
 
    if(gc_flag == 1)
    {
        udata += 1;
        engine::memory::Base3::ICls *obj = *(engine::memory::Base3::ICls**)udata;
        delete obj;
    }

    return 0;
}

static int lua____engine____memory____lll__get_base(lua_State *lua_state)
{
    lua_settop(lua_state, 0);
    const base::Base1 *v = engine::memory::lll::get_base();
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(base::Base1*));
    uint32 &gc_flag = *udata;
    gc_flag = 1;
    udata += 1;
    *(base::Base1**)udata = (base::Base1*)v;
    luaL_setmetatable(lua_state, "_base.Base1");

    return 1;
}

static int lua____engine____memory____lll__get_name(lua_State *lua_state)
{
    int32 arg_1 = luaL_checkint(lua_state, 1);
    lua_settop(lua_state, 0);
    std::string v = engine::memory::lll::get_name(arg_1);
    lua_pushstring(lua_state, v.c_str());

    return 1;
}

static int lua____engine____memory___Base3__get_name(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_engine._memory.Base3");
    udata_self += 1;
    engine::memory::Base3 *obj = *(engine::memory::Base3**)udata_self;
    int32 arg_1 = luaL_checkint(lua_state, 2);
    lua_settop(lua_state, 0);
    std::string v = obj->get_name(arg_1);
    lua_pushstring(lua_state, v.c_str());

    return 1;
}

static int lua____engine____memory___Base3__bb1(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_engine._memory.Base3");
    udata_self += 1;
    engine::memory::Base3 *obj = *(engine::memory::Base3**)udata_self;
    const gabriel::b1 *v = obj->bb1;
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(gabriel::b1*));
    uint32 &gc_flag = *udata;
    gc_flag = 0;
    udata += 1;
    *(gabriel::b1**)udata = (gabriel::b1*)v;
    luaL_setmetatable(lua_state, "_gabriel.b1");

    return 1;
}

static int lua____engine____memory___Base3__garbage_colloect(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "_engine._memory.Base3");
    uint32 &gc_flag = *udata;
 
    if(gc_flag == 1)
    {
        udata += 1;
        engine::memory::Base3 *obj = *(engine::memory::Base3**)udata;
        delete obj;
    }

    return 0;
}

static int lua____gabriel___b1__b1_id(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_gabriel.b1");
    udata_self += 1;
    gabriel::b1 *obj = *(gabriel::b1**)udata_self;
    int32 v = obj->b1_id;
    lua_pushinteger(lua_state, v);

    return 1;
}

static int lua____gabriel___b1__set_b1_name(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_gabriel.b1");
    udata_self += 1;
    gabriel::b1 *obj = *(gabriel::b1**)udata_self;
    const char *arg_1 = luaL_checkstring(lua_state, 2);
    lua_settop(lua_state, 0);
    obj->modify_name(arg_1);

    return 0;
}

static int lua____gabriel___b1__print_b1(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_gabriel.b1");
    udata_self += 1;
    gabriel::b1 *obj = *(gabriel::b1**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_b1();

    return 0;
}

static int lua____gabriel___b1__garbage_colloect(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "_gabriel.b1");
    uint32 &gc_flag = *udata;
 
    if(gc_flag == 1)
    {
        udata += 1;
        gabriel::b1 *obj = *(gabriel::b1**)udata;
        delete obj;
    }

    return 0;
}

static int lua____gabriel___b2__b2_name(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_gabriel.b2");
    udata_self += 1;
    gabriel::b2 *obj = *(gabriel::b2**)udata_self;
    std::string v = obj->b2_name;
    lua_pushstring(lua_state, v.c_str());

    return 1;
}

static int lua____gabriel___b2__last_name(lua_State *lua_state)
{
    lua_settop(lua_state, 0);
    std::string v = gabriel::b2::last_name();
    lua_pushstring(lua_state, v.c_str());

    return 1;
}

static int lua____gabriel___b2__uniqued_id(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "_gabriel.b2");
    udata_self += 1;
    gabriel::b2 *obj = *(gabriel::b2**)udata_self;
    lua_settop(lua_state, 0);
    uint32 v = obj->uniqued_id();
    lua_pushunsigned(lua_state, v);

    return 1;
}

static int lua____gabriel___b2__garbage_colloect(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "_gabriel.b2");
    uint32 &gc_flag = *udata;
 
    if(gc_flag == 1)
    {
        udata += 1;
        gabriel::b2 *obj = *(gabriel::b2**)udata;
        delete obj;
    }

    return 0;
}

static int lua___Derived__new(lua_State *lua_state)
{
    int32 arg_1 = luaL_checkint(lua_state, 1);
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(Derived*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default in constructor */
    udata += 1;
    *(Derived**)udata = new Derived(arg_1);
    luaL_setmetatable(lua_state, "Derived");

    return 1;
}

static int lua___Derived__b_value(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Derived");
    udata_self += 1;
    Derived *obj = *(Derived**)udata_self;
    bool v = obj->b_value;
    lua_pushboolean(lua_state, v ? 1 : 0);

    return 1;
}

static int lua___Derived__pt_hello_v(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Derived");
    udata_self += 1;
    Derived *obj = *(Derived**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_hello_v();

    return 0;
}

static int lua___Derived__pt_world(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Derived");
    udata_self += 1;
    Derived *obj = *(Derived**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_world();

    return 0;
}

static int lua___Derived__pt_arg_2(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Derived");
    udata_self += 1;
    Derived *obj = *(Derived**)udata_self;
    uint32 *udata_1 = (uint32*)luaL_checkudata(lua_state, 2, "_arg.Cpp_Arg");
    udata_1 += 1;
    arg::Cpp_Arg *arg_1 = *(arg::Cpp_Arg**)udata_1;
    int32 arg_2 = luaL_checkint(lua_state, 3);
    const char *arg_3 = luaL_checkstring(lua_state, 4);
    lua_settop(lua_state, 0);
    std::string v = obj->pt_arg_2(arg_1, arg_2, arg_3);
    lua_pushstring(lua_state, v.c_str());

    return 1;
}

static int lua___Derived__print_static(lua_State *lua_state)
{
    const char *arg_1 = luaL_checkstring(lua_state, 1);
    lua_settop(lua_state, 0);
    Derived::print_static(arg_1);

    return 0;
}

static int lua___Derived__b1_id(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Derived");
    udata_self += 1;
    Derived *obj = *(Derived**)udata_self;
    int32 v = obj->b1_id;
    lua_pushinteger(lua_state, v);

    return 1;
}

static int lua___Derived__set_b1_name(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Derived");
    udata_self += 1;
    Derived *obj = *(Derived**)udata_self;
    const char *arg_1 = luaL_checkstring(lua_state, 2);
    lua_settop(lua_state, 0);
    obj->modify_name(arg_1);

    return 0;
}

static int lua___Derived__print_b1(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Derived");
    udata_self += 1;
    Derived *obj = *(Derived**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_b1();

    return 0;
}

static int lua___Derived__garbage_colloect(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "Derived");
    uint32 &gc_flag = *udata;
 
    if(gc_flag == 1)
    {
        udata += 1;
        Derived *obj = *(Derived**)udata;
        delete obj;
    }

    return 0;
}

static int lua___Der2__new(lua_State *lua_state)
{
    bool arg_1 = luaL_checkint(lua_state, 1) > 0 ? true : false;
    int32 arg_2 = luaL_checkint(lua_state, 2);
    const char *arg_3 = luaL_checkstring(lua_state, 3);
    lua_settop(lua_state, 0);
    uint32 *udata = (uint32*)lua_newuserdata(lua_state, sizeof(uint32) + sizeof(Der2*));
    uint32 &gc_flag = *udata;
    gc_flag = 1; /* need gc default in constructor */
    udata += 1;
    *(Der2**)udata = new Der2(arg_1, arg_2, arg_3);
    luaL_setmetatable(lua_state, "Der2");

    return 1;
}

static int lua___Der2__b_value(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    udata_self += 1;
    Der2 *obj = *(Der2**)udata_self;
    bool v = obj->b_value;
    lua_pushboolean(lua_state, v ? 1 : 0);

    return 1;
}

static int lua___Der2__pt_hello_v(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    udata_self += 1;
    Der2 *obj = *(Der2**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_hello_v();

    return 0;
}

static int lua___Der2__pt_world(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    udata_self += 1;
    Der2 *obj = *(Der2**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_world();

    return 0;
}

static int lua___Der2__pt_arg_2(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    udata_self += 1;
    Der2 *obj = *(Der2**)udata_self;
    uint32 *udata_1 = (uint32*)luaL_checkudata(lua_state, 2, "_arg.Cpp_Arg");
    udata_1 += 1;
    arg::Cpp_Arg *arg_1 = *(arg::Cpp_Arg**)udata_1;
    int32 arg_2 = luaL_checkint(lua_state, 3);
    const char *arg_3 = luaL_checkstring(lua_state, 4);
    lua_settop(lua_state, 0);
    std::string v = obj->pt_arg_2(arg_1, arg_2, arg_3);
    lua_pushstring(lua_state, v.c_str());

    return 1;
}

static int lua___Der2__b1_id(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    udata_self += 1;
    Der2 *obj = *(Der2**)udata_self;
    int32 v = obj->b1_id;
    lua_pushinteger(lua_state, v);

    return 1;
}

static int lua___Der2__set_b1_name(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    udata_self += 1;
    Der2 *obj = *(Der2**)udata_self;
    const char *arg_1 = luaL_checkstring(lua_state, 2);
    lua_settop(lua_state, 0);
    obj->modify_name(arg_1);

    return 0;
}

static int lua___Der2__print_b1(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    udata_self += 1;
    Der2 *obj = *(Der2**)udata_self;
    lua_settop(lua_state, 0);
    obj->pt_b1();

    return 0;
}

static int lua___Der2__b2_name(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    udata_self += 1;
    Der2 *obj = *(Der2**)udata_self;
    std::string v = obj->b2_name;
    lua_pushstring(lua_state, v.c_str());

    return 1;
}

static int lua___Der2__uniqued_id(lua_State *lua_state)
{
    uint32 *udata_self = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    udata_self += 1;
    Der2 *obj = *(Der2**)udata_self;
    lua_settop(lua_state, 0);
    uint32 v = obj->uniqued_id();
    lua_pushunsigned(lua_state, v);

    return 1;
}

static int lua___Der2__garbage_colloect(lua_State *lua_state)
{
    uint32 *udata = (uint32*)luaL_checkudata(lua_state, 1, "Der2");
    uint32 &gc_flag = *udata;
 
    if(gc_flag == 1)
    {
        udata += 1;
        Der2 *obj = *(Der2**)udata;
        delete obj;
    }

    return 0;
}

static void register_lua2cpp(lua_State *lua_state)
{
    /* register global namespace */
    lua_pushcfunction(lua_state, lua__exe_script);
    lua_setglobal(lua_state, "exe_script");
    lua_pushcfunction(lua_state, lua__ooopp);
    lua_setglobal(lua_state, "ooopp");
    lua_pushcfunction(lua_state, lua__get_obj);
    lua_setglobal(lua_state, "get_obj");

    /* register non-global namespace */
    build_global_table(lua_state, "_arg.Cpp_Arg");
    build_global_table(lua_state, "_base.Base1");
    build_global_table(lua_state, "_engine._memory.Base2");
    build_global_table(lua_state, "_engine._memory.Base3.ICls");
    build_global_table(lua_state, "_engine._memory._lll");
    build_global_table(lua_state, "_engine._memory.Base3");
    build_global_table(lua_state, "_gabriel.b1");
    build_global_table(lua_state, "_gabriel.b2");
    build_global_table(lua_state, "Derived");
    build_global_table(lua_state, "Der2");

    {
        luaL_Reg _arg_Cpp_Arg[] = 
        {
            {"new", lua____arg___Cpp_Arg__new},
            {"new1", lua____arg___Cpp_Arg__new1},
            {"hp", lua____arg___Cpp_Arg__hp},
            {"set_d", lua____arg___Cpp_Arg__set_d},
            {"set_hp", lua____arg___Cpp_Arg__set_hp},
            {"get_v", lua____arg___Cpp_Arg__get_v},
            {"set_v", lua____arg___Cpp_Arg__set_v},
            {"__gc", lua____arg___Cpp_Arg__garbage_colloect},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "_arg.Cpp_Arg");
        luaL_setfuncs(lua_state, _arg_Cpp_Arg, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "_arg.Cpp_Arg");
        luaL_setfuncs(lua_state, _arg_Cpp_Arg, 0);
    }

    {
        luaL_Reg _base_Base1[] = 
        {
            {"new", lua____base___Base1__new},
            {"owerid", lua____base___Base1__owerid},
            {"r3r3r", lua____base___Base1__r3r3r},
            {"IJ3RJ2", lua____base___Base1__IJ3RJ2},
            {"get_info", lua____base___Base1__get_info},
            {"__gc", lua____base___Base1__garbage_colloect},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "_base.Base1");
        luaL_setfuncs(lua_state, _base_Base1, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "_base.Base1");
        luaL_setfuncs(lua_state, _base_Base1, 0);
    }

    {
        luaL_Reg _engine__memory_Base2[] = 
        {
            {"new", lua____engine____memory___Base2__new},
            {"pt_world_v", lua____engine____memory___Base2__pt_world_v},
            {"pt_world", lua____engine____memory___Base2__pt_world},
            {"pt_arg_2", lua____engine____memory___Base2__pt_arg_2},
            {"__gc", lua____engine____memory___Base2__garbage_colloect},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "_engine._memory.Base2");
        luaL_setfuncs(lua_state, _engine__memory_Base2, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "_engine._memory.Base2");
        luaL_setfuncs(lua_state, _engine__memory_Base2, 0);
    }

    {
        luaL_Reg _engine__memory_Base3_ICls[] = 
        {
            {"get_name", lua____engine____memory___Base3___ICls__get_name},
            {"__gc", lua____engine____memory___Base3___ICls__garbage_colloect},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "_engine._memory.Base3.ICls");
        luaL_setfuncs(lua_state, _engine__memory_Base3_ICls, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "_engine._memory.Base3.ICls");
        luaL_setfuncs(lua_state, _engine__memory_Base3_ICls, 0);
    }

    {
        luaL_Reg _engine__memory__lll[] = 
        {
            {"get_base", lua____engine____memory____lll__get_base},
            {"get_name", lua____engine____memory____lll__get_name},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "_engine._memory._lll");
        luaL_setfuncs(lua_state, _engine__memory__lll, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "_engine._memory._lll");
        luaL_setfuncs(lua_state, _engine__memory__lll, 0);
    }

    {
        luaL_Reg _engine__memory_Base3[] = 
        {
            {"get_name", lua____engine____memory___Base3__get_name},
            {"bb1", lua____engine____memory___Base3__bb1},
            {"__gc", lua____engine____memory___Base3__garbage_colloect},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "_engine._memory.Base3");
        luaL_setfuncs(lua_state, _engine__memory_Base3, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "_engine._memory.Base3");
        luaL_setfuncs(lua_state, _engine__memory_Base3, 0);
    }

    {
        luaL_Reg _gabriel_b1[] = 
        {
            {"b1_id", lua____gabriel___b1__b1_id},
            {"set_b1_name", lua____gabriel___b1__set_b1_name},
            {"print_b1", lua____gabriel___b1__print_b1},
            {"__gc", lua____gabriel___b1__garbage_colloect},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "_gabriel.b1");
        luaL_setfuncs(lua_state, _gabriel_b1, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "_gabriel.b1");
        luaL_setfuncs(lua_state, _gabriel_b1, 0);
    }

    {
        luaL_Reg _gabriel_b2[] = 
        {
            {"b2_name", lua____gabriel___b2__b2_name},
            {"last_name", lua____gabriel___b2__last_name},
            {"uniqued_id", lua____gabriel___b2__uniqued_id},
            {"__gc", lua____gabriel___b2__garbage_colloect},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "_gabriel.b2");
        luaL_setfuncs(lua_state, _gabriel_b2, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "_gabriel.b2");
        luaL_setfuncs(lua_state, _gabriel_b2, 0);
    }

    {
        luaL_Reg Derived[] = 
        {
            {"new", lua___Derived__new},
            {"b_value", lua___Derived__b_value},
            {"pt_hello_v", lua___Derived__pt_hello_v},
            {"pt_world", lua___Derived__pt_world},
            {"pt_arg_2", lua___Derived__pt_arg_2},
            {"print_static", lua___Derived__print_static},
            {"b1_id", lua___Derived__b1_id},
            {"set_b1_name", lua___Derived__set_b1_name},
            {"print_b1", lua___Derived__print_b1},
            {"__gc", lua___Derived__garbage_colloect},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "Derived");
        luaL_setfuncs(lua_state, Derived, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "Derived");
        luaL_setfuncs(lua_state, Derived, 0);
    }

    {
        luaL_Reg Der2[] = 
        {
            {"new", lua___Der2__new},
            {"b_value", lua___Der2__b_value},
            {"pt_hello_v", lua___Der2__pt_hello_v},
            {"pt_world", lua___Der2__pt_world},
            {"pt_arg_2", lua___Der2__pt_arg_2},
            {"b1_id", lua___Der2__b1_id},
            {"set_b1_name", lua___Der2__set_b1_name},
            {"print_b1", lua___Der2__print_b1},
            {"b2_name", lua___Der2__b2_name},
            {"uniqued_id", lua___Der2__uniqued_id},
            {"__gc", lua___Der2__garbage_colloect},
            {NULL, NULL}
        };

        lua_settop(lua_state, 0);
        luaL_newmetatable(lua_state, "Der2");
        luaL_setfuncs(lua_state, Der2, 0);
        lua_setfield(lua_state, -1, "__index");
        get_global_table(lua_state, "Der2");
        luaL_setfuncs(lua_state, Der2, 0);
    }
}

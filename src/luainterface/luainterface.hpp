#ifndef LUA_INTERFACE_HPP
#define LUA_INTERFACE_HPP

#include <lua.hpp>
#include <QVector2D>

class LuaInterface {
public:
    LuaInterface();
    ~LuaInterface();

    void runScript(const std::string& scriptFile);
    
    static int lua_move_to(lua_State* L);
    static int lua_face_to(lua_State* L);

private:
    lua_State* L;
};

#endif // LUA_INTERFACE_HPP

#ifndef LUA_INTERFACE_HPP
#define LUA_INTERFACE_HPP

#include <lua.hpp>
#include <QVector2D>
#include "radio.hpp"

class LuaInterface {
public:
    LuaInterface(Radio* radio);
    ~LuaInterface();

    void runScript(const std::string& scriptFile);
    
    static int lua_move_to(lua_State* L);
    static int lua_face_to(lua_State* L);

private:
    lua_State* L;
    Radio* radio;
};

#endif // LUA_INTERFACE_HPP

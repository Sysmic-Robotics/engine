#ifndef LUA_INTERFACE_HPP
#define LUA_INTERFACE_HPP


#include <lua.hpp>
#include <QVector2D>
#include "radio.hpp"
#include "world.hpp"

class LuaInterface {
public:
    LuaInterface(Radio* radio_, World* world);
    ~LuaInterface();

    void runScript(const std::string& scriptFile);
    void callProcess();  // Added function to call Lua's process() every frame

    static int lua_move_to(lua_State* L);
    static int lua_get_robot_state(lua_State* L);
    static int lua_get_ball_state(lua_State* L);
    static int lua_face_to(lua_State* L);
    static int lua_print_override(lua_State* L);
    static int lua_kickx(lua_State* L);
    static int lua_kickz(lua_State* L);
    static int lua_dribbler(lua_State* L);

    bool haveScript();

private:
    lua_State* L;
    Radio* m_radio;
    World* m_world;
    bool m_haveScript = false;
};

#endif // LUA_INTERFACE_HPP

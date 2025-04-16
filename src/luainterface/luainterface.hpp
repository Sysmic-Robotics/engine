#ifndef LUA_INTERFACE_SOL_HPP
#define LUA_INTERFACE_SOL_HPP

#include <sol/sol.hpp>
#include <QVector2D>
#include "radio.hpp"
#include "world.hpp"

class LuaInterface {
public:
    LuaInterface(Radio* radio_, World* world);
    ~LuaInterface();

    // Loads a Lua script file (reinitializes the Lua state)
    void runScript();
    // Calls the global Lua function "process"
    void callProcess();

    bool haveScript();

private:
    // Helper to register all your functions with Lua
    void register_functions();

    sol::state m_lua;
    Radio* m_radio;
    World* m_world;
    bool m_haveScript = false;
};

#endif // LUA_INTERFACE_SOL_HPP

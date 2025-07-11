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
    void runScript(const QString& scriptPath);
    // Calls the global Lua function "process"
    void pauseScript();

    void resumeScript();

    void callProcess();

private:
    // Helper to register all your functions with Lua
    void register_functions();

    sol::state m_lua;
    Radio* m_radio;
    World* m_world;
    bool m_haveScript = false;
    bool m_isPaused = false;
};

#endif // LUA_INTERFACE_SOL_HPP

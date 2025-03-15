#include "luainterface.hpp"
#include "motion.hpp"
#include <iostream>

// Constructor: Initialize Lua
LuaInterface::LuaInterface(Radio* radio_) {
    radio = radio_;

    L = luaL_newstate();
    luaL_openlibs(L); // Load Lua standard libraries

    // Register C++ functions to Lua
    lua_register(L, "move_to", lua_move_to);
    lua_register(L, "face_to", lua_face_to);
}

// Destructor: Close Lua
LuaInterface::~LuaInterface() {
    if (L) {
        lua_close(L);
    }
}

// Run a Lua script file
void LuaInterface::runScript(const std::string& scriptFile) {
    if (luaL_dofile(L, scriptFile.c_str()) != LUA_OK) {
        std::cerr << "Error loading script: " << lua_tostring(L, -1) << std::endl;
    }
}

// C++ function exposed to Lua for moving a robot
int LuaInterface::lua_move_to(lua_State* L) {
    int robotId = lua_tointeger(L, 1);
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);

    std::cout << "[Lua] Moving robot " << robotId << " to (" << x << ", " << y << ")" << std::endl;

    // Call your actual C++ move_to function (to be connected in MainApp)
    // Example: MainApp::getInstance()->moveRobot(robotId, QVector2D(x, y));
    static Motion motion;
    //motion.to_point()
    radio->appendCommand();

    return 0;
}

// C++ function exposed to Lua for rotating a robot
int LuaInterface::lua_face_to(lua_State* L) {
    int robotId = lua_tointeger(L, 1);
    double targetX = lua_tonumber(L, 2);
    double targetY = lua_tonumber(L, 3);

    std::cout << "[Lua] Rotating robot " << robotId << " to face (" << targetX << ", " << targetY << ")" << std::endl;

    // Call your actual C++ face_to function
    // Example: MainApp::getInstance()->rotateRobot(robotId, QVector2D(targetX, targetY));

    return 0;
}

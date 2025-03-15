#include "luainterface.hpp"
#include "motion.hpp"
#include <iostream>

// Constructor: Initialize Lua
LuaInterface::LuaInterface(Radio* radio, World* world) {
    m_radio = radio;
    m_world = world;

    L = luaL_newstate();
    luaL_openlibs(L); // Load Lua standard libraries

    // Store 'this' as a userdata in Lua
    lua_pushlightuserdata(L, this);
    lua_setglobal(L, "LuaInstance");

    // Override Lua's print() function
    lua_pushcfunction(L, LuaInterface::lua_print_override);
    lua_setglobal(L, "print");

    // Register C++ functions to Lua
    lua_register(L, "move_to", lua_move_to);
    lua_register(L, "face_to", lua_face_to);
    lua_register(L, "get_robot_state", lua_get_robot_state);
}

// Destructor: Close Lua
LuaInterface::~LuaInterface() {
    if (L) {
        lua_close(L);
    }
}

void LuaInterface::runScript(const std::string& scriptFile) {
    // Close existing Lua state to reset the environment
    if (L) {
        lua_close(L);
    }

    // Create a new Lua state
    L = luaL_newstate();
    luaL_openlibs(L);

    // Restore LuaInstance reference
    lua_pushlightuserdata(L, this);
    lua_setglobal(L, "LuaInstance");

    std::string scriptDir = "/home/gerson/Sysmic/CondorSSL/src/luainterface/"; // Change to your absolute path
    m_haveScript = true;
    std::string luaPathCmd = "package.path = '" + scriptDir + "?.lua;' .. package.path";
    luaL_dostring(L, luaPathCmd.c_str());

    // Register functions again
    lua_register(L, "move_to", lua_move_to);
    lua_register(L, "get_robot_state", lua_get_robot_state);
    lua_register(L, "face_to", lua_face_to);
    
    std::cout << "[Lua] Loading script: " << scriptFile << std::endl;

    if (luaL_dofile(L, scriptFile.c_str()) != LUA_OK) {
        std::cerr << "[Lua] Error loading script: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1); // Remove error from stack
    } else {
        std::cout << "[Lua] Script loaded successfully!" << std::endl;
    }
}

void LuaInterface::callProcess() {
    lua_getglobal(L, "process"); // Get the Lua function process()

    if (!lua_isfunction(L, -1)) {
        std::cerr << "[Lua] Error: process() is not defined in script!" << std::endl;
        lua_pop(L, 1); // Remove invalid value from stack
        return;
    }

    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        std::cerr << "[Lua] Runtime error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1); // Remove error message from stack
    }
}


int LuaInterface::lua_move_to(lua_State* L) {
    // Retrieve 'LuaInstance' from Lua
    lua_getglobal(L, "LuaInstance");
    LuaInterface* instance = static_cast<LuaInterface*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (!instance || !instance->m_world || !instance->m_radio) {
        std::cerr << "Error: LuaInterface, World, or Radio instance is null!" << std::endl;
        return 0;
    }

    // Ensure correct number of arguments (robotId, team, point table)
    if (lua_gettop(L) != 3 || !lua_isinteger(L, 1) || !lua_isinteger(L, 2) || !lua_istable(L, 3)) {
        std::cerr << "[Lua] Error: move_to(robotId, team, point) expects (int, int, table)" << std::endl;
        return 0;
    }

    // Extract robot ID
    int robotId = lua_tointeger(L, 1);

    // Extract team
    int team = lua_tointeger(L, 2);

    // Extract point (x, y) from Lua table (3rd argument)
    lua_getfield(L, 3, "x");
    double x = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 3, "y");
    double y = lua_tonumber(L, -1);
    lua_pop(L, 1);

    // Ensure the robot exists in the world
    RobotState robotState = instance->m_world->getRobotState(robotId, team);
    if (!robotState.isActive()) {
        std::cerr << "[Lua] Error: Robot " << robotId << " is inactive or not found!" << std::endl;
        return 0;
    }

    // Generate movement command
    static Motion motion;
    MotionCommand cmd = motion.to_point(robotState, QVector2D(x, y));

    // Send command to radio
    instance->m_radio->appendCommand(cmd);

    std::cout << "[Lua] Moving Robot " << robotId << " (Team " << team << ") to ("
              << x << ", " << y << ")" << std::endl;

    return 0;
}



int LuaInterface::lua_get_robot_state(lua_State* L) {
    lua_getglobal(L, "LuaInstance");
    LuaInterface* instance = static_cast<LuaInterface*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (!instance || !instance->m_world) {
        std::cerr << "Error: LuaInterface or World instance is null!" << std::endl;
        return 0;
    }

    int robotId = lua_tointeger(L, 1);
    int team = lua_tointeger(L, 2);

    RobotState robotState = instance->m_world->getRobotState(robotId, team);

    // Return as a Lua table
    lua_newtable(L);

    lua_pushinteger(L, robotState.getId());
    lua_setfield(L, -2, "id");

    lua_pushinteger(L, robotState.getTeam());
    lua_setfield(L, -2, "team");

    lua_pushnumber(L, robotState.getPosition().x());
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, robotState.getPosition().y());
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, robotState.getVelocity().x());
    lua_setfield(L, -2, "vel_x");

    lua_pushnumber(L, robotState.getVelocity().y());
    lua_setfield(L, -2, "vel_y");

    lua_pushnumber(L, robotState.getOrientation());
    lua_setfield(L, -2, "orientation");

    lua_pushboolean(L, robotState.isActive());
    lua_setfield(L, -2, "active");

    return 1; // Returning 1 table
}

// C++ function exposed to Lua for rotating a robot
int LuaInterface::lua_face_to(lua_State* L) {
    int robotId = lua_tointeger(L, 1);
    double targetX = lua_tonumber(L, 2);
    double targetY = lua_tonumber(L, 3);

    std::cout << "[Lua] Rotating robot " << robotId << " to face (" << targetX << ", " << targetY << ")" << std::endl;

    return 0;
}

bool LuaInterface::haveScript() { return m_haveScript; }

// Static function for overriding Lua's print() function
int LuaInterface::lua_print_override(lua_State* L) {
    int nargs = lua_gettop(L);
    std::string output;

    for (int i = 1; i <= nargs; i++) {
        if (lua_isstring(L, i)) {
            output += lua_tostring(L, i);
        } else {
            output += "[non-string value]";
        }
        output += " ";
    }

    std::cout << "[Lua] " << output << std::endl;
    return 0;
}

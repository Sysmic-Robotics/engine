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
    lua_register(L, "get_ball_state", lua_get_ball_state);
    lua_register(L, "kickx", lua_kickx);
    lua_register(L, "kickz", lua_kickz);
    lua_register(L, "dribbler", lua_dribbler);
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
    lua_register(L, "get_ball_state", lua_get_ball_state);
    lua_register(L, "kickx", lua_kickx);
    lua_register(L, "kickz", lua_kickz);
    lua_register(L, "dribbler", lua_dribbler);
    
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
    instance->m_radio->addMotionCommand(cmd);

    return 0;
}

int LuaInterface::lua_get_ball_state(lua_State* L) {
    lua_getglobal(L, "LuaInstance");
    LuaInterface* instance = static_cast<LuaInterface*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (!instance || !instance->m_world) {
        std::cerr << "Error: LuaInterface or World instance is null!" << std::endl;
        return 0;
    }

    BallState ballState = instance->m_world->getBallState();

    // Return as a Lua table
    lua_newtable(L);


    lua_pushnumber(L, ballState.getPosition().x());
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, ballState.getPosition().y());
    lua_setfield(L, -2, "y");


    return 1; // Returning 1 table
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
        std::cerr << "[Lua] Error: face_to(robotId, team, point) expects (int, int, table)" << std::endl;
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
        MotionCommand cmd = motion.face_to(robotState, QVector2D(x, y));
    
        // Send command to radio
        instance->m_radio->addMotionCommand(cmd);


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


int LuaInterface::lua_kickx(lua_State* L) {
    // Retrieve the LuaInterface instance from the global variable
    lua_getglobal(L, "LuaInstance");
    LuaInterface* instance = static_cast<LuaInterface*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (!instance || !instance->m_radio) {
        std::cerr << "[Lua] Error: LuaInstance or Radio is null!" << std::endl;
        return 0;
    }

    // Expecting two arguments: robotId and team.
    if (lua_gettop(L) != 2 || !lua_isinteger(L, 1) || !lua_isinteger(L, 2)) {
        std::cerr << "[Lua] Error: kickx(robotId, team) expects two integers." << std::endl;
        return 0;
    }

    int robotId = lua_tointeger(L, 1);
    int team = lua_tointeger(L, 2);

    // Create a kicker command for this robot and set kickX flag.
    KickerCommand command(robotId, team);
    command.setKickX(true);

    instance->m_radio->addKickerCommand(command);
    return 0;
}

int LuaInterface::lua_kickz(lua_State* L) {
    // Retrieve LuaInterface instance
    lua_getglobal(L, "LuaInstance");
    LuaInterface* instance = static_cast<LuaInterface*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (!instance || !instance->m_radio) {
        std::cerr << "[Lua] Error: LuaInstance or Radio is null!" << std::endl;
        return 0;
    }

    // Expecting two arguments: robotId and team.
    if (lua_gettop(L) != 2 || !lua_isinteger(L, 1) || !lua_isinteger(L, 2)) {
        std::cerr << "[Lua] Error: kickz(robotId, team) expects two integers." << std::endl;
        return 0;
    }

    int robotId = lua_tointeger(L, 1);
    int team = lua_tointeger(L, 2);

    // Create a kicker command and set kickZ flag.
    KickerCommand command(robotId, team);
    command.setKickZ(true);

    instance->m_radio->addKickerCommand(command);
    return 0;
}

int LuaInterface::lua_dribbler(lua_State* L) {
    // Retrieve LuaInterface instance
    lua_getglobal(L, "LuaInstance");
    LuaInterface* instance = static_cast<LuaInterface*>(lua_touserdata(L, -1));
    lua_pop(L, 1);

    if (!instance || !instance->m_radio) {
        std::cerr << "[Lua] Error: LuaInstance or Radio is null!" << std::endl;
        return 0;
    }

    // Expecting three arguments: robotId, team, and dribbler speed.
    if (lua_gettop(L) != 3 || !lua_isinteger(L, 1) || !lua_isinteger(L, 2) || !lua_isnumber(L, 3)) {
        std::cerr << "[Lua] Error: dribbler(robotId, team, speed) expects (int, int, number)." << std::endl;
        return 0;
    }

    int robotId = lua_tointeger(L, 1);
    int team = lua_tointeger(L, 2);
    double speed = lua_tonumber(L, 3);

    // Clamp speed to the range [0, 10]
    if (speed < 0) speed = 0;
    if (speed > 10) speed = 10;

    // Create a kicker command and set the dribbler speed.
    KickerCommand command(robotId, team);
    command.setDribbler(speed);

    instance->m_radio->addKickerCommand(command);
    return 0;
}
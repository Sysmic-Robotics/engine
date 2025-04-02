#include "luainterface.hpp"
#include "motion.hpp"
#include <iostream>
#include <sstream>

LuaInterface::LuaInterface(Radio* radio, World* world)
    : m_radio(radio), m_world(world)
{
    // Open required Lua libraries
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os, sol::lib::string);

    // Override Lua's print() function with a custom lambda
    lua.set_function("print", [](sol::variadic_args args) {
        std::string output;
        for (auto v : args) {
            if (v.is<std::string>()) {
                output += v.as<std::string>();
            } else {
                output += "[non-string value]";
            }
            output += " ";
        }
        std::cout << "[Lua] " << output << std::endl;
    });

    // Register your game-specific functions
    register_functions();
}

LuaInterface::~LuaInterface() {
    // No manual state cleanup is needed; sol::state cleans up automatically.
}

void LuaInterface::register_functions() {
    // move_to(robotId, team, point)
    lua.set_function("move_to", [this](int robotId, int team, sol::table point) {
        if (!m_world || !m_radio) {
            std::cerr << "Error: LuaInterface, World, or Radio instance is null!" << std::endl;
            return;
        }
        double x = point["x"];
        double y = point["y"];
        RobotState robotState = m_world->getRobotState(robotId, team);
        if (!robotState.isActive()) {
            std::cerr << "[Lua] Error: Robot " << robotId << " is inactive or not found!" << std::endl;
            return;
        }
        static Motion motion;
        MotionCommand cmd = motion.to_point(robotState, QVector2D(x, y));
        m_radio->addMotionCommand(cmd);
    });

    // face_to(robotId, team, point)
    lua.set_function("face_to", [this](int robotId, int team, sol::table point) {
        if (!m_world || !m_radio) {
            std::cerr << "Error: LuaInterface, World, or Radio instance is null!" << std::endl;
            return;
        }
        double x = point["x"];
        double y = point["y"];
        RobotState robotState = m_world->getRobotState(robotId, team);
        if (!robotState.isActive()) {
            std::cerr << "[Lua] Error: Robot " << robotId << " is inactive or not found!" << std::endl;
            return;
        }
        static Motion motion;
        MotionCommand cmd = motion.face_to(robotState, QVector2D(x, y));
        m_radio->addMotionCommand(cmd);
    });

    // get_robot_state(robotId, team) returns a table with robot data
    lua.set_function("get_robot_state", [this](int robotId, int team) -> sol::table {
        if (!m_world) {
            std::cerr << "Error: LuaInterface or World instance is null!" << std::endl;
            return lua.create_table();
        }
        RobotState robotState = m_world->getRobotState(robotId, team);
        sol::table tbl = lua.create_table();
        tbl["id"]          = robotState.getId();
        tbl["team"]        = robotState.getTeam();
        tbl["x"]           = robotState.getPosition().x();
        tbl["y"]           = robotState.getPosition().y();
        tbl["vel_x"]       = robotState.getVelocity().x();
        tbl["vel_y"]       = robotState.getVelocity().y();
        tbl["orientation"] = robotState.getOrientation();
        tbl["active"]      = robotState.isActive();
        return tbl;
    });

    // get_ball_state() returns a table with ball data
    lua.set_function("get_ball_state", [this]() -> sol::table {
        if (!m_world) {
            std::cerr << "Error: LuaInterface or World instance is null!" << std::endl;
            return lua.create_table();
        }
        BallState ballState = m_world->getBallState();
        sol::table tbl = lua.create_table();
        tbl["x"] = ballState.getPosition().x();
        tbl["y"] = ballState.getPosition().y();
        return tbl;
    });

    // kickx(robotId, team)
    lua.set_function("kickx", [this](int robotId, int team) {
        if (!m_radio) {
            std::cerr << "[Lua] Error: Radio is null!" << std::endl;
            return;
        }
        KickerCommand command(robotId, team);
        command.setKickX(true);
        m_radio->addKickerCommand(command);
    });

    // kickz(robotId, team)
    lua.set_function("kickz", [this](int robotId, int team) {
        if (!m_radio) {
            std::cerr << "[Lua] Error: Radio is null!" << std::endl;
            return;
        }
        KickerCommand command(robotId, team);
        command.setKickZ(true);
        m_radio->addKickerCommand(command);
    });

    // dribbler(robotId, team, speed)
    lua.set_function("dribbler", [this](int robotId, int team, double speed) {
        if (!m_radio) {
            std::cerr << "[Lua] Error: Radio is null!" << std::endl;
            return;
        }
        if (speed < 0)
            speed = 0;
        if (speed > 10)
            speed = 10;
        KickerCommand command(robotId, team);
        command.setDribbler(speed);
        m_radio->addKickerCommand(command);
    });
}

void LuaInterface::runScript(const std::string& scriptFile) {
    // Reinitialize the Lua state to reset the environment
    lua = sol::state();
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os, sol::lib::string);

    // Override print() again
    lua.set_function("print", [](sol::variadic_args args) {
        std::string output;
        for (auto v : args) {
            if (v.is<std::string>()) {
                output += v.as<std::string>();
            } else {
                output += "[non-string value]";
            }
            output += " ";
        }
        std::cout << "[Lua] " << output << std::endl;
    });

    // Adjust package.path to include your script directory
    std::string scriptDir = "C:/CondorSSL/src/luainterface/script.lua";
    std::string luaPathCmd = "package.path = '" + scriptDir + "?.lua;' .. package.path";
    lua.script(luaPathCmd);

    // Re-register functions
    register_functions();

    std::cout << "[Lua] Loading script: " << scriptFile << std::endl;

    try {
        lua.script_file(scriptFile);
        std::cout << "[Lua] Script loaded successfully!" << std::endl;
        m_haveScript = true;
    }
    catch (const sol::error &e) {
        std::cerr << "[Lua] Error loading script: " << e.what() << std::endl;
        m_haveScript = false;
    }
}

void LuaInterface::callProcess() {
    sol::function process = lua["process"];
    if (!process.valid()) {
        std::cerr << "[Lua] Error: process() is not defined in script!" << std::endl;
        return;
    }
    try {
        process();
    }
    catch (const sol::error &e) {
        std::cerr << "[Lua] Runtime error: " << e.what() << std::endl;
    }
}

bool LuaInterface::haveScript() {
    return m_haveScript;
}

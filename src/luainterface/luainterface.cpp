#include "luainterface.hpp"
#include "motion.hpp"
#include <iostream>
#include <sstream>
#include <QDir>
#include <QCoreApplication>

LuaInterface::LuaInterface(Radio* radio, World* world)
    : m_radio(radio), m_world(world)
{
    m_lua = sol::state();
    // Open required m_lua libraries
    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os, sol::lib::string);
    
    // Override m_lua's print() function with a custom lambda
    m_lua.set_function("print", [](sol::variadic_args args) {
        std::string output;
        for (auto v : args) {
            if (v.is<std::string>()) {
                output += v.as<std::string>();
            } else {
                output += "[non-string value]";
            }
            output += " ";
        }
        std::cout << "[m_lua] " << output << std::endl;
    });

    // Register your game-specific functions
    register_functions();
}

LuaInterface::~LuaInterface() {
    // No manual state cleanup is needed; sol::state cleans up automatically.
}

void LuaInterface::register_functions() {
    // move_to(robotId, team, point)
    m_lua.set_function("move_to", [this](int robotId, int team, sol::table point) {
        if (!m_world || !m_radio) {
            std::cerr << "Error: LuaInterface, World, or Radio instance is null!" << std::endl;
            return;
        }
        double x = point["x"];
        double y = point["y"];
        RobotState robotState = m_world->getRobotState(robotId, team);
        if (!robotState.isActive()) {
            std::cerr << "[m_lua] Error: Robot " << robotId << " is inactive or not found!" << std::endl;
            return;
        }
        static Motion motion;
        MotionCommand cmd = motion.to_point(robotState, QVector2D(x, y));
        m_radio->addMotionCommand(cmd);
    });

    // face_to(robotId, team, point)
    m_lua.set_function("face_to", [this](int robotId, int team, sol::table point) {
        if (!m_world || !m_radio) {
            std::cerr << "Error: LuaInterface, World, or Radio instance is null!" << std::endl;
            return;
        }
        double x = point["x"];
        double y = point["y"];
        RobotState robotState = m_world->getRobotState(robotId, team);
        if (!robotState.isActive()) {
            std::cerr << "[m_lua] Error: Robot " << robotId << " is inactive or not found!" << std::endl;
            return;
        }
        static Motion motion;
        MotionCommand cmd = motion.face_to(robotState, QVector2D(x, y));
        m_radio->addMotionCommand(cmd);
    });

    // get_robot_state(robotId, team) returns a table with robot data
    m_lua.set_function("get_robot_state", [this](int robotId, int team) -> sol::table {
        if (!m_world) {
            std::cerr << "Error: LuaInterface or World instance is null!" << std::endl;
            return m_lua.create_table();
        }
        RobotState robotState = m_world->getRobotState(robotId, team);
        sol::table tbl = m_lua.create_table();
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
    m_lua.set_function("get_ball_state", [this]() -> sol::table {
        if (!m_world) {
            std::cerr << "Error: LuaInterface or World instance is null!" << std::endl;
            return m_lua.create_table();
        }
        BallState ballState = m_world->getBallState();
        sol::table tbl = m_lua.create_table();
        tbl["x"] = ballState.getPosition().x();
        tbl["y"] = ballState.getPosition().y();
        return tbl;
    });

    // kickx(robotId, team)
    m_lua.set_function("kickx", [this](int robotId, int team) {
        if (!m_radio) {
            std::cerr << "[m_lua] Error: Radio is null!" << std::endl;
            return;
        }
        KickerCommand command(robotId, team);
        command.setKickX(true);
        m_radio->addKickerCommand(command);
    });

    // kickz(robotId, team)
    m_lua.set_function("kickz", [this](int robotId, int team) {
        if (!m_radio) {
            std::cerr << "[m_lua] Error: Radio is null!" << std::endl;
            return;
        }
        KickerCommand command(robotId, team);
        command.setKickZ(true);
        m_radio->addKickerCommand(command);
    });

    // dribbler(robotId, team, speed)
    m_lua.set_function("dribbler", [this](int robotId, int team, double speed) {
        if (!m_radio) {
            std::cerr << "[m_lua] Error: Radio is null!" << std::endl;
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

void LuaInterface::runScript() {
    m_runScript = true;
    // Reinitialize the m_lua state to reset the environment
    m_lua = sol::state();
    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os, sol::lib::string);

    // Override print() again
    m_lua.set_function("print", [](sol::variadic_args args) {
        std::string output;
        for (auto v : args) {
            if (v.is<std::string>()) {
                output += v.as<std::string>();
            } else {
                output += "[non-string value]";
            }
            output += " ";
        }
        std::cout << "[m_lua] " << output << std::endl;
    });

    // 💡 Build relative path
    QString scriptPath = QCoreApplication::applicationDirPath() +
                         "/../scripts/script.lua";  // adjust if needed

    QString normalizedPath = QDir(scriptPath).absolutePath();
    std::string fullScriptPath = normalizedPath.toStdString();

    // Optional: make Lua's package.path include script folder
    QString scriptDir = QFileInfo(scriptPath).absolutePath();
    std::string luaPathCmd = "package.path = '" +
                             scriptDir.replace("\\", "/").toStdString() + "/?.lua;' .. package.path";
    m_lua.script(luaPathCmd);

    // Re-register functions
    register_functions();

    std::cout << "[m_lua] Loading script: " << fullScriptPath  << std::endl;

    try {
        m_lua.script_file(fullScriptPath );
        std::cout << "[m_lua] Script loaded successfully!" << std::endl;
        m_haveScript = true;
    }
    catch (const sol::error &e) {
        std::cerr << "[m_lua] Error loading script: " << e.what() << std::endl;
        m_haveScript = false;
    }
}

void LuaInterface::callProcess() {
    if (m_runScript){
        sol::function process = m_lua["process"];
        if (!process.valid()) {
            std::cerr << "[m_lua] Error: process() is not defined in script!" << std::endl;
            return;
        }
        try {
            process();
        }
        catch (const sol::error &e) {
            std::cerr << "[m_lua] Runtime error: " << e.what() << std::endl;
        }
    }
}

bool LuaInterface::haveScript() {
    return m_haveScript;
}

void LuaInterface::stopScript() {
    m_runScript = false;
}

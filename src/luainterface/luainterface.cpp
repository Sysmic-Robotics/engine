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
    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os, sol::lib::string);
    register_functions();
}

LuaInterface::~LuaInterface() {}

void LuaInterface::register_functions() {
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
        MotionCommand cmd = motion.to_point(robotState, QVector2D(x, y), m_world);
        m_radio->addMotionCommand(cmd);
    });

    m_lua.set_function("face_to", [this](int robotId, int team, sol::table point, sol::optional<double> kp, sol::optional<double> ki, sol::optional<double> kd) {
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
        double Kp = kp.value_or(1.0);
        double Ki = ki.value_or(1.0);
        double Kd = kd.value_or(0.1);
        static Motion motion;
        MotionCommand cmd = motion.face_to(robotState, QVector2D(x, y), Kp, Ki, Kd);
        m_radio->addMotionCommand(cmd);
    });

    m_lua.set_function("get_robot_state", [this](int robotId, int team) -> sol::table {
        if (!m_world) {
            std::cerr << "Error: LuaInterface or World instance is null!" << std::endl;
            return m_lua.create_table();
        }
        RobotState robotState = m_world->getRobotState(robotId, team);
        sol::table tbl = m_lua.create_table();
        tbl["id"] = robotState.getId();
        tbl["team"] = robotState.getTeam();
        tbl["x"] = robotState.getPosition().x();
        tbl["y"] = robotState.getPosition().y();
        tbl["vel_x"] = robotState.getVelocity().x();
        tbl["vel_y"] = robotState.getVelocity().y();
        tbl["orientation"] = robotState.getOrientation();
        tbl["active"] = robotState.isActive();
        return tbl;
    });

    m_lua.set_function("get_ball_state", [this]() -> sol::table {
        if (!m_world) {
            std::cerr << "Error: LuaInterface or World instance is null!" << std::endl;
            return m_lua.create_table();
        }
        BallState ballState = m_world->getBallState();
        sol::table tbl = m_lua.create_table();
        tbl["x"] = ballState.getPosition().x();
        tbl["y"] = ballState.getPosition().y();
        tbl["vel_x"] = ballState.getVelocity().x();
        tbl["vel_y"] = ballState.getVelocity().y();
        return tbl;
    });

    m_lua.set_function("kickx", [this](int robotId, int team) {
        if (!m_radio) {
            std::cerr << "[m_lua] Error: Radio is null!" << std::endl;
            return;
        }
        KickerCommand command(robotId, team);
        command.setKickX(true);
        m_radio->addKickerCommand(command);
    });

    m_lua.set_function("kickz", [this](int robotId, int team) {
        if (!m_radio) {
            std::cerr << "[m_lua] Error: Radio is null!" << std::endl;
            return;
        }
        KickerCommand command(robotId, team);
        command.setKickZ(true);
        m_radio->addKickerCommand(command);
    });

    m_lua.set_function("dribbler", [this](int robotId, int team, double speed) {
        if (!m_radio) {
            std::cerr << "[m_lua] Error: Radio is null!" << std::endl;
            return;
        }
        if (speed < 0) speed = 0;
        if (speed > 10) speed = 10;
        KickerCommand command(robotId, team);
        command.setDribbler(speed);
        m_radio->addKickerCommand(command);
    });

    // Tabla grsim con funciones de teletransporte
    sol::table grsim = m_lua.create_table();

    grsim.set_function("teleport_robot", [this](int id, int team, double x, double y, double dir) {
        if (!m_radio) {
            std::cerr << "[m_lua] Error: Radio is null!" << std::endl;
            return;
        }
        m_radio->teleportRobot(id, team, x, y, dir);
    });

    grsim.set_function("teleport_ball", [this](double x, double y) {
        if (!m_radio) {
            std::cerr << "[m_lua] Error: Radio is null!" << std::endl;
            return;
        }
        m_radio->teleportBall(x, y);
    });

    m_lua["grsim"] = grsim;
}

void LuaInterface::runScript(const QString& scriptPath) {
    m_runScript = true;
    m_lua = sol::state();
    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::os, sol::lib::string);

    QString normalizedPath = QDir(scriptPath).absolutePath();
    std::string fullScriptPath = scriptPath.toStdString();

    QString scriptDir = QFileInfo(scriptPath).absolutePath();
    std::string luaPathCmd = "package.path = '" + scriptDir.replace("\\", "/").toStdString() + "/?.lua;' .. package.path";
    m_lua.script(luaPathCmd);

    register_functions();

    std::cout << "[m_lua] Loading script: " << fullScriptPath << std::endl;

    try {
        m_lua.script_file(fullScriptPath);
        std::cout << "[m_lua] Script loaded successfully!" << std::endl;
        m_haveScript = true;
    }
    catch (const sol::error &e) {
        std::cerr << "[m_lua] Error loading script: " << e.what() << std::endl;
        m_haveScript = false;
    }
}

void LuaInterface::callProcess() {
    if (m_runScript) {
        sol::protected_function process = m_lua["process"];
        if (!process.valid()) {
            std::cerr << "[m_lua] Error: process() is not defined in script!" << std::endl;
            return;
        }

        sol::protected_function_result result = process();
        if (!result.valid()) {
            m_runScript = false;
            sol::error err = result;
            std::cerr << "[m_lua] Runtime error in process(): " << err.what() << std::endl;
        }
    }
}

bool LuaInterface::haveScript() {
    return m_haveScript;
}

void LuaInterface::stopScript() {
    m_runScript = false;
}

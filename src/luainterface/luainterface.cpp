#include "luainterface.hpp"
#include "motion.hpp"
#include <iostream>
#include <sstream>
#include <QDir>
#include <QCoreApplication>

LuaInterface::LuaInterface(Radio* radio, World* world, GameState* game_state)
    : m_radio(radio), m_world(world), m_gameState(game_state)
{
    m_lua = sol::state();
    m_lua.open_libraries(
    sol::lib::base,
    sol::lib::debug,
    sol::lib::package,
    sol::lib::io,       // needed for file reading
    sol::lib::string,
    sol::lib::math,
    sol::lib::table,
    sol::lib::os
    );


    // Register your game-specific functions
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
            std::cerr << "[Lua] Error: Robot " << robotId << " is inactive or not found!" << std::endl;
            return;
        }
        static Motion motion;
        MotionCommand cmd = motion.move_to(robotState, QVector2D(x, y), m_world);
        m_radio->addMotionCommand(cmd);
    });

        m_lua.set_function("get_ref_message", [this]() -> std::string {
        if (!m_gameState) {
            std::cerr << "Error: GameState instance is null!" << std::endl;
            return "ERROR: GameState is null";
        }
        return m_gameState->GetRefMessage().toStdString();
    });


    m_lua.set_function("get_yellow_team_state", [this]() -> sol::table {
    sol::table result = m_lua.create_table();
    if (!m_world) {
        std::cerr << "Error: World instance is null!" << std::endl;
        return result;
    }

    QList<RobotState> team = m_world->getYellowTeamState();
    for (int i = 0; i < team.size(); ++i) {
        const RobotState& robot = team[i];
        sol::table robotTable = m_lua.create_table();
        robotTable["id"] = robot.getId();
        robotTable["team"] = 1;
        robotTable["x"] = robot.getPosition().x();
        robotTable["y"] = robot.getPosition().y();
        robotTable["vel_x"] = robot.getVelocity().x();
        robotTable["vel_y"] = robot.getVelocity().y();
        robotTable["orientation"] = robot.getOrientation();
        robotTable["omega"] = robot.getAngularVelocity();
        robotTable["active"] = robot.isActive();
        result[i + 1] = robotTable;  // Lua tables are 1-based
    }

    return result;
    });

    m_lua.set_function("get_blue_team_state", [this]() -> sol::table {
    sol::table result = m_lua.create_table();
    if (!m_world) {
        std::cerr << "Error: World instance is null!" << std::endl;
        return result;
    }

    QList<RobotState> team = m_world->getBlueTeamState();
    for (int i = 0; i < team.size(); ++i) {
        const RobotState& robot = team[i];
        sol::table robotTable = m_lua.create_table();
        robotTable["id"] = robot.getId();
        robotTable["team"] = 0;
        robotTable["x"] = robot.getPosition().x();
        robotTable["y"] = robot.getPosition().y();
        robotTable["vel_x"] = robot.getVelocity().x();
        robotTable["vel_y"] = robot.getVelocity().y();
        robotTable["orientation"] = robot.getOrientation();
        robotTable["omega"] = robot.getAngularVelocity();
        robotTable["active"] = robot.isActive();
        result[i + 1] = robotTable;
    }

    return result;
});

    m_lua.set_function("move_direct", [this](int robotId, int team, sol::table point) {
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
        MotionCommand cmd = motion.move_direct(robotState, QVector2D(x, y));
        m_radio->addMotionCommand(cmd);
    });

    m_lua.set_function("motion", [this](int robotId, int team, sol::table point,
                                        sol::optional<double> kp_x,
                                        sol::optional<double> ki_x,
                                        sol::optional<double> kp_y,
                                        sol::optional<double> ki_y) {
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
        MotionCommand cmd = motion.motion(
            robotState,
            QVector2D(x, y),
            m_world,
            kp_x.value_or(0.5),
            ki_x.value_or(0.1),
            kp_y.value_or(0.5),
            ki_y.value_or(0.1)
        );

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
        std::cerr << "[Lua] Error: Robot " << robotId << " is inactive or not found!" << std::endl;
        return;
    }

    // Set default PID parameters if not provided
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

    m_lua.set_function("send_velocity", [this](int robotId, int team, double vx, double vy, double omega) {
        if (!m_world || !m_radio) {
            std::cerr << "Error: LuaInterface, World, or Radio instance is null!" << std::endl;
            return;
        }

        RobotState robotState = m_world->getRobotState(robotId, team);
        if (!robotState.isActive()) {
            std::cerr << "[m_lua] Error: Robot " << robotId << " is inactive or not found!" << std::endl;
            return;
        }

        MotionCommand cmd(robotId, team, vx,vy);
        cmd.setAngular(omega);

        m_radio->addMotionCommand(cmd);
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
            std::cerr << "[Lua] Error: Radio is null!" << std::endl;
            return;
        }
        KickerCommand command(robotId, team);
        command.setKickX(true);
        m_radio->addKickerCommand(command);
    });

    m_lua.set_function("kickz", [this](int robotId, int team) {
        if (!m_radio) {
            std::cerr << "[Lua] Error: Radio is null!" << std::endl;
            return;
        }
        KickerCommand command(robotId, team);
        command.setKickZ(true);
        m_radio->addKickerCommand(command);
    });

    m_lua.set_function("dribbler", [this](int robotId, int team, double speed) {
        if (!m_radio) {
            std::cerr << "[Lua] Error: Radio is null!" << std::endl;
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
    m_isPaused = false;
    m_lua = sol::state();
    m_lua.open_libraries(
        sol::lib::base,
        sol::lib::debug,
        sol::lib::package,
        sol::lib::io,       // needed for file reading
        sol::lib::string,
        sol::lib::math,
        sol::lib::table,
        sol::lib::os
    );
    // Normalize script path
    QString normalizedPath = QDir(scriptPath).absolutePath();
    std::string fullScriptPath = scriptPath.toStdString();

    QString scriptDir = QFileInfo(scriptPath).absolutePath();
    std::string luaPathCmd = "package.path = '" + scriptDir.replace("\\", "/").toStdString() + "/?.lua;' .. package.path";
    m_lua.script(luaPathCmd);

    register_functions();

    std::cout << "[Lua] Loading script: " << fullScriptPath << std::endl;

    try {
        m_lua.script_file(fullScriptPath);
        std::cout << "[Lua] Script loaded successfully!" << std::endl;
        m_haveScript = true;
    }
    catch (const sol::error &e) {
        std::cerr << "[Lua] Error loading script: " << e.what() << std::endl;
        m_haveScript = false;
    }
}

void LuaInterface::callProcess() {
    if(m_haveScript){
        if (!m_isPaused) {
            sol::protected_function process = m_lua["process"];
            if (!process.valid()) {
                std::cerr << "[Lua] Error: process() is not defined in script!" << std::endl;
                return;
            }

            sol::protected_function_result result = process();
            if (!result.valid()) {
                m_isPaused = false;
                sol::error err = result;
                std::cerr << "[Lua] Runtime error in process(): " << err.what() << std::endl;
            }
        }else{

            for(int team = 0; team < 2; team++){
                for(int id = 0; id < 6; id++){
                    MotionCommand cmd(id, team, 0, 0);
                    cmd.setAngular(0);
                    m_radio->addMotionCommand(cmd);
                }
            }
        }
    }
}

void LuaInterface::pauseScript() {
    m_isPaused = true;
    MotionCommand cmd(0, 0, 0,0);
    cmd.setAngular(0);
    m_radio->addMotionCommand(cmd);
    qDebug() << "[Lua] Script stopped.";
}


void LuaInterface::resumeScript() {
    m_isPaused = false;
}

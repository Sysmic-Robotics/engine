#include "luabindings.hpp"
#include <iostream>

// ---- Include your project headers here ----
// #include "World.hpp"
// #include "Radio.hpp"
// #include "GameState.hpp"
// #include "Motion.hpp"
// #include "MotionCommand.hpp"
// #include "KickerCommand.hpp"
// #include "RobotState.hpp"
// #include "BallState.hpp"
// #include <QVector2D>

// ------------------ ctor ------------------
LuaBindings::LuaBindings(LuaDeps deps) : d_(deps) {
    if (!d_.log) {
        d_.log = [](const std::string& s) { std::cerr << s << std::endl; };
    }
}

// ----------------- helpers ----------------
void LuaBindings::logError(const std::string& msg) const {
    d_.log(msg);
}

bool LuaBindings::checkWorld() const {
    if (!d_.world) {
        logError("Error: World instance is null!");
        return false;
    }
    return true;
}

bool LuaBindings::checkRadio() const {
    if (!d_.radio) {
        logError("Error: Radio instance is null!");
        return false;
    }
    return true;
}

bool LuaBindings::checkGame() const {
    if (!d_.game) {
        logError("Error: GameState instance is null!");
        return false;
    }
    return true;
}

sol::table LuaBindings::makeRobotTable(const RobotState& r) const {
    sol::table t = d_.lua.create_table();
    t["id"]          = r.getId();
    t["team"]        = r.getTeam();
    t["x"]           = r.getPosition().x();
    t["y"]           = r.getPosition().y();
    t["vel_x"]       = r.getVelocity().x();
    t["vel_y"]       = r.getVelocity().y();
    t["orientation"] = r.getOrientation();
    t["omega"]       = r.getAngularVelocity();
    t["active"]      = r.isActive();
    return t;
}

sol::table LuaBindings::makeBallTable(const BallState& b) const {
    sol::table t = d_.lua.create_table();
    t["x"]     = b.getPosition().x();
    t["y"]     = b.getPosition().y();
    t["vel_x"] = b.getVelocity().x();
    t["vel_y"] = b.getVelocity().y();
    return t;
}

// --------------- registrations -------------
void LuaBindings::registerAll() {
    auto& m_lua = d_.lua;

    // move_to(robotId, team, {x=, y=})
    m_lua.set_function("move_to", [this](int robotId, int team, sol::table point) {
        if (!checkWorld() || !checkRadio()) return;

        double x = point["x"];
        double y = point["y"];

        RobotState robotState = d_.world->getRobotState(robotId, team);
        if (!robotState.isActive()) {
            logError("[Lua] Error: Robot " + std::to_string(robotId) + " is inactive or not found!");
            return;
        }

        static Motion motion;
        MotionCommand cmd = motion.move_to(robotState, QVector2D(x, y), d_.world);
        d_.radio->addMotionCommand(cmd);
    });

    // get_ref_message()
    m_lua.set_function("get_ref_message", [this]() -> std::string {
        if (!checkGame()) {
            return "ERROR: GameState is null";
        }
        return d_.game->GetRefMessage().toStdString();
    });

    // get_yellow_team_state()
    m_lua.set_function("get_yellow_team_state", [this]() -> sol::table {
        sol::table result = d_.lua.create_table();
        if (!checkWorld()) return result;

        QList<RobotState> team = d_.world->getYellowTeamState();
        for (int i = 0; i < team.size(); ++i) {
            result[i + 1] = makeRobotTable(team[i]); // Lua 1-based
            result[i + 1]["team"] = 1;
        }
        return result;
    });

    // get_blue_team_state()
    m_lua.set_function("get_blue_team_state", [this]() -> sol::table {
        sol::table result = d_.lua.create_table();
        if (!checkWorld()) return result;

        QList<RobotState> team = d_.world->getBlueTeamState();
        for (int i = 0; i < team.size(); ++i) {
            result[i + 1] = makeRobotTable(team[i]);
            result[i + 1]["team"] = 0;
        }
        return result;
    });

    // move_direct(robotId, team, {x=, y=})
    m_lua.set_function("move_direct", [this](int robotId, int team, sol::table point) {
        if (!checkWorld() || !checkRadio()) return;

        double x = point["x"];
        double y = point["y"];

        RobotState robotState = d_.world->getRobotState(robotId, team);
        if (!robotState.isActive()) {
            logError("[Lua] Error: Robot " + std::to_string(robotId) + " is inactive or not found!");
            return;
        }

        static Motion motion;
        MotionCommand cmd = motion.move_direct(robotState, QVector2D(x, y));
        d_.radio->addMotionCommand(cmd);
    });

    // motion(robotId, team, {x=, y=}, kp_x?, ki_x?, kp_y?, ki_y?)
    m_lua.set_function("motion",
        [this](int robotId, int team, sol::table point,
               sol::optional<double> kp_x,
               sol::optional<double> ki_x,
               sol::optional<double> kp_y,
               sol::optional<double> ki_y) {
            if (!checkWorld() || !checkRadio()) return;

            double x = point["x"];
            double y = point["y"];

            RobotState robotState = d_.world->getRobotState(robotId, team);
            if (!robotState.isActive()) {
                logError("[Lua] Error: Robot " + std::to_string(robotId) + " is inactive or not found!");
                return;
            }

            static Motion motion;
            MotionCommand cmd = motion.motion(
                robotState,
                QVector2D(x, y),
                d_.world,
                kp_x.value_or(0.5),
                ki_x.value_or(0.1),
                kp_y.value_or(0.5),
                ki_y.value_or(0.1)
            );

            d_.radio->addMotionCommand(cmd);
        });

    // face_to(robotId, team, {x=, y=}, kp?, ki?, kd?)
    m_lua.set_function("face_to",
        [this](int robotId, int team, sol::table point,
               sol::optional<double> kp,
               sol::optional<double> ki,
               sol::optional<double> kd) {
            if (!checkWorld() || !checkRadio()) return;

            double x = point["x"];
            double y = point["y"];

            RobotState robotState = d_.world->getRobotState(robotId, team);
            if (!robotState.isActive()) {
                logError("[Lua] Error: Robot " + std::to_string(robotId) + " is inactive or not found!");
                return;
            }

            double Kp = kp.value_or(1.0);
            double Ki = ki.value_or(1.0);
            double Kd = kd.value_or(0.1);

            static Motion motion;
            MotionCommand cmd = motion.face_to(robotState, QVector2D(x, y), Kp, Ki, Kd);
            d_.radio->addMotionCommand(cmd);
        });

    // get_robot_state(robotId, team)
    m_lua.set_function("get_robot_state", [this](int robotId, int team) -> sol::table {
        if (!checkWorld()) return d_.lua.create_table();

        RobotState robotState = d_.world->getRobotState(robotId, team);
        return makeRobotTable(robotState);
    });

    // send_velocity(robotId, team, vx, vy, omega)
    m_lua.set_function("send_velocity", [this](int robotId, int team, double vx, double vy, double omega) {
        if (!checkWorld() || !checkRadio()) return;

        RobotState robotState = d_.world->getRobotState(robotId, team);
        if (!robotState.isActive()) {
            logError("[Lua] Error: Robot " + std::to_string(robotId) + " is inactive or not found!");
            return;
        }

        MotionCommand cmd(robotId, team, vx, vy);
        cmd.setAngular(omega);
        d_.radio->addMotionCommand(cmd);
    });

    // get_ball_state()
    m_lua.set_function("get_ball_state", [this]() -> sol::table {
        if (!checkWorld()) return d_.lua.create_table();
        BallState ballState = d_.world->getBallState();
        return makeBallTable(ballState);
    });

    // kickx(robotId, team)
    m_lua.set_function("kickx", [this](int robotId, int team) {
        if (!checkRadio()) return;
        KickerCommand command(robotId, team);
        command.setKickX(true);
        d_.radio->addKickerCommand(command);
    });

    // kickz(robotId, team)
    m_lua.set_function("kickz", [this](int robotId, int team) {
        if (!checkRadio()) return;
        KickerCommand command(robotId, team);
        command.setKickZ(true);
        d_.radio->addKickerCommand(command);
    });

    // dribbler(robotId, team, speed[0..10])
    m_lua.set_function("dribbler", [this](int robotId, int team, double speed) {
        if (!checkRadio()) return;
        if (speed < 0)  speed = 0;
        if (speed > 10) speed = 10;
        KickerCommand command(robotId, team);
        command.setDribbler(speed);
        d_.radio->addKickerCommand(command);
    });

    // grsim.{teleport_robot, teleport_ball}
    sol::table grsim = m_lua.create_table();

    grsim.set_function("teleport_robot", [this](int id, int team, double x, double y, double dir) {
        if (!checkRadio()) return;
        d_.radio->teleportRobot(id, team, x, y, dir);
    });

    grsim.set_function("teleport_ball", [this](double x, double y) {
        if (!checkRadio()) return;
        d_.radio->teleportBall(x, y);
    });

    m_lua["grsim"] = grsim;
}

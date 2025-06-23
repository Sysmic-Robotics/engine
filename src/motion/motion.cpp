#include "motion.hpp"
#include "pid.hpp"
#include "path_planner.hpp"
#include "pi_vel/pi_vel.hpp"

// CHANGE THE NAME TO move_to_point
MotionCommand Motion::to_point(const RobotState& robotState, QVector2D targetPoint, const World* world) {
    static BangBangControl bangbangControl(2.5f, 5.0f); // Acceleration & velocity limits
    static FastPathPlanner planner;

    QVector2D from = robotState.getPosition();
    QVector2D to = targetPoint;
    int selfId = robotState.getId();
    int selfTeam = robotState.getTeam();

    std::vector<QVector2D> otherRobots;
    for (int id = 0; id < 12; ++id) {
        if (id == selfId) continue;
        RobotState rBlue = world->getRobotState(id, 0);
        if (rBlue.isActive()) otherRobots.push_back(rBlue.getPosition());
        RobotState rYellow = world->getRobotState(id, 1);
        if (rYellow.isActive()) otherRobots.push_back(rYellow.getPosition());
    }

    std::vector<QVector2D> pathVec = planner.getPath(from, to, otherRobots);

    QList<QVector2D> path;
    if (!pathVec.empty()) {
        for (const QVector2D& p : pathVec)
            path.append(p);
    } else {
        path = { from, to }; // fallback if path is invalid
    }

    double delta = 1.0 / 60.0; // Frame delta time
    // Calcular velocidades de referencia
    MotionCommand cmd = bangbangControl.computeMotion(robotState, path, delta);

    return cmd;
}

MotionCommand Motion::motion(const RobotState& robotState, QVector2D targetPoint, const World* world,
    double Kp_vel, double Ki_vel) {
    static BangBangControl bangbangControl(2.5f, 5.0f); // Acceleration & velocity limits
    static FastPathPlanner planner;

    QVector2D from = robotState.getPosition();
    QVector2D to = targetPoint;
    int selfId = robotState.getId();
    int selfTeam = robotState.getTeam();

    std::vector<QVector2D> otherRobots;
    for (int id = 0; id < 12; ++id) {
        if (id == selfId) continue;
        RobotState rBlue = world->getRobotState(id, 0);
        if (rBlue.isActive()) otherRobots.push_back(rBlue.getPosition());
        RobotState rYellow = world->getRobotState(id, 1);
        if (rYellow.isActive()) otherRobots.push_back(rYellow.getPosition());
    }

    std::vector<QVector2D> pathVec = planner.getPath(from, to, otherRobots);

    QList<QVector2D> path;
    if (!pathVec.empty()) {
        for (const QVector2D& p : pathVec)
            path.append(p);
    } else {
        path = { from, to }; // fallback if path is invalid
    }

    double delta = 1.0 / 60.0; // Frame delta time
    // Calcular velocidades de referencia
    MotionCommand cmd = bangbangControl.computeMotion(robotState, path, delta);

    QVector2D knownDrift(0.0, 0.0); // This should be set based on your robot's known drift

    // error de velocidades
    QVector2D errorVelocity(
        cmd.getVx() - robotState.getVelocity().x() - knownDrift.x(),
        cmd.getVy() - robotState.getVelocity().y() - knownDrift.y()
    );

    static PI_VEL piVelControl(Kp_vel, Ki_vel); // Example gains, adjust as needed
    QVector2D pi_velVector(
        piVelControl.compute(errorVelocity.x(), delta),
        piVelControl.compute(errorVelocity.y(), delta)
    );

    // Agregar vector de correccion a priori ya conocido
    cmd.setVx(cmd.getVx() + pi_velVector.x() + knownDrift.x());
    cmd.setVy(cmd.getVy() + pi_velVector.y() + knownDrift.y());

    return cmd;
}

MotionCommand Motion::face_to(const RobotState& robotState, QVector2D targetPoint,
    double Kp, double Ki, double Kd) {
    // Create a PID controller with provided or default parameters
    PID pidControl(Kp, Ki, Kd);

    // Helper lambda to normalize angles to [-pi, pi]
    auto normalizeAngle = [](double angle) {
    while (angle > M_PI) angle -= 2 * M_PI;
    while (angle < -M_PI) angle += 2 * M_PI;
    return angle;
    };

    // Normalize angles
    double currentAngle = normalizeAngle(robotState.getOrientation());
    double targetAngle = normalizeAngle(std::atan2(
    targetPoint.y() - robotState.getPosition().y(),
    targetPoint.x() - robotState.getPosition().x()
    ));

    // Compute error
    double error = normalizeAngle(targetAngle - currentAngle);

    // Time step for PID computation (e.g., 60Hz update rate)
    double delta = 1.0 / 60.0;
    double angularVelocity = pidControl.compute(error, delta);

    // Create and return the motion command
    MotionCommand cmd(robotState.getId(), robotState.getTeam());
    cmd.setAngular(angularVelocity);
    return cmd;
}



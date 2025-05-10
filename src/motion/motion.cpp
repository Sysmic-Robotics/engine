#include "motion.hpp"
#include "pid.hpp"
#include "path_planner.hpp"
#include "../world/world.hpp"

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
    MotionCommand cmd = bangbangControl.computeMotion(robotState, path, delta);
    return cmd;
}

MotionCommand Motion::face_to(const RobotState& robotState, QVector2D targetPoint) {
    static PID pidControl(1.0, 0.01, 0.1); // Kp, Ki, Kd values

    double targetAngle = std::atan2(targetPoint.y() - robotState.getPosition().y(),
                                    targetPoint.x() - robotState.getPosition().x());

    double error = targetAngle - robotState.getOrientation();
    while (error > M_PI) error -= 2 * M_PI;
    while (error < -M_PI) error += 2 * M_PI;

    double delta = 1.0 / 60.0;
    double angularVelocity = pidControl.compute(error, delta);

    MotionCommand cmd(robotState.getId(), robotState.getTeam());
    cmd.setAngular(angularVelocity);
    return cmd;
}

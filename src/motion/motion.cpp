#include "motion.hpp"
#include "pid.hpp"
#include "path_planner.hpp"

// Move without obstacles: Use when you have to make small movements (PivotKick, Goalie movement)
MotionCommand Motion::move_direct(const RobotState& robotState, QVector2D targetPoint) {
    static BangBangControl bangbangControl(2.5f, 5.0f); // Acceleration & velocity limits

    // Create a direct path from current position to the target
    QVector2D from = robotState.getPosition();
    QList<QVector2D> path = { from, targetPoint };
    
    double delta = 1.0 / 60.0; // Frame delta time
    MotionCommand cmd = bangbangControl.computeMotion(robotState, path, delta);
    return cmd;
}

// Move avoiding obstacles
MotionCommand Motion::move_to(const RobotState& robotState, QVector2D targetPoint, const World* world) {
    static BangBangControl bangbangControl(2.5f, 5.0f); // Acceleration & velocity limits
    static FastPathPlanner planner;

    QVector2D from = robotState.getPosition();
    QVector2D to = targetPoint;
    int selfId = robotState.getId();

    std::vector<QVector2D> otherRobots;

    for (int id = 0; id < 12; ++id) {
        if (id == selfId) continue;

        RobotState rBlue = world->getRobotState(id, 0);
        if (rBlue.isActive()) otherRobots.push_back(rBlue.getPosition());

        RobotState rYellow = world->getRobotState(id, 1);
        if (rYellow.isActive()) otherRobots.push_back(rYellow.getPosition());
    }

    // === Get ball position from the world ===
    QVector2D ballPos = world->getBallState().getPosition();

    // === Create environment ===
    Environment env(otherRobots, ballPos);

    // === Compute path ===
    std::vector<QVector2D> pathVec = planner.getPath(from, to, env);

    // === Convert to QList for the motion controller ===
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
    double Kp_x, double Ki_x, double Kp_y, double Ki_y) {
    static BangBangControl bangbangControl(2.5f, 5.0f); // Acceleration & velocity limits
    static FastPathPlanner planner;

    QVector2D from = robotState.getPosition();
    QVector2D to = targetPoint;
    int selfId = robotState.getId();

    std::vector<QVector2D> otherRobots;

    for (int id = 0; id < 12; ++id) {
        if (id == selfId) continue;

        RobotState rBlue = world->getRobotState(id, 0);
        if (rBlue.isActive()) otherRobots.push_back(rBlue.getPosition());

        RobotState rYellow = world->getRobotState(id, 1);
        if (rYellow.isActive()) otherRobots.push_back(rYellow.getPosition());
    }

    // === Get ball position from the world ===
    QVector2D ballPos = world->getBallState().getPosition();

    // === Create environment ===
    Environment env(otherRobots, ballPos);

    // === Compute path ===
    std::vector<QVector2D> pathVec = planner.getPath(from, to, env);

    // === Convert to QList for the motion controller ===
    QList<QVector2D> path;
    if (!pathVec.empty()) {
        for (const QVector2D& p : pathVec)
            path.append(p);
    } else {
        path = { from, to }; // fallback if path is invalid
    }

    double delta = 1.0 / 60.0; // Frame delta time
    // Calcular velocidades de referencia
    MotionCommand ref_vel = bangbangControl.computeMotion(robotState, path, delta);
    double orientation = robotState.getOrientation();
    QVector2D localVelocity(
        robotState.getVelocity().x() * cos(-orientation) - robotState.getVelocity().y() * sin(-orientation),
        robotState.getVelocity().x() * sin(-orientation) + robotState.getVelocity().y() * cos(-orientation)
    );
    // Vx
    static PID pidControlX(Kp_x, Ki_x, 0);
    double error_x = ref_vel.getVx() - localVelocity.x();
    double new_vx = pidControlX.compute(error_x, 0.016);

    // Vy
    static PID pidControlY(Kp_y, Ki_y, 0);
    double error_y = ref_vel.getVy() - localVelocity.y();
    double new_vy = pidControlY.compute(error_y, 0.016);

    MotionCommand cmd(robotState.getId(), robotState.getTeam(), ref_vel.getVx() + new_vx, ref_vel.getVy() + new_vy);
 


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



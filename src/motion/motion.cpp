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

    Environment env(world, robotState);

    // === Compute path ===
    QList<QVector2D> path = planner.getPath(from, to, env);

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

    Environment env(world, robotState);

    // === Compute path ===
    QList<QVector2D> path = planner.getPath(from, to, env);

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
    double new_vx = pidControlX.compute(error_x, delta);

    // Vy
    static PID pidControlY(Kp_y, Ki_y, 0);
    double error_y = ref_vel.getVy() - localVelocity.y();
    double new_vy = pidControlY.compute(error_y, delta);

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


double Motion::normalizeAngle(double angle) {
    while (angle > M_PI) angle -= 2 * M_PI;
    while (angle < -M_PI) angle += 2 * M_PI;
    return angle;
}

MotionCommand Motion::face_to_angle(const RobotState& robotState, double targetAngle,
    double Kp, double Ki) {

    PID anglePID(Kp, Ki, 0);

    double currentAngle = normalizeAngle(robotState.getOrientation());
    targetAngle = normalizeAngle(targetAngle);

    double error = normalizeAngle(targetAngle - currentAngle);

    double deltaTime = 1.0 / 60.0;
    double angularVelocity = anglePID.compute(error, deltaTime);

    MotionCommand cmd(robotState.getId(), robotState.getTeam());
    cmd.setAngular(angularVelocity);
    return cmd;
}


MotionCommand Motion::motion_with_orientation(const RobotState& robotState, QVector2D targetPoint, double targetAngle,
    const World* world,
    double Kp_x, double Ki_x,
    double Kp_y, double Ki_y,
    double Kp_angle, double Ki_angle) {

    static BangBangControl bangbangControl(2.5f, 5.0f); // Acceleration & velocity limits
    static FastPathPlanner planner;

    QVector2D from = robotState.getPosition();
    QVector2D to = targetPoint;
    Environment env(world, robotState);

    // === Compute path ===
    QList<QVector2D> path = planner.getPath(from, to, env);

    double delta = 1.0 / 60.0;

    // === Reference motion from path planner ===
    MotionCommand ref_vel = bangbangControl.computeMotion(robotState, path, delta);

    // === Transform velocity to local frame ===
    double orientation = robotState.getOrientation();
    QVector2D localVelocity(
        robotState.getVelocity().x() * std::cos(-orientation) - robotState.getVelocity().y() * std::sin(-orientation),
        robotState.getVelocity().x() * std::sin(-orientation) + robotState.getVelocity().y() * std::cos(-orientation)
    );

    // === PID Control for Vx ===
    static PID pidControlX(Kp_x, Ki_x, 0);
    double error_x = ref_vel.getVx() - localVelocity.x();
    double control_vx = pidControlX.compute(error_x, delta);

    // === PID Control for Vy ===
    static PID pidControlY(Kp_y, Ki_y, 0);
    double error_y = ref_vel.getVy() - localVelocity.y();
    double control_vy = pidControlY.compute(error_y, delta);

    // === PID Control for Angular Velocity ===
    static PID anglePID(Kp_angle, Ki_angle, 0);
    double currentAngle = normalizeAngle(robotState.getOrientation());
    targetAngle = normalizeAngle(targetAngle);
    double error_angle = normalizeAngle(targetAngle - currentAngle);
    double angularVelocity = anglePID.compute(error_angle, delta);

    // === Construct final motion command ===
    MotionCommand cmd(robotState.getId(), robotState.getTeam(),
                      ref_vel.getVx() + control_vx,
                      ref_vel.getVy() + control_vy);
    cmd.setAngular(angularVelocity);

    return cmd;
}
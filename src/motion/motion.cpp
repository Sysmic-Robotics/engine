#include "motion.hpp"
#include "pid.hpp"
#include <QDebug>

// CHANGE THE NAME TO move_to_point
MotionCommand Motion::to_point(const RobotState& robotState, QVector2D targetPoint) {
    // Define BangBangControl with acceleration & velocity limits
    static BangBangControl bangbangControl(2.5f, 5.0f); // Example values for A_MAX and V_MAX

    // Example path (replace with actual path generation)
    QList<QVector2D> path = { robotState.getPosition(), targetPoint};

    // Compute motion using BangBangControl
    double delta = 1.0 / 60.0; // Example delta time
    MotionCommand cmd = bangbangControl.computeMotion(robotState, path, delta);

    // Set angular velocity (example value, adjust as needed)
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



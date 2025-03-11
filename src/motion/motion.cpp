#include "motion.hpp"
#include "pid.hpp"
#include <QDebug>

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

MotionCommand Motion::face_to(const RobotState& robotState, QVector2D targetPoint) {
    // Define PID control with appropriate gains
    static PID pidControl(1.0, 0.01, 0.1); // Kp, Ki, Kd values (adjust as needed)

    // Compute target angle
    double targetAngle = std::atan2(targetPoint.y() - robotState.getPosition().y(),
                                    targetPoint.x() - robotState.getPosition().x());
    
    // Compute error
    double error = targetAngle - robotState.getOrientation();
    // Normalize error to range [-pi, pi]
    while (error > M_PI) error -= 2 * M_PI;
    while (error < -M_PI) error += 2 * M_PI;
    
    // Compute motion using PID control
    double delta = 1.0 / 60.0; // Example delta time
    double angularVelocity = pidControl.compute(error, delta);
    
    // Construct motion command
    MotionCommand cmd(robotState.getId(), robotState.getTeam());
    cmd.setAngular(angularVelocity);
    return cmd;
}

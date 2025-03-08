#include "motion.hpp"
#include <QDebug>

MotionCommand Motion::to_point(const RobotState& robotState) {
    // Define BangBangControl with acceleration & velocity limits
    static BangBangControl bangbangControl(2.5f, 5.0f); // Example values for A_MAX and V_MAX

    // Example path (replace with actual path generation)
    QList<QVector2D> path = { robotState.getPosition(), QVector2D(0, 0) };

    // Compute motion using BangBangControl
    double delta = 1.0 / 60.0; // Example delta time
    MotionCommand cmd = bangbangControl.computeMotion(robotState, path, delta);

    // Set angular velocity (example value, adjust as needed)
    return cmd;
}

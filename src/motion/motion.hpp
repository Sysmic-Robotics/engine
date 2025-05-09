#ifndef MOTION_HPP
#define MOTION_HPP

#include "robotstate.hpp"
#include "motioncommand.hpp"
#include "bangbangcontrol.hpp"

class Motion {
public:
    Motion() = default;

    // Computes a motion command to move the robot towards a target point
    MotionCommand to_point(const RobotState& robotState, QVector2D targetPoint);
    MotionCommand face_to(const RobotState& robotState, QVector2D targetPoint,
        double Kp = 1.0, double Ki = 1.0, double Kd = 0.1);
};

#endif // MOTION_HPP

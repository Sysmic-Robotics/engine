#ifndef MOTION_HPP
#define MOTION_HPP

#include "robotstate.hpp"
#include "motioncommand.hpp"
#include "bangbangcontrol.hpp"
#include "world.hpp"




class Motion {
public:
    Motion() = default;

    // Computes a motion command to move the robot towards a target point
    MotionCommand face_to(const RobotState& robotState, QVector2D targetPoint, double Kp, double Ki, double Kd);
    MotionCommand move_to(const RobotState& robotState, QVector2D targetPoint, const World* world);
    MotionCommand move_direct(const RobotState& robotState, QVector2D targetPoint);
    MotionCommand motion(const RobotState& robotState, QVector2D targetPoint, const World* world,
    double Kp_x, double Ki_x, double Kp_y, double Ki_y);
    MotionCommand face_to_angle(const RobotState& robotState, double targetAngle,
    double Kp, double Ki);
    double normalizeAngle(double angle);
    MotionCommand motion_with_orientation(const RobotState& robotState, QVector2D targetPoint, double targetAngle,
    const World* world,
    double Kp_x, double Ki_x,
    double Kp_y, double Ki_y,
    double Kp_angle, double Ki_angle);

};

#endif // MOTION_HPP

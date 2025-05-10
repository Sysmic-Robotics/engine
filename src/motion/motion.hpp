#ifndef MOTION_HPP
#define MOTION_HPP

#include "robotstate.hpp"
#include "motioncommand.hpp"
#include "bangbangcontrol.hpp"
#include "../world/world.hpp"




class Motion {
public:
    Motion() = default;

    // Computes a motion command to move the robot towards a target point
    MotionCommand face_to(const RobotState& robotState, QVector2D targetPoint);
    MotionCommand to_point(const RobotState& robotState, QVector2D targetPoint, const World* world);

};

#endif // MOTION_HPP

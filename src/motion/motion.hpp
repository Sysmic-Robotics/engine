#ifndef MOTION_HPP
#define MOTION_HPP

#include "robotstate.hpp"
#include "motioncommand.hpp"

class Motion {
public:
    Motion() = default;

    // Computes a motion command to move the robot towards a target point
    MotionCommand to_point();
};

#endif // MOTION_HPP

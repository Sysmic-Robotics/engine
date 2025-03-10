#ifndef STATECHECKER_HPP
#define STATECHECKER_HPP

#include <QVector2D>
#include "robotstate.hpp"

class StateChecker {
public:
    // Check if the robot is exactly at a target position within a given threshold
    static bool isAtPoint(const RobotState& robot, const QVector2D& target, double threshold = 0.08);
};

#endif // STATECHECKER_HPP

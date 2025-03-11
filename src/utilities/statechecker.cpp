#include "statechecker.hpp"

bool StateChecker::isAtPoint(const RobotState& robot, const QVector2D& target, double threshold) {
    return (robot.getPosition().distanceToPoint(target) <= threshold);
}
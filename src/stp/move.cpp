#include "move.hpp"

Move::Move(QObject *parent) : Skill(parent), completed(false), threshold(0.05) {}

MotionCommand Move::process(const RobotState &robotState, QVector2D target) {
    static Motion motion;
    targetPosition = target;  // ✅ Store the target position

    // ✅ Check if robot is close enough to the target
    double distance = (robotState.getPosition() - targetPosition).length();
    if (distance < threshold) {
        completed = true;  // ✅ Mark movement as completed
        return MotionCommand(robotState.getId(), robotState.getTeam());  // Stop the robot
    }

    completed = false;  // ✅ Keep moving if not reached
    return motion.to_point(robotState, targetPosition);
}

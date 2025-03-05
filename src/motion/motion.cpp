#include "motion.hpp"
#include <QDebug>

MotionCommand Motion::to_point(const RobotState& robotState) {
    MotionCommand cmd(robotState.getId(), robotState.getTeam());

    cmd.setAngular(1);

    return cmd;
}

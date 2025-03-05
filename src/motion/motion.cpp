#include "motion.hpp"
#include <QDebug>

MotionCommand Motion::to_point() {
    MotionCommand cmd;

    cmd.setOmega(1);

    return cmd;
}

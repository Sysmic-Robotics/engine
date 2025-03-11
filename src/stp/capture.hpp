#ifndef CAPTURE_HPP
#define CAPTURE_HPP

#include "skill.hpp"
#include "motion.hpp"
#include "move.hpp"
#include "motioncommand.hpp"
#include "ballstate.hpp"
#include <QVector2D>
#include <QDebug>

class Capture : public Skill {
    Q_OBJECT

private:
    BallState ball;  // Target point for movement
    bool completed = false;    // Flag indicating movement completion

public:
    explicit Capture(QObject *parent = nullptr);

    MotionCommand process(const RobotState &robotState, const BallState &ball);

    bool isCompleted() const { return completed; } 
};

#endif // MOVE_HPP

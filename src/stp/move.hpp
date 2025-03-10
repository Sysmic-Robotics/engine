#ifndef MOVE_HPP
#define MOVE_HPP

#include "skill.hpp"
#include "motion.hpp"
#include "motioncommand.hpp"
#include <QVector2D>
#include <QDebug>

class Move : public Skill {
    Q_OBJECT

private:
    QVector2D targetPosition;  // Target point for movement
    bool completed = false;    // Flag indicating movement completion
    double threshold = 0.05;   // ✅ Distance threshold for stopping [meters]

public:
    explicit Move(QObject *parent = nullptr);

    MotionCommand process(const RobotState &robotState, QVector2D target);  // ✅ Now only exists in Move

    bool isCompleted() const { return completed; }  // ✅ Getter for movement completion
};

#endif // MOVE_HPP

#ifndef BANGBANGCONTROL_HPP
#define BANGBANGCONTROL_HPP

#include <QVector2D>
#include <QList>
#include "motioncommand.hpp"
#include "robotstate.hpp"
#include "trajectory2d.hpp"

class BangBangControl {
public:
    BangBangControl(float a_max, float v_max);

    MotionCommand computeMotion(const RobotState& state, QList<QVector2D> path, double delta);

private:
    bool isNearToBreak(const RobotState& robot, const QVector2D& point) const;

    float m_A_MAX;
    float m_V_MAX;
};

#endif // BANGBANGCONTROL_HPP

#pragma once

#include <QVector2D>
#include <vector>
#include "world.hpp"
#include "robotstate.hpp"

class Environment {
public:
    Environment(const World* world, const RobotState& self);

    bool collides(const QVector2D& point) const;

    const std::vector<QVector2D>& getRobots() const;
    const QVector2D& getBallPosition() const;

private:
    std::vector<QVector2D> robots;
    QVector2D ball_position;
};

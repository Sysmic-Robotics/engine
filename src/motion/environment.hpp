#pragma once

#include <QVector2D>
#include <vector>

class Environment {
public:
    Environment(const std::vector<QVector2D>& robots, const QVector2D& ball_position);

    bool collides(const QVector2D& point) const;

    const std::vector<QVector2D>& getRobots() const;
    const QVector2D& getBallPosition() const;

private:
    std::vector<QVector2D> robots;
    QVector2D ball_position;
};

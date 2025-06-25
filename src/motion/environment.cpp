#include "environment.hpp"

Environment::Environment(const std::vector<QVector2D>& robots, const QVector2D& ball_position)
    : robots(robots), ball_position(ball_position) {}

bool Environment::collides(const QVector2D& point) const {
    // Field bounds check
    if (point.x() < -4.5 || point.x() > 4.5 || point.y() < -3.0 || point.y() > 3.0)
        return true;

    // Robot collision check
    for (const auto& robot : robots) {
        if ((point - robot).length() <= 0.2)  // Robot radius
            return true;
    }

    if ((ball_position - point).length() <= 0.1){
        return true;
    }

    return false;
}

const std::vector<QVector2D>& Environment::getRobots() const {
    return robots;
}

const QVector2D& Environment::getBallPosition() const {
    return ball_position;
}

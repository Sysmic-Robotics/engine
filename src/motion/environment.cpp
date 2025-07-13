#include "environment.hpp"


// In Environment.cpp
Environment::Environment(const World* world, const RobotState& self) {
    int selfId = self.getId();

    // Gather positions of all other active robots
    for (int id = 0; id < 12; ++id) {
        if (id == selfId) continue;

        RobotState rBlue = world->getRobotState(id, 0);
        if (rBlue.isActive()) robots.push_back(rBlue.getPosition());

        RobotState rYellow = world->getRobotState(id, 1);
        if (rYellow.isActive()) robots.push_back(rYellow.getPosition());
    }

    // Get ball position
    ball_position = world->getBallState().getPosition();
}

bool Environment::collides(const QVector2D& point) const {
    // Field bounds check
    if (point.x() < -4.5 || point.x() > 4.5 || point.y() < -3.0 || point.y() > 3.0)
        return true;

    // Yellow goalie box (x: 3.5 to 4.5, y: -1 to 1)
    if (point.x() >= 3.5 && point.x() <= 4.5 &&
        point.y() >= -1.0 && point.y() <= 1.0)
        return true;

    // Blue goalie box (x: -4.5 to -3.5, y: -1 to 1)
    if (point.x() >= -4.5 && point.x() <= -3.5 &&
        point.y() >= -1.0 && point.y() <= 1.0)
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

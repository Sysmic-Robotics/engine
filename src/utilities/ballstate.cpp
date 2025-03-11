#include "ballstate.hpp"

BallState::BallState() : position(0, 0), velocity(0, 0) {}

BallState::BallState(QVector2D position, QVector2D velocity)
    : position(position), velocity(velocity) {}

QVector2D BallState::getPosition() const {
    return position;
}

QVector2D BallState::getVelocity() const {
    return velocity;
}

bool BallState::isMoving() const {
    return velocity.length() > 0.1;  // Consider movement if speed > 0.01 m/s
}

void BallState::setPosition(const QVector2D &pos) {
    position = pos;
}

void BallState::setVelocity(const QVector2D &vel) {
    velocity = vel;
}

QString BallState::toString() const {
    return QString("Ball Position: (%1, %2), Velocity: (%3, %4)")
        .arg(position.x()).arg(position.y())
        .arg(velocity.x()).arg(velocity.y());
}
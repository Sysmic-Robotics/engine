#include "robotstate.hpp"

// Default constructor
RobotState::RobotState() : robotId(0), team(0), position(0, 0), orientation(0.0f), height(0.0f) {}

// Parameterized constructor
RobotState::RobotState(int id, int team) : robotId(id), team(team), position(0, 0), orientation(0.0f), height(0.0f) {}

int RobotState::getId() const {
    return robotId;
}

int RobotState::getTeam() const {
    return team;
}

bool RobotState::isActive() const{
    return active;
}

QVector2D RobotState::getPosition() const {
    return position;
}

QVector2D RobotState::getVelocity() const {
    return velocity;
}

float RobotState::getAngularVelocity() const {
    return angular_velocity;
}

void RobotState::setAngularVelocity(float omega) {
    angular_velocity = omega;
}

float RobotState::getOrientation() const {
    return orientation;
}

void RobotState::setId(int id) {
    robotId = id;
}

void RobotState::setActive(bool flag) {
    active = flag;
}

void RobotState::setTeam(int team) {
    this->team = team;
}

void RobotState::setPosition(const QVector2D &position) {
    this->position = position;
}

void RobotState::setVelocity(const QVector2D &velocity) {
    this->velocity = velocity;
}


void RobotState::setOrientation(float orientation) {
    this->orientation = orientation;
}



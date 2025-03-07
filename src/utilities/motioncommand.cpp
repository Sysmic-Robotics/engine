#include "motioncommand.hpp"
#include <QDebug>



MotionCommand::MotionCommand(int id, int team)
    : id(id), team(team) {}

MotionCommand::MotionCommand(int id, int team, double vx, double vy)
    : id(id), team(team), vx(vx), vy(vy) {}

// Getters
int MotionCommand::getId() const { return id; }
int MotionCommand::getTeam() const { return team; }
double MotionCommand::getVx() const { return vx; }
double MotionCommand::getVy() const { return vy; }
double MotionCommand::getAngular() const { return angular; }

// Setters
void MotionCommand::setVx(double vx_) { vx = vx_; }
void MotionCommand::setVy(double vy_) { vy = vy_; }
void MotionCommand::setAngular(double angular_) { angular = angular_; }

// Debugging helper
QString MotionCommand::toString() const {
    return QString("MotionCommand: vx=%1, vy=%2, omega=%3")
        .arg(vx)
        .arg(vy)
        .arg(angular);
}

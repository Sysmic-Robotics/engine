#include "motioncommand.hpp"
#include <QDebug>

MotionCommand::MotionCommand() : vx(0.0), vy(0.0), omega(0.0) {}

MotionCommand::MotionCommand(double vx, double vy, double omega)
    : vx(vx), vy(vy), omega(omega) {}

// Getters
double MotionCommand::getVx() const { return vx; }
double MotionCommand::getVy() const { return vy; }
double MotionCommand::getOmega() const { return omega; }

// Setters
void MotionCommand::setVx(double vx_) { vx = vx_; }
void MotionCommand::setVy(double vy_) { vy = vy_; }
void MotionCommand::setOmega(double omega_) { omega = omega_; }

// Debugging helper
QString MotionCommand::toString() const {
    return QString("MotionCommand: vx=%1, vy=%2, omega=%3")
        .arg(vx)
        .arg(vy)
        .arg(omega);
}

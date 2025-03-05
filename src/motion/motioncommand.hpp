#ifndef MOTION_COMMAND_HPP
#define MOTION_COMMAND_HPP

#include <QVector2D>
#include <QString>

class MotionCommand {
public:
    MotionCommand();
    MotionCommand(double vx, double vy, double omega);

    // Getters
    double getVx() const;
    double getVy() const;
    double getOmega() const;

    // Setters
    void setVx(double vx);
    void setVy(double vy);
    void setOmega(double omega);

    // Debugging helper (for Qt logging)
    QString toString() const;

private:
    double vx = 0.0;  // Linear velocity in the X direction
    double vy = 0.0;  // Linear velocity in the Y direction
    double omega = 0.0;  // Angular velocity (rotation)
};

#endif // MOTION_COMMAND_HPP

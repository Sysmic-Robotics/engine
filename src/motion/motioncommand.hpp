#ifndef MOTION_COMMAND_HPP
#define MOTION_COMMAND_HPP

#include <QVector2D>
#include <QString>

class MotionCommand {
public:
    MotionCommand(int id, int team);

    // Getters
    int getId() const;
    int getTeam() const;
    double getVx() const;
    double getVy() const;
    double getAngular() const;

    // Setters
    void setVx(double vx);
    void setVy(double vy);
    void setAngular(double omega);

    // Debugging helper (for Qt logging)
    QString toString() const;

private:
    double vx = 0.0;  // Linear velocity in the X direction
    double vy = 0.0;  // Linear velocity in the Y direction
    double angular = 0.0;  // Angular velocity (rotation)
    int team;
    int id;
};

#endif // MOTION_COMMAND_HPP

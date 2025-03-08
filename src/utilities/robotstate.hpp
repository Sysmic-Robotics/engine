#ifndef ROBOTSTATE_HPP
#define ROBOTSTATE_HPP

#include <QVector2D>
#include <QString>

class RobotState {
public:
    RobotState();  // Default constructor
    RobotState(int id, int team);  // Parameterized constructor

    int getId() const;
    int getTeam() const;
    QVector2D getPosition() const;
    QVector2D getVelocity() const;
    float getOrientation() const;
    bool isActive() const;

    void setId(int id);
    void setTeam(int team);
    void setPosition(const QVector2D &position);
    void setVelocity(const QVector2D &velocity);
    void setOrientation(float orientation);
    void setActive(bool flag);

private:
    int robotId;
    int team;
    QVector2D position;  // [x, y]
    QVector2D velocity;
    float orientation = 0.0f;  // Explicitly initialize
    float height = 0.0f;       // Explicitly initialize
    bool active = false;
};

#endif // ROBOTSTATE_HPP

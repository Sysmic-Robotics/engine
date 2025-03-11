#ifndef BALLSTATE_HPP
#define BALLSTATE_HPP

#include <QVector2D>
#include <QString>

class BallState {
public:
    BallState();  // Default constructor
    BallState(QVector2D position, QVector2D velocity);  // Parameterized constructor

    // Getters
    QVector2D getPosition() const;
    QVector2D getVelocity() const;
    bool isMoving() const;

    // Setters
    void setPosition(const QVector2D &position);
    void setVelocity(const QVector2D &velocity);

    // Debugging helper (for Qt logging)
    QString toString() const;

private:
    QVector2D position;  // Ball's position [x, y]
    QVector2D velocity;  // Ball's velocity [vx, vy]
};

#endif // BALLSTATE_HPP

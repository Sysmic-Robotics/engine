#ifndef WORLD_HPP
#define WORLD_HPP

#include "robotstate.hpp"
#include "ballstate.hpp"
#include <QMap>
#include <QVector2D>
#include <QObject>
#include <QList>
#include <QDebug>


class World : public QObject {
    Q_OBJECT  // Ensure Qt's meta-object system works
    public:
        explicit World(int n_blue, int n_yellow, QObject *parent = nullptr);
        
        void update();
        // Returns the RobotState of a robot given its team and ID
        RobotState getRobotState(int id, int team) const;

    public slots:
        void updateRobot(int id, int team, QVector2D position, float orientation);
        void updateBall(QVector2D position);

    signals:
        void robotUpdated(const RobotState &robotState); // New Signal
        void ballUpdated(const BallState &ball);
    
    private:
        QMap<int, RobotState> yellowRobots;  // Key: robot ID, Value: yellow team robot state
        QMap<int, RobotState> blueRobots;  // Key: robot ID, Value: blue team robot state
        BallState ball;

};

#endif // WORLD_HPP

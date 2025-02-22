#ifndef WORLD_HPP
#define WORLD_HPP

#include "robotstate.hpp"
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

    public slots:
        void updateRobot(int id, int team, QVector2D position, float orientation);

    signals:
        void robotUpdated(int id, int team, QVector2D position, float orientation); // New Signal
    
    private:
        QMap<int, RobotState> yellowRobots;  // Key: robot ID, Value: yellow team robot state
        QMap<int, RobotState> blueRobots;  // Key: robot ID, Value: blue team robot state
};

#endif // WORLD_HPP

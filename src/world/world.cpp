#include "world.hpp"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

QJsonObject World::toJson() const {
    QJsonObject root;

    QJsonArray robotsArray;

    // Serialize blue robots
    for (const RobotState &robot : blueRobots) {
        if (!robot.isActive()) continue;

        QJsonObject robObj;
        robObj["id"] = robot.getId();
        robObj["team"] = "blue";

        QJsonObject pos;
        pos["x"] = robot.getPosition().x();
        pos["y"] = robot.getPosition().y();
        robObj["position"] = pos;

        QJsonObject vel;
        vel["x"] = robot.getVelocity().x();
        vel["y"] = robot.getVelocity().y();
        robObj["velocity"] = vel;

        robObj["orientation"] = robot.getOrientation();

        robotsArray.append(robObj);
    }

    // Serialize yellow robots
    for (const RobotState &robot : yellowRobots) {
        if (!robot.isActive()) continue;

        QJsonObject robObj;
        robObj["id"] = robot.getId();
        robObj["team"] = "yellow";

        QJsonObject pos;
        pos["x"] = robot.getPosition().x();
        pos["y"] = robot.getPosition().y();
        robObj["position"] = pos;

        QJsonObject vel;
        vel["x"] = robot.getVelocity().x();
        vel["y"] = robot.getVelocity().y();
        robObj["velocity"] = vel;

        robObj["orientation"] = robot.getOrientation();

        robotsArray.append(robObj);
    }

    root["robots"] = robotsArray;

    // Serialize ball
    QJsonObject ballObj;
    QJsonObject ballPos;
    ballPos["x"] = ball.getPosition().x();
    ballPos["y"] = ball.getPosition().y();
    ballObj["position"] = ballPos;

    QJsonObject ballVel;
    ballVel["x"] = ball.getVelocity().x();
    ballVel["y"] = ball.getVelocity().y();
    ballObj["velocity"] = ballVel;

    root["ball"] = ballObj;

    return root;
}

World::World(int n_blue, int n_yellow, QObject *parent) : QObject(parent) {
    ball = BallState();
    for (int id = 0; id < n_blue; id++) {   
        blueRobots.insert(id, RobotState(id, 0));
    }
    for (int id = 0; id < n_yellow; id++) {   
        yellowRobots.insert(id, RobotState(id, 1));
    }
}

RobotState World::getRobotState(int id, int team) const {
    if (team == 0) {  // Blue team
        if (blueRobots.contains(id)) {
            return blueRobots.value(id);
        }
    } else if (team == 1) {  // Yellow team
        if (yellowRobots.contains(id)) {
            return yellowRobots.value(id);
        }
    }
    return RobotState();  // Return a default RobotState if not found
}

BallState World::getBallState() const {
    return ball;
}

void World::update() {
    // Robot is not being updated here; maybe a better name should be chosen for "robotUpdated" signal
    for (const RobotState &robot : blueRobots) { // Use reference to avoid unnecessary copies
        if ( robot.isActive() );
            emit robotUpdated(robot);
    }
    
    for (const RobotState &robot : yellowRobots) { // Use reference here too
        if ( robot.isActive() );
            emit robotUpdated(robot);
    }
    emit ballUpdated(ball);
}


void World::updateRobot(int id, int team, QVector2D position, float orientation) {
    constexpr double timeStep = 0.016; // 1/60 in seconds
    if (team == 0){
        if (blueRobots.contains(id)) {
            blueRobots[id].setVelocity( ( position - blueRobots[id].getPosition() )/timeStep );
            blueRobots[id].setPosition(position);
            blueRobots[id].setOrientation(orientation);
            blueRobots[id].setActive(true);
        }
    }else{
        if (yellowRobots.contains(id)) {
            yellowRobots[id].setVelocity( ( position - yellowRobots[id].getPosition() )/timeStep );
            yellowRobots[id].setPosition(position);
            yellowRobots[id].setOrientation(orientation);
            yellowRobots[id].setActive(true);
        }
    }
}

void World::updateBall(QVector2D position){
    constexpr double timeStep = 0.016; // 1/60 in seconds
    ball.setVelocity( ( position - ball.getPosition() )/timeStep );
    ball.setPosition(position);
}
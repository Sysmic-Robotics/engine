#include "world.hpp"


World::World(QObject *parent) : QObject(parent) {
    for (int id = 0; id < 3; id++) {   
        blueRobots.insert(id, RobotState(id, 0));
    }
    for (int id = 0; id < 3; id++) {   
        yellowRobots.insert(id, RobotState(id, 1));
    }
}


void World::update() {
    for (const RobotState &robot : blueRobots) { // Use reference to avoid unnecessary copies
        if ( robot.isActive() );
            emit robotUpdated(robot.getId(), robot.getTeam(), robot.getPosition(), robot.getOrientation());
    }
    
    for (const RobotState &robot : yellowRobots) { // Use reference here too
        if ( robot.isActive() );
        emit robotUpdated(robot.getId(), robot.getTeam(), robot.getPosition(), robot.getOrientation());
    }
}
   



void World::updateRobot(int id, int team, QVector2D position, float orientation) {
    if (team == 0){
        blueRobots[id].setPosition(position);
        blueRobots[id].setOrientation(orientation);
        blueRobots[id].setActive(true);
    }else{
        yellowRobots[id].setPosition(position);
        yellowRobots[id].setOrientation(orientation);
        yellowRobots[id].setActive(true);
    }
}
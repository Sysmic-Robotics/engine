#include "world.hpp"


World::World(int n_blue, int n_yellow, QObject *parent) : QObject(parent) {
    for (int id = 0; id < n_blue; id++) {   
        blueRobots.insert(id, RobotState(id, 0));
    }
    for (int id = 0; id < n_yellow; id++) {   
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
        if (blueRobots.contains(id)) {
            blueRobots[id].setPosition(position);
            blueRobots[id].setOrientation(orientation);
            blueRobots[id].setActive(true);
        }
    }else{
        if (yellowRobots.contains(id)) {
            yellowRobots[id].setPosition(position);
            yellowRobots[id].setOrientation(orientation);
            yellowRobots[id].setActive(true);
        }
    }
}
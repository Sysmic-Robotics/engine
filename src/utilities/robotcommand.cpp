#include "robotcommand.hpp"

// Constructor
RobotCommand::RobotCommand(int id, int team)
    : id(id), team(team), motionCommand(id, team), kickerCommand(id, team) {}

// Getters
int RobotCommand::getId() const {
    return id;
}

int RobotCommand::getTeam() const {
    return team;
}

MotionCommand RobotCommand::getMotionCommand() const {
    return motionCommand;
}

KickerCommand RobotCommand::getKickerCommand() const {
    return kickerCommand;
}

// Setters
void RobotCommand::setMotionCommand(const MotionCommand& motion) {
    motionCommand = motion;
}

void RobotCommand::setKickerCommand(const KickerCommand& kicker) {
    kickerCommand = kicker;
}

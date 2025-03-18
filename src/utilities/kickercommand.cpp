#include "kickercommand.hpp"
#include <QDebug>

// Constructor
KickerCommand::KickerCommand(int id, int team) : id(id), team(team) {}

// Getters
int KickerCommand::getId() const {
    return id;
}

int KickerCommand::getTeam() const {
    return team;
}

bool KickerCommand::getKickX() const {
    return kicker_x;
}

bool KickerCommand::getKickZ() const {
    return kicker_z;
}

double KickerCommand::getDribbler() const {
    return dribbler_speed;
}

// Setters
void KickerCommand::setKickX(bool flag) {
    kicker_x = flag;
}

void KickerCommand::setKickZ(bool flag) {
    kicker_z = flag;
}

void KickerCommand::setDribbler(double speed) {
    dribbler_speed = speed;
}

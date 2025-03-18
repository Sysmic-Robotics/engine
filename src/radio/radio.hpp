#ifndef RADIO_HPP
#define RADIO_HPP

#include <QHash>
#include <QDebug>
#include "robotcommand.hpp"
#include "grsim.hpp"

class Radio {
public:
    // Adds/updates a MotionCommand for a specific robot.
    void addMotionCommand(const MotionCommand& motion);
    // Adds/updates a KickerCommand for a specific robot.
    void addKickerCommand(const KickerCommand& kicker);
    // Sends all combined commands via Grsim.
    void sendCommands();

private:
    // QHash keyed by robot ID.
    QHash<int, RobotCommand> commandMap;
};

#endif // RADIO_HPP

#ifndef ROBOT_COMMAND_HPP
#define ROBOT_COMMAND_HPP

#include "motioncommand.hpp"
#include "kickercommand.hpp"

class RobotCommand {
public:
    RobotCommand(int id, int team);

    // Getters
    int getId() const;
    int getTeam() const;
    MotionCommand getMotionCommand() const;
    KickerCommand getKickerCommand() const;

    // Setters
    void setMotionCommand(const MotionCommand& motion);
    void setKickerCommand(const KickerCommand& kicker);

private:
    int id;
    int team;
    MotionCommand motionCommand;
    KickerCommand kickerCommand;
};

#endif // ROBOT_COMMAND_HPP

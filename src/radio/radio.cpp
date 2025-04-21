#include "radio.hpp"

// Add or update a MotionCommand for a robot.
void Radio::addMotionCommand(const MotionCommand& motion) {
    int robotId = motion.getId();
    
    // Use QHash::find() to see if a command for this robot already exists.
    auto it = commandMap.find(robotId);
    if (it == commandMap.end()) {
        // Insert a new RobotCommand if it doesn't exist.
        commandMap.insert(robotId, RobotCommand(robotId, motion.getTeam()));
        it = commandMap.find(robotId);
    }
    
    // Update the MotionCommand by merging the new command with the existing one.
    RobotCommand &cmd = it.value();
    MotionCommand currentMotion = cmd.getMotionCommand();

    // Overwrite only non-default parameters.
    double newVx = (motion.getVx() != 0.0) ? motion.getVx() : currentMotion.getVx();
    double newVy = (motion.getVy() != 0.0) ? motion.getVy() : currentMotion.getVy();
    double newAngular = (motion.getAngular() != 0.0) ? motion.getAngular() : currentMotion.getAngular();

    currentMotion.setVx(newVx);
    currentMotion.setVy(newVy);
    currentMotion.setAngular(newAngular);
    
    cmd.setMotionCommand(currentMotion);
}

// Add or update a KickerCommand for a robot.
void Radio::addKickerCommand(const KickerCommand& kicker) {
    int robotId = kicker.getId();
    
    auto it = commandMap.find(robotId);
    if (it == commandMap.end()) {
        commandMap.insert(robotId, RobotCommand(robotId, kicker.getTeam()));
        it = commandMap.find(robotId);
    }
    
    RobotCommand &cmd = it.value();
    KickerCommand currentKicker = cmd.getKickerCommand();

    // Overwrite non-default parameters.
    bool newKickX = kicker.getKickX() ? true : currentKicker.getKickX();
    bool newKickZ = kicker.getKickZ() ? true : currentKicker.getKickZ();
    double newDribbler = (kicker.getDribbler() != 0.0) ? kicker.getDribbler() : currentKicker.getDribbler();

    currentKicker.setKickX(newKickX);
    currentKicker.setKickZ(newKickZ);
    currentKicker.setDribbler(newDribbler);
    
    cmd.setKickerCommand(currentKicker);
}

// Send all commands using Grsim and then clear the command map.
void Radio::sendCommands() {
    if (commandMap.isEmpty()) {
        return;
    }
    static Grsim grsim;

    // Iterate over all robot commands in the QHash.
    for (auto it = commandMap.begin(); it != commandMap.end(); ++it) {
        RobotCommand cmd = it.value();
        MotionCommand motion = cmd.getMotionCommand();
        KickerCommand kicker = cmd.getKickerCommand();

        grsim.communicate_grsim(
            cmd.getId(),             // Robot ID
            cmd.getTeam(),           // Team
            motion.getAngular(),     // Angular velocity
            kicker.getKickX() ? 5.0 : 0,  // KickSpeedX (example: power 5.0 if enabled)
            kicker.getKickZ() ? 3.0 : 0,  // KickSpeedZ (example: power 3.0 if enabled)
            motion.getVx(),          // Velocity X
            motion.getVy(),          // Velocity Y
            kicker.getDribbler(),    // Dribbler speed
            false                    // Placeholder flag
        );

    }
    
    // Clear commands after sending.
    commandMap.clear();
}

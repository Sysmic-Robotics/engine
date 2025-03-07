#include "radio.hpp"

// Append command to queue
void Radio::appendCommand(MotionCommand command) {
    commandQueue.append(command);
}

void Radio::sendCommands() {
    if (commandQueue.isEmpty()) {
        qDebug() << "No commands to send.";
        return;
    }
    static Grsim grsim;

    for (const MotionCommand& cmd : commandQueue) {
        grsim.communicate_grsim(
            cmd.getId(), // ID
            cmd.getTeam(), // Team
            cmd.getAngular(), // Vel angular
            0, // KickSpeedX
            0, // KickSpeedZ
            cmd.getVx(), // Vel x
            cmd.getVy(), // Vel y
            0, // Spinner
            false);
    }
    // Clear the queue after sending
    commandQueue.clear();

}
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
            1, // ID
            1, // Team
            2, // Vel angular
            0, // KickSpeedX
            0, // KickSpeedZ
            0, // Vel x
            0, // Vel y
            0, // Spinner
            false);
    }
    // Clear the queue after sending
    commandQueue.clear();

}
#ifndef RADIO_HPP
#define RADIO_HPP

#include <QList>
#include <QDebug>
#include "motioncommand.hpp"
#include "grsim.hpp"

class Radio {
public:

    void appendCommand(MotionCommand command);
    void sendCommands();

    private:
        QList<MotionCommand> commandQueue;

};

#endif // RADIO_HPP

// packetserializer.cpp
#include "packetserializer.hpp"
#include <cmath>
#include <QDebug>

QByteArray PacketSerializer::serialize(const QHash<int, RobotCommand>& commandMap,
                                       int numRobots)
{
    QByteArray packet;
    packet.resize(numRobots * 5);
    packet.fill(char(0));

    for (int id = 0; id < numRobots; ++id) {
        uint8_t bytes[5] = {0,0,0,0,0};

        auto it = commandMap.find(id);
        if (it != commandMap.end()) {
            const RobotCommand& cmd = it.value();
            const MotionCommand& m = cmd.getMotionCommand();
            const KickerCommand& k = cmd.getKickerCommand();

            int vX      = static_cast<int>(m.getVx()*100);
            int vY      = static_cast<int>(m.getVy()*100);
            int vTH     = static_cast<int>(m.getAngular()*100);
            int dribb   = static_cast<int>(k.getDribbler());
            qDebug() << " m.getVx():" << m.getVx()
                     << " m.getVy():" << m.getVy()
                     << " m.getAngular():" << m.getAngular()
                     << " k.getDribbler():" << k.getDribbler();

            int kick    = k.getKickX() ? 1 : 0;
            int callback= 0;  // si necesitas callback, ajusta aquí

            // Byte 0: ID(3) | dribbler(3) | kick(1) | callback(1)
            bytes[0]  = (id & 0x07) << 5;
            bytes[0] |= (dribb & 0x07) << 2;
            bytes[0] |= (kick & 0x01) << 1;
            bytes[0] |= (callback & 0x01);

            // Byte 1: signo vX (1) | 7 bits abs(vX)
            bytes[1] = ((vX >= 0) ? 1 : 0) << 7
                     | (std::abs(vX) & 0x7F);

            // Byte 2: signo vY (1) | 7 bits abs(vY)
            bytes[2] = ((vY < 0) ? 1 : 0) << 7
                     | (std::abs(vY) & 0x7F);

            // Byte 3: signo vTH (1) | 7 bits abs(vTH)
            bytes[3] = ((vTH < 0) ? 1 : 0) << 7
                     | (std::abs(vTH) & 0x7F);

            // Byte 4: MSB vX (2) | MSB vY (2) | MSB vTH (4)
            bytes[4]  = ((std::abs(vX) >> 7) & 0x03) << 6;
            bytes[4] |= ((std::abs(vY) >> 7) & 0x03) << 4;
            bytes[4] |= ((std::abs(vTH)>> 7) & 0x0F);
        }

        // Copiar estos 5 bytes en la posición correspondiente
        for (int b = 0; b < 5; ++b) {
            packet[id*5 + b] = char(bytes[b]);
        }
    }

    return packet;
}

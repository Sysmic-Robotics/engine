// packetserializer.hpp
#ifndef PACKETSERIALIZER_HPP
#define PACKETSERIALIZER_HPP

#include <QByteArray>
#include <QHash>
#include "robotcommand.hpp"

namespace PacketSerializer {

/// Serializa los comandos en un bloque de 5 bytes por robot.
/// @param commandMap  Mapa de RobotCommand indexado por ID (0..numRobots-1)
/// @param numRobots   Número total de robots (por defecto 6)
/// @return QByteArray de tamaño numRobots*5 listo para enviar
QByteArray serialize(const QHash<int, RobotCommand>& commandMap,
                     int numRobots = 6);

} // namespace PacketSerializer

#endif // PACKETSERIALIZER_HPP

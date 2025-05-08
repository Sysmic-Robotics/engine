// radio.hpp
#ifndef RADIO_HPP 
#define RADIO_HPP

#include <QHash>
#include <QDebug>
#include <QSerialPort>

#include "robotcommand.hpp"
#include "grsim.hpp"

class Radio {
public:
    /// Constructor
    /// @param portName  Nombre del puerto (ej. "COM5")
    /// @param baudRate  Baud rate (ej. QSerialPort::Baud115200)
    /// @param useRadio  true = enviar por USB serial; false = enviar sólo a grSim
    Radio(const QString &portName = "COM5",
          bool useRadio = true);

    ~Radio();

    void addMotionCommand(const MotionCommand& motion);
    void addKickerCommand(const KickerCommand& kicker);

    /// Envía todos los comandos acumulados
    void sendCommands();

    /// Permite cambiar el modo en tiempo de ejecución
    void setUseRadio(bool enable) { m_useRadio = enable; }

private:
    QHash<int, RobotCommand> commandMap;
    QSerialPort          serialPort;
    bool                 m_useRadio;
};

#endif // RADIO_HPP

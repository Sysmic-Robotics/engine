// radio.hpp
#ifndef RADIO_HPP
#define RADIO_HPP

#include <QObject>
#include <QHash>
#include <QDebug>
#include <QSerialPort>
#include <QFile>
#include "robotcommand.hpp"
#include "grsim.hpp"

class Radio : public QObject{
    Q_OBJECT
public:
    /// Constructor
    /// @param useRadio  true = enviar por USB serial; false = enviar sólo a grSim
    /// @param portName  Nombre del puerto (ej. "COM5" o "/dev/ttyUSB0")
    /// @param baudRate  Baud rate (ej. QSerialPort::Baud115200)
    Radio(bool useRadio = true, const QString &portName = "COM5", qint32 baudRate = QSerialPort::Baud115200);

    ~Radio();

    void addMotionCommand(const MotionCommand &motion);
    void addKickerCommand(const KickerCommand &kicker);

    /// Envía todos los comandos acumulados
    void sendCommands();

    /// Permite cambiar el modo en tiempo de ejecución
    void setUseRadio(bool enable) { m_useRadio = enable; }
    
    const QHash<int, RobotCommand>& getCommandMap() const;
    
    void teleportRobot(int id, int team, double x, double y, double dir);
    void teleportBall(double x, double y);
private:
    QHash<int, RobotCommand> commandMap;
    QSerialPort serialPort;
    bool m_useRadio;
    qint32 m_baudRate;
    QString m_portName;
};

#endif // RADIO_HPP

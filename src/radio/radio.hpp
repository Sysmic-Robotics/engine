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
#include "world.hpp"

class Radio : public QObject{
    Q_OBJECT
public:
    /// Constructor
    /// @param portName  Nombre del puerto (ej. "COM5")
    /// @param baudRate  Baud rate (ej. QSerialPort::Baud115200)
    /// @param useRadio  true = enviar por USB serial; false = enviar sólo a grSim
    Radio(World *_world, bool useRadio = false, const QString &portName = "COM5");

    ~Radio();

    void addMotionCommand(const MotionCommand &motion);
    void addKickerCommand(const KickerCommand &kicker);

    /// Envía todos los comandos acumulados
    void sendCommands();

    /// Permite cambiar el modo en tiempo de ejecución
    void setUseRadio(bool enable) { m_useRadio = enable; }

private:
    QHash<int, RobotCommand> commandMap;
    QSerialPort serialPort;
    bool m_useRadio;
    World *m_world;

private:
    bool m_isRecording = false;
    QFile m_logFile;
    QTextStream m_logStream;

public slots:
    void startRecording();
    void stopRecording();
};

#endif // RADIO_HPP

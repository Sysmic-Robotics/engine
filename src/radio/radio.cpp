// radio.cpp
#include "radio.hpp"
#include "packetserializer.hpp"

Radio::Radio(bool useRadio, const QString &portName)
    : m_useRadio(useRadio)
{
    
    if (m_useRadio) {
        qint32 baudRate = QSerialPort::Baud115200;
        serialPort.setPortName(portName);
        serialPort.setBaudRate(baudRate);
        serialPort.setDataBits(QSerialPort::Data8);
        serialPort.setParity(QSerialPort::NoParity);
        serialPort.setStopBits(QSerialPort::OneStop);
        serialPort.setFlowControl(QSerialPort::NoFlowControl);

        if (!serialPort.open(QIODevice::WriteOnly)) {
            qWarning() << "Radio: no se pudo abrir puerto serial"
                       << portName << ":" << serialPort.errorString();
        } else {
            qDebug() << "Radio: puerto serial abierto en" << portName;
        }
    }
}

Radio::~Radio()
{
    if (serialPort.isOpen()) {
        serialPort.close();
        qDebug() << "Radio: puerto serial cerrado";
    }
}

void Radio::addMotionCommand(const MotionCommand& motion) {
    int id = motion.getId();
    auto it = commandMap.find(id);
    if (it == commandMap.end()) {
        commandMap.insert(id, RobotCommand(id, motion.getTeam()));
        it = commandMap.find(id);
    }
    RobotCommand &cmd = it.value();
    MotionCommand cm = cmd.getMotionCommand();
    if (motion.getVx()      != 0.0) cm.setVx(motion.getVx());
    if (motion.getVy()      != 0.0) cm.setVy(motion.getVy());
    if (motion.getAngular() != 0.0) cm.setAngular(motion.getAngular());
    cmd.setMotionCommand(cm);
}

void Radio::addKickerCommand(const KickerCommand& kicker) {
    int id = kicker.getId();
    auto it = commandMap.find(id);
    if (it == commandMap.end()) {
        commandMap.insert(id, RobotCommand(id, kicker.getTeam()));
        it = commandMap.find(id);
    }
    RobotCommand &cmd = it.value();
    KickerCommand kc = cmd.getKickerCommand();
    if (kicker.getKickX())       kc.setKickX(true);
    if (kicker.getKickZ())       kc.setKickZ(true);
    if (kicker.getDribbler() != 0) kc.setDribbler(kicker.getDribbler());
    cmd.setKickerCommand(kc);
}

void Radio::sendCommands() {
    if (commandMap.isEmpty()) return;

    if (m_useRadio) {
        // 1) Serializar todo el paquete en un único bloque
        QByteArray buffer = PacketSerializer::serialize(commandMap, /*numRobots=*/6);

        // 2) Enviar por serial
        if (serialPort.isOpen()) {
            serialPort.write(buffer);
            serialPort.flush();
        }
    } else {
        // Envío exclusivo a grSim
        static Grsim grsim;
        for (auto it = commandMap.begin(); it != commandMap.end(); ++it) {
            const RobotCommand &cmd = it.value();
            const MotionCommand &m = cmd.getMotionCommand();
            const KickerCommand &k = cmd.getKickerCommand();

            grsim.communicate_grsim(
                cmd.getId(),
                cmd.getTeam(),
                m.getAngular(),
                k.getKickX() ? 3.0 : 0.0,
                k.getKickZ() ? 3.0 : 0.0,
                m.getVx(),
                m.getVy(),
                k.getDribbler(),
                false
            );
        }
    }

    commandMap.clear();
}

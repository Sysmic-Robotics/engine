// radioRx.hpp
#ifndef RADIORX_HPP
#define RADIORX_HPP

#include <QObject>
#include <QHash>
#include <QDebug>
#include <QSerialPort>
#include <QFile>
#include <QTimer>
#include <QVector4D>
#include "robotcommand.hpp"
#include "grsim.hpp"

class RadioRx : public QObject{
    Q_OBJECT
public:
    /// Constructor
    /// @param portName  Nombre del puerto (ej. "COM5" o "/dev/ttyUSB0")
    /// @param baudRate  Baud rate (ej. QSerialPort::Baud460800)
    RadioRx(const QString &portName = "COM6", qint32 baudRate = QSerialPort::Baud460800);

    ~RadioRx();

    struct RobotRxData {
        int id;
        uint16_t ballDistance;
    };

    // Devuelve el último paquete recibido para cada robot
    QHash<int, RobotRxData> getLatestRobotData() const;

    void readSerial();

private:
    QSerialPort serialPort;
    qint32 m_baudRate;
    QString m_portName;
    QHash<int, RobotRxData> latestRobotData;
};

#endif // RADIORX_HPP

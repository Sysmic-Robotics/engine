// radioRx.cpp
#include "radioRx.hpp"
#include "packetserializer.hpp"
#include <QDebug>
#include <QTimer>
#include <QDataStream>
#include <QtEndian>
#include <cstring>

RadioRx::RadioRx(const QString &portName, qint32 baudRate)
    : m_baudRate(baudRate), m_portName(portName), pollTimer(nullptr)
{
    serialPort.setPortName(m_portName);
    serialPort.setBaudRate(m_baudRate);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    serialPort.open(QIODevice::ReadOnly);
}

RadioRx::~RadioRx()
{
    if (serialPort.isOpen()) {
        serialPort.close();
        qDebug() << "RadioRx: puerto serial cerrado";
    }
    if (pollTimer) pollTimer->stop();
}

void RadioRx::readSerial() {
    while (serialPort.bytesAvailable() >= 32) {
        QByteArray packet = serialPort.read(32);
        if (packet.size() != 32) return;
        // Parse first byte
        quint8 b0 = static_cast<quint8>(packet[0]);
        int id = (b0 & 0xE0) >> 5; // 3 MSB
        bool ballDetected = (b0 & 0x01);
        float wheelSpeeds[4];
        for (int i = 0; i < 4; ++i) {
            float value;
            std::memcpy(&value, packet.constData() + 1 + i * 4, 4);
            wheelSpeeds[i] = value;
        }
        RobotRxData data{id, ballDetected, {wheelSpeeds[0], wheelSpeeds[1], wheelSpeeds[2], wheelSpeeds[3]}};
        latestRobotData[id] = data;
    }
}

QHash<int, RadioRx::RobotRxData> RadioRx::getLatestRobotData() const {
    return latestRobotData;
}




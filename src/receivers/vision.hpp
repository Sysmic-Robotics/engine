#ifndef VISION_H
#define VISION_H

#include <QCoreApplication>
#include <QUdpSocket>
#include <QHostAddress>
#include <QMap>
#include <utility> // for std::pair
#include "../protobuf/ssl_vision_wrapper.pb.h"
#include "../protobuf/ssl_vision_detection.pb.h"
#include "../utilities/robotstate.hpp"
#include "tracker.hpp"


class Vision : public QObject{
    Q_OBJECT  // Add this macro to enable Qt's signal/slot mechanism
public:
    Vision(const QString &multicastAddress, int port, QObject *parent = nullptr);
    void startListen();

signals:
    void robotReceived(int id, int team, QVector2D position, float orientation, QVector2D lineal_velocity, float angular_velocity);  // Signal to notify when packet is received
    void ballReceived(QVector2D position, QVector2D velocity);

private:
    QUdpSocket *m_udpSocket;
    QString m_multicastAddress;
    int m_port;
    void readPendingDatagrams();
    void deserializePacket(const QByteArray &datagram);
    void processData(const SSL_DetectionRobot& robot,
                    int team);
    void processBall(const SSL_DetectionBall &ball);
    Tracker* tracker_camera_0;

};

#endif // VISION_H
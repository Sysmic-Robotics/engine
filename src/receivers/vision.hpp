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
#include "kalman/robotkalman.hpp"
#include "kalman/robotstatscollector.hpp"


class Vision : public QObject{
    Q_OBJECT  // Add this macro to enable Qt's signal/slot mechanism
public:
    Vision(const QString &multicastAddress, int port, QObject *parent = nullptr);
    void startListen();

signals:
    void robotReceived(int id, int team, QVector2D position, float orientation);  // Signal to notify when packet is received
    void ballReceived(QVector2D position);

private:
    int selectedRobotId = 1;
    int selectedRobotTeam = 1;  // 0 = blue, 1 = yellow
    bool statsCollected = false;
    RobotStatsCollector* m_statsCollector = nullptr;
    QUdpSocket *m_udpSocket;
    QString m_multicastAddress;
    int m_port;
    QMap<std::pair<int, int>, RobotKalman*> robotKalmanFilters;
    void readPendingDatagrams();
    void deserializePacket(const QByteArray &datagram);
    void processData(const SSL_DetectionRobot& robot,
                    int team,
                    bool useKalman,
                    QMap<std::pair<int, int>, RobotKalman*>& filters);

};

#endif // VISION_H
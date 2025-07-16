// vision.cpp modificado para aplicar filtro de Kalman condicionalmente con una flag

#include "vision.hpp"
#include <QDebug>
#include <QHash>



Vision::Vision(const QString &multicastAddress, int port, QObject *parent)
    : QObject(parent), m_multicastAddress(multicastAddress), m_port(port), m_udpSocket(nullptr)
{

    tracker_camera_0 = new Tracker();

}

void Vision::startListen(){
    m_udpSocket = new QUdpSocket(this);

    if (!m_udpSocket)
    {
        qCritical() << "Failed to create UDP socket.";
        return;
    }

    if (!m_udpSocket->bind(QHostAddress::AnyIPv4, m_port,
                         QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))
    {
        qCritical() << "Failed to bind socket:" << m_udpSocket->errorString();
        return;
    }

    qInfo() << "Listening on port" << m_port << "...";

    QHostAddress groupAddress(m_multicastAddress);
    if (!m_udpSocket->joinMulticastGroup(groupAddress))
    {
        qCritical() << "Failed to join multicast group:" << m_udpSocket->errorString();
        return;
    }

    qInfo() << "Joined multicast group:" << m_multicastAddress;

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &Vision::readPendingDatagrams);
}

void Vision::readPendingDatagrams()
{
    while (m_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        deserializePacket(datagram);
    }
}

void Vision::deserializePacket(const QByteArray &datagram){
    SSL_WrapperPacket wrapperPacket;

    if (wrapperPacket.ParseFromArray(datagram.data(), datagram.size()))
    {
        if (wrapperPacket.has_detection())
        {
            const SSL_DetectionFrame &detectionFrame = wrapperPacket.detection();

            for (const SSL_DetectionBall &ball : detectionFrame.balls()){
                if (ball.x() >= 0) {
                    processBall(ball);
                }
                
                }
            
            for (const SSL_DetectionRobot &robot : detectionFrame.robots_yellow()){
                if (robot.x() >= 0) {
                    processData(robot, 1);
                }
            }
            for (const SSL_DetectionRobot &robot : detectionFrame.robots_blue()){
                if (robot.x() >= 0) {
                    processData(robot, 0);
                }
            }
        }
    }
    else
    {
        qWarning() << "Failed to parse the detection frame.";
    }
}

void Vision::processBall(const SSL_DetectionBall &ball){
    float x = ball.x() / 1000.0f;
    float y = ball.y() / 1000.0f;
    auto [xf, yf, thetaf, vx, vy, omega] = tracker_camera_0->track(-1, -1, x, y, 0, 0.016);
    emit ballReceived(QVector2D(xf, yf), QVector2D(vx, vy));
}

void Vision::processData(const SSL_DetectionRobot& robot, int team){
    int id = robot.robot_id();
    float x = robot.x() / 1000.0f;
    float y = robot.y() / 1000.0f;
    float theta = robot.orientation();
    auto [xf, yf, thetaf, vx, vy, omega] = tracker_camera_0->track(team, id, x, y, theta, 0.016);
    emit robotReceived(id, team, QVector2D(xf, yf), thetaf, QVector2D(vx,vy), omega);
}


// Copyright: Gerson Goncalves 2025
// Nota: Nunca me gusto hacer este codigo, me meti a la iniciativa para trabajar
// en las estrategias -_-

#include "vision.hpp"
#include <QDebug>

Vision::Vision(const QString &multicastAddress, int port, QObject *parent)
    : QObject(parent), m_multicastAddress(multicastAddress), m_port(port), m_udpSocket(nullptr)
{}


void Vision::startListen(){
    m_udpSocket = new QUdpSocket(this);

    // Check if the socket is created successfully
    if (!m_udpSocket)
    {
        qCritical() << "Failed to create UDP socket.";
        return;
    }

    // Bind the socket to listen on any IPv4 address and the specified port
    if (!m_udpSocket->bind(QHostAddress::AnyIPv4, m_port,
                         QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint))
    {
        qCritical() << "Failed to bind socket:" << m_udpSocket->errorString();
        return;
    }

    qInfo() << "Listening on port" << m_port << "...";

    // Join the multicast group
    QHostAddress groupAddress(m_multicastAddress);
    if (!m_udpSocket->joinMulticastGroup(groupAddress))
    {
        qCritical() << "Failed to join multicast group:" << m_udpSocket->errorString();
        return;
    }

    qInfo() << "Joined multicast group:" << m_multicastAddress;

    // Connect the readyRead signal to handle incoming data
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

            // Process detected balls
            for (const SSL_DetectionBall &ball : detectionFrame.balls()){
                emit ballReceived(QVector2D(ball.x()/1000, ball.y()/1000));
            }

            // Process yellow robots
            for (const auto &robot : detectionFrame.robots_yellow()){
                emit robotReceived(robot.robot_id(), 1, QVector2D(robot.x()/1000, robot.y()/1000), robot.orientation());
            }
            
            // Process blue robots
            for (const auto &robot : detectionFrame.robots_blue()){
                emit robotReceived(robot.robot_id(), 0, QVector2D(robot.x()/1000, robot.y()/1000), robot.orientation());
            }
        }
    }
    else
    {
        qWarning() << "Failed to parse the detection frame.";
    }
}
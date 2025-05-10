// vision.cpp modificado para aplicar filtro de Kalman condicionalmente con una flag

#include "Kalman/KalmanFilter.hpp"
#include "vision.hpp"
#include <QDebug>
#include <QHash>

// === FLAG DE FILTRADO ===
// Cambiar manualmente a false si no quieres usar el filtro de Kalman
constexpr bool USE_KALMAN_FILTER = false;

// Filtro de Kalman para la pelota
KalmanFilter ballFilter;

// Filtros por robot_id y por equipo (solo si se usa filtro)
QHash<int, KalmanFilter> yellowFilters;
QHash<int, KalmanFilter> blueFilters;

Vision::Vision(const QString &multicastAddress, int port, QObject *parent)
    : QObject(parent), m_multicastAddress(multicastAddress), m_port(port), m_udpSocket(nullptr)
{}

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
                float z[2] = {ball.x() / 1000.0f, ball.y() / 1000.0f};
                if (USE_KALMAN_FILTER) {
                    float filtered[2][2];
                    ballFilter.updatePosition(z, filtered);
                    float x = filtered[0][0];
                    float y = filtered[1][0];
                    emit ballReceived(QVector2D(x, y));
                } else {
                    emit ballReceived(QVector2D(z[0], z[1]));
                }
            }

            for (const auto &robot : detectionFrame.robots_yellow()){
                int id = robot.robot_id();
                float z[2] = {robot.x()/1000.0f, robot.y()/1000.0f};

                if (USE_KALMAN_FILTER) {
                    if (!yellowFilters.contains(id)) {
                        yellowFilters[id] = KalmanFilter();
                    }
                    float filtered[2][2];
                    yellowFilters[id].updatePosition(z, filtered);
                    emit robotReceived(id, 1, QVector2D(filtered[0][0], filtered[1][0]), robot.orientation());
                } else {
                    emit robotReceived(id, 1, QVector2D(z[0], z[1]), robot.orientation());
                }
            }

            for (const auto &robot : detectionFrame.robots_blue()){
                int id = robot.robot_id();
                float z[2] = {robot.x()/1000.0f, robot.y()/1000.0f};

                if (USE_KALMAN_FILTER) {
                    if (!blueFilters.contains(id)) {
                        blueFilters[id] = KalmanFilter();
                    }
                    float filtered[2][2];
                    blueFilters[id].updatePosition(z, filtered);
                    emit robotReceived(id, 0, QVector2D(filtered[0][0], filtered[1][0]), robot.orientation());
                } else {
                    emit robotReceived(id, 0, QVector2D(z[0], z[1]), robot.orientation());
                }
            }
        }
    }
    else
    {
        qWarning() << "Failed to parse the detection frame.";
    }
}

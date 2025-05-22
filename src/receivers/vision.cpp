// vision.cpp modificado para aplicar filtro de Kalman condicionalmente con una flag

#include "vision.hpp"
#include <QDebug>
#include <QHash>

constexpr bool USE_KALMAN_FILTER = true;

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
                emit ballReceived(QVector2D(ball.x() / 1000.0f, ball.y() / 1000.0f));
                }
            
            for (const SSL_DetectionRobot &robot : detectionFrame.robots_yellow()){
                processData(robot, 1, USE_KALMAN_FILTER, robotKalmanFilters);
                
            }
            for (const SSL_DetectionRobot &robot : detectionFrame.robots_blue()){
                processData(robot, 0, USE_KALMAN_FILTER, robotKalmanFilters);
            }
        }
    }
    else
    {
        qWarning() << "Failed to parse the detection frame.";
    }
}

void Vision::processData(const SSL_DetectionRobot& robot,
                         int team,
                         bool useKalman,
                         QMap<std::pair<int, int>, RobotKalman*>& filters)
{
    int id = robot.robot_id();
    float x = robot.x() / 1000.0f;
    float y = robot.y() / 1000.0f;
    float theta = robot.orientation();

    std::pair<int, int> key = {team, id};

    QVector2D position(x, y);

    // 👉 Stats collection for selected robot only
    if (!statsCollected && team == 0 && id == 0) {
        float dt = 0.016f; // Approx. 60Hz

        if (m_statsCollector == nullptr) {
            m_statsCollector = new RobotStatsCollector(id, team, 10.0);
        }

        m_statsCollector->update(x, y, theta, dt);

        if (m_statsCollector->isReady()) {
            QVector3D std = m_statsCollector->getStandardDeviations();
            qInfo() << "[STATS] Robot" << id << "Team" << team
                    << "o_x:" << std.x() << "o_y:" << std.y() << "o_θ:" << std.z();
            statsCollected = true;

            // Optional: cleanup
            delete m_statsCollector;
            m_statsCollector = nullptr;
        }
    }


    if (useKalman) {
        if (!filters.contains(key)) {
            filters[key] = new RobotKalman(0.016, 0.00184849*4, 0.0018652*4, 0.0018652*4, 1e-4);
            filters[key]->initialize(x, y, theta);
        }

        RobotKalman* kf = filters[key];
        kf->predict();
        kf->update(x, y, theta);

        QVector<double> state = kf->getState();
        position = QVector2D(state[0], state[2]);
        theta = static_cast<float>(state[4]);
    }
    
    emit robotReceived(id, team, position, theta);
}


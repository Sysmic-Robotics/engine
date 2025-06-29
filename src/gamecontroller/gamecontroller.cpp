#include "gamecontroller.hpp"
#include <QDebug>

GameController::GameController(QObject *parent) : QObject(parent) {}

GameController::~GameController() {
    if (m_udpSocket) {
        m_udpSocket->close();
        delete m_udpSocket;
    }
}

void GameController::startListen() {
    m_udpSocket = new QUdpSocket(this);

    quint16 port = 10003;  // Default SSL GameController port

    if (!m_udpSocket->bind(QHostAddress::AnyIPv4, port,
                           QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        qCritical() << "GameController: Failed to bind port:" << port << m_udpSocket->errorString();
        return;
    }

    qInfo() << "GameController: Listening on port" << port << "...";

    connect(m_udpSocket, &QUdpSocket::readyRead, this, &GameController::readPendingDatagrams);
}

void GameController::readPendingDatagrams() {
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());

        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        SSL_Referee referee;
        if (referee.ParseFromArray(datagram.constData(), datagram.size())) {
            m_lastReferee = referee;

            // DEBUG: Print basic info
            qDebug() << "[GameController] Stage:"
                     << referee.stage()
                     << ", Command:" << referee.command();

            emit refereeUpdated(referee);

        } else {
            qWarning() << "[GameController] Failed to parse SSL_Referee packet.";
        }
    }
}

SSL_Referee::Stage GameController::getStage() const {
    return m_lastReferee.stage();
}

SSL_Referee::Command GameController::getCommand() const {
    return m_lastReferee.command();
}

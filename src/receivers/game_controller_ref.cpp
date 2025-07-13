#include "game_controller_ref.hpp"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/text_format.h>
#include <cmath>
#include <iostream>

GameControllerRef::GameControllerRef(const QString& multicastAddress, quint16 port, QObject* parent)
    : QObject(parent), m_multicastAddress(multicastAddress), m_port(port){
    connect(&m_socket, &QUdpSocket::readyRead, this, &GameControllerRef::onReadyRead);
}

bool GameControllerRef::startListen(){

    if (!m_socket.bind(QHostAddress::AnyIPv4, m_port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        qWarning() << "Failed to bind UDP socket:" << m_socket.errorString();
        return false;
    }

    if (!m_socket.joinMulticastGroup(m_multicastAddress)) {
        qWarning() << "Failed to join multicast group:" << m_multicastAddress.toString();
        return false;
    }
    

    qDebug() << "[✔] Listening to referee messages on" << m_multicastAddress << ":" << m_port;
    return true;
}

void GameControllerRef::stop()
{
    m_socket.leaveMulticastGroup(m_multicastAddress);
    m_socket.close();
    qDebug() << "[GameControllerRef] Stopped listening.";
}

void GameControllerRef::onReadyRead()
{
    while (m_socket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_socket.pendingDatagramSize());
        m_socket.readDatagram(datagram.data(), datagram.size());

        processMessage(datagram);
    }
}

void GameControllerRef::processMessage(const QByteArray& data)
{
    Referee refMsg;
    if (!refMsg.ParseFromArray(data.constData(), data.size())) {
        qWarning() << "Failed to parse referee message";
        return;
    }
    emit onRefCommand(commandToString(refMsg.command()));
}

QString GameControllerRef::commandToString(Referee::Command cmd) {
    switch (cmd) {
        case Referee::HALT: return "HALT";
        case Referee::STOP: return "STOP";
        case Referee::NORMAL_START: return "NORMAL_START";
        case Referee::FORCE_START: return "FORCE_START";
        case Referee::PREPARE_KICKOFF_YELLOW: return "PREPARE_KICKOFF_YELLOW";
        case Referee::PREPARE_KICKOFF_BLUE: return "PREPARE_KICKOFF_BLUE";
        case Referee::PREPARE_PENALTY_YELLOW: return "PREPARE_PENALTY_YELLOW";
        case Referee::PREPARE_PENALTY_BLUE: return "PREPARE_PENALTY_BLUE";
        case Referee::DIRECT_FREE_YELLOW: return "DIRECT_FREE_YELLOW";
        case Referee::DIRECT_FREE_BLUE: return "DIRECT_FREE_BLUE";
        case Referee::TIMEOUT_YELLOW: return "TIMEOUT_YELLOW";
        case Referee::TIMEOUT_BLUE: return "TIMEOUT_BLUE";
        case Referee::GOAL_YELLOW: return "GOAL_YELLOW";
        case Referee::GOAL_BLUE: return "GOAL_BLUE";
        case Referee::BALL_PLACEMENT_YELLOW: return "BALL_PLACEMENT_YELLOW";
        case Referee::BALL_PLACEMENT_BLUE: return "BALL_PLACEMENT_BLUE";
        default: return "UNKNOWN";
    }
}


void GameControllerRef::printJSON(const Referee& refMsg)
{
    std::string json;
    google::protobuf::util::JsonPrintOptions options;
    options.add_whitespace = true;
    auto status = google::protobuf::util::MessageToJsonString(refMsg, &json, options);

    if (status.ok()) {
        qDebug().noquote() << QString::fromStdString(json);
    } else {
        qWarning() << "Failed to convert referee message to JSON";
    }
}
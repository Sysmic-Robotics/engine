#include "game_controller_ref.hpp"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <google/protobuf/util/json_util.h>
#include <google/protobuf/text_format.h>
#include <cmath>
#include <iostream>

GameControllerRef::GameControllerRef(QObject* parent)
    : QObject(parent)
{
    connect(&m_socket, &QUdpSocket::readyRead, this, &GameControllerRef::onReadyRead);
}

bool GameControllerRef::start(const QString& multicastAddress, quint16 port, bool fullScreen, bool verbose)
{
    m_multicastAddress = QHostAddress(multicastAddress);
    m_port = port;
    m_fullScreen = fullScreen;
    m_verbose = verbose;

    if (!m_socket.bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
        qWarning() << "Failed to bind UDP socket:" << m_socket.errorString();
        return false;
    }

    if (!m_socket.joinMulticastGroup(m_multicastAddress)) {
        qWarning() << "Failed to join multicast group:" << m_multicastAddress.toString();
        return false;
    }

    qDebug() << "[✔] Listening to referee messages on" << multicastAddress << ":" << port;
    return true;
}

void GameControllerRef::stop()
{
    m_socket.leaveMulticastGroup(m_multicastAddress);
    m_socket.close();
    qDebug() << "[✋] Stopped listening.";
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

    // Only keep recent changes
    if (m_history.isEmpty() || refMsg.command() != m_history.last()) {
        m_history.append(refMsg.command());
    }

    if (m_fullScreen) {
        printFullScreen(refMsg);
    } else {
        printJSON(refMsg);
    }
}

void GameControllerRef::printFullScreen(const Referee& refMsg)
{
    // Clear console (ANSI escape)
    std::cout << "\033[H\033[2J";
    std::cout.flush();

    std::cout << "Last commands: ";
    int n = qMin(5, m_history.size());
    for (int i = 0; i < n; ++i) {
        std::cout << Referee::Command_Name(m_history[m_history.size() - 1 - i]);
        if (i != n - 1) std::cout << ", ";
    }
    std::cout << "\n\n";

    std::string textFormat;
    google::protobuf::TextFormat::Printer printer;
    printer.SetSingleLineMode(false);
    printer.PrintToString(refMsg, &textFormat);
    std::cout << textFormat << std::endl;
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

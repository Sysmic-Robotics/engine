#include "game_controller_socket.hpp"
#include <QDebug>
#include <google/protobuf/util/delimited_message_util.h>

using namespace google::protobuf::io;
using namespace google::protobuf::util;

GameControllerSocket::GameControllerSocket(const QString& host, quint16 port, QObject* parent)
    : QObject(parent),
      m_socket(this),
      m_host(QHostAddress(host)),
      m_port(port)
{
    connect(&m_socket, &QTcpSocket::readyRead, this, &GameControllerSocket::onReadyRead);
}

bool GameControllerSocket::connectToController() {
    if (m_socket.state() == QAbstractSocket::ConnectedState)
        return true;

    m_socket.connectToHost(m_host, m_port);
    if (!m_socket.waitForConnected(1000)) {
        qWarning() << "Connection failed:" << m_socket.errorString();
        return false;
    }

    qDebug() << "Connected to GameController at" << m_host.toString() << ":" << m_port;
    emit connected();
    return true;
}

void GameControllerSocket::close() {
    m_socket.disconnectFromHost();
    if (m_socket.state() != QAbstractSocket::UnconnectedState)
        m_socket.waitForDisconnected();
}

void GameControllerSocket::sendRegistration(const QString& teamName, const QString& teamColor) {
    // Expect a message from the controller with token
    ControllerToTeam initReply;
    if (!receiveDelimitedMessage(initReply)) {
        qDebug() << "Failed to receive initial message from controller";
        emit registrationFailed("No token");
        return;
    }

    if (!initReply.has_controller_reply() || !initReply.controller_reply().has_next_token()) {
        emit registrationFailed("Initial reply missing token");
        return;
    }

    m_token = initReply.controller_reply().next_token();

    // --- Construct registration message ---
    TeamRegistration registration;
    registration.set_team_name(teamName.toStdString());

    // Set team color if given (YELLOW/BLUE)
    if (teamColor.toUpper() == "YELLOW") {
        registration.set_team(Team::YELLOW);
    } else if (teamColor.toUpper() == "BLUE") {
        registration.set_team(Team::BLUE);
    }

    // Signature (token only for now)
    //Signature* sig = registration.mutable_signature();
    //sig->set_token(m_token);
    // If signing: sig->set_pkcs1v15(Sign(...))

    if (!sendDelimitedMessage(registration)) {
        emit registrationFailed("Send failed");
        return;
    }

    // Receive reply from GC
    ControllerToTeam response;
    if (!receiveDelimitedMessage(response)) {
        emit registrationFailed("No reply from controller");
        return;
    }

    const auto& reply = response.controller_reply();
    if (!reply.has_status_code() || reply.status_code() != ControllerReply::OK) {
        emit registrationFailed(QString::fromStdString(reply.reason()));
        return;
    }

    if (reply.has_next_token()) {
        m_token = reply.next_token();
    }

    emit registrationSuccess();
}


void GameControllerSocket::sendAdvantageChoice(AdvantageChoice choice) {
    TeamToController message;

    // oneof msg: use set_advantage_choice
    message.set_advantage_choice(choice);

    // Signature with token
    if (!m_token.empty()) {
        Signature* sig = message.mutable_signature();
        sig->set_token(m_token);
        // sig->set_pkcs1v15(...); // if using private key signing
    }

    if (!sendDelimitedMessage(message)) {
        qWarning() << "Failed to send advantage choice";
    }
}


void GameControllerSocket::onReadyRead() {
    ControllerToTeam reply;
    if (receiveDelimitedMessage(reply)) {
        emit messageReceived(reply);
    }
}

bool GameControllerSocket::sendDelimitedMessage(const google::protobuf::Message& msg) {
    QByteArray buffer;

    // Reserve enough space for message
    buffer.resize(msg.ByteSizeLong() + 10);  // extra space for delimiter

    google::protobuf::io::ArrayOutputStream arrayOut(buffer.data(), buffer.size());

    if (!google::protobuf::util::SerializeDelimitedToZeroCopyStream(msg, &arrayOut)) {
        qWarning() << "Failed to serialize message";
        return false;
    }

    // Resize buffer to actual used size
    buffer.resize(arrayOut.ByteCount());

    m_socket.write(buffer);
    m_socket.flush();
    return true;
}


bool GameControllerSocket::receiveDelimitedMessage(ControllerToTeam& msg) {
    if (m_socket.bytesAvailable() < 2)
        return false;

    QByteArray rawData = m_socket.readAll();

    google::protobuf::io::ArrayInputStream arrayIn(rawData.constData(), rawData.size());

    bool cleanEof = false;

    // ✅ Use ArrayInputStream directly
    if (!google::protobuf::util::ParseDelimitedFromZeroCopyStream(&msg, &arrayIn, &cleanEof)) {
        qWarning() << "Failed to parse delimited protobuf message";
        return false;
    }

    return true;
}


#pragma once

#include <QTcpSocket>
#include <QObject>
#include <QHostAddress>
#include <QTimer>

#include "../protobuf/ssl_gc_rcon_team.pb.h"
#include "../protobuf/ssl_gc_rcon.pb.h"
#include "../protobuf/ssl_gc_common.pb.h"

class GameControllerSocket : public QObject {
    Q_OBJECT

public:
    explicit GameControllerSocket(const QString& host, quint16 port, QObject* parent = nullptr);

    bool connectToController();
    void close();

    void sendRegistration(const QString& teamName, const QString& teamColor);
    
    void sendAdvantageChoice(AdvantageChoice choice);

signals:
    void connected();
    void registrationSuccess();
    void registrationFailed(const QString& reason);
    void messageReceived(const ControllerToTeam& message);

private slots:
    void onReadyRead();

private:
    bool sendDelimitedMessage(const google::protobuf::Message& msg);
    bool receiveDelimitedMessage(ControllerToTeam& msg);

    QTcpSocket m_socket;
    QHostAddress m_host;
    quint16 m_port;
    std::string m_token;
};

/*
        m_game_controller_socket = new GameControllerSocket("127.0.0.1", 10008);
        m_game_controller_socket->connectToController();

        // --- Handle connection ---
        QObject::connect(m_game_controller_socket, &GameControllerSocket::connected, [=]()
                         {
                             qDebug() << "[✔] Connected to GameController.";

                             // Send registration immediately after connection
                             m_game_controller_socket->sendRegistration("Test Team", "YELLOW"); // Replace with your actual team name and color
                         });

        // --- Handle successful registration ---
        QObject::connect(m_game_controller_socket, &GameControllerSocket::registrationSuccess, [=]()
                         {
                             qDebug() << "[✔] Registered with GameController.";

                             // Start sending AdvantageChoice messages every 5 seconds
                             QTimer *timer = new QTimer(m_game_controller_socket);
                             QObject::connect(timer, &QTimer::timeout, [=]()
                                              {
            static bool toggle = false;
            m_game_controller_socket->sendAdvantageChoice(toggle ? CONTINUE : STOP);
            qDebug() << "→ Sent advantage choice:" << (toggle ? "CONTINUE" : "STOP");
            toggle = !toggle; });
                             timer->start(5000); // every 5 seconds
                         });

        // --- Handle registration failure ---
        QObject::connect(m_game_controller_socket, &GameControllerSocket::registrationFailed, [=](const QString &reason)
                         { qDebug() << "[✘] Registration failed:" << reason; });

        // --- Handle incoming messages (ControllerReply, etc.) ---
        QObject::connect(m_game_controller_socket, &GameControllerSocket::messageReceived, [=](const ControllerToTeam &message)
                         {
            if (message.has_controller_reply()) {
                const auto& reply = message.controller_reply();
                QString statusText = (reply.status_code() == 0) ? "OK" : "FAILED";
                qDebug() << "[?] Controller reply:"
                        << "Status =" << statusText
                        << ", Reason =" << QString::fromStdString(reply.reason());
                            } else {
                qDebug() << "[↩] Unknown message from GameController.";
            } });

        // Start connection attempt
        if (!m_game_controller_socket->connectToController())
        {
            qDebug() << "✘ Could not connect to GameController.";
        }*/
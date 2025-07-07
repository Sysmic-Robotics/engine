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

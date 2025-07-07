#pragma once

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QVector>
#include <QTimer>
#include <memory>
#include "../protobuf/ssl_gc_referee_message.pb.h"

class GameControllerRef : public QObject {
    Q_OBJECT

public:
    explicit GameControllerRef(QObject* parent = nullptr);
    bool start(const QString& multicastAddress = "224.5.23.1", quint16 port = 10003, bool fullScreen = false, bool verbose = false);
    void stop();

private slots:
    void onReadyRead();

private:
    void processMessage(const QByteArray& data);
    void printFullScreen(const Referee& refMsg);
    void printJSON(const Referee& refMsg);

    QUdpSocket m_socket;
    QHostAddress m_multicastAddress;
    quint16 m_port;
    bool m_fullScreen = false;
    bool m_verbose = false;

    QVector<Referee::Command> m_history;
};

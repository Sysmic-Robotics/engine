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
    explicit GameControllerRef(const QString& multicastAddress, quint16 port, QObject* parent = nullptr);
    bool startListen();
    void stop();

private slots:
    void onReadyRead();

signals:
    void onRefCommand(const QString& state);

private:
    QString commandToString(Referee::Command cmd);
    void processMessage(const QByteArray& data);
    void printJSON(const Referee& refMsg);

    QUdpSocket m_socket;
    QHostAddress m_multicastAddress;
    quint16 m_port;
    bool m_fullScreen = false;
    bool m_verbose = false;

    QVector<Referee::Command> m_history;
};

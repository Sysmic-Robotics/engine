#pragma once

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include "luainterface.hpp"
#include "radio.hpp"
#include "motioncommand.hpp"
#include "kickercommand.hpp"

class WebSocketServer : public QObject {
    Q_OBJECT

public:
    explicit WebSocketServer(Radio* _radio, LuaInterface* _luaInterface, QObject *parent = nullptr);
    ~WebSocketServer();

    void broadcast(const QJsonObject &message);

private slots:
    void onNewConnection();
    void onDisconnected();
    void onTextMessageReceived(const QString &message);

private:
    QWebSocketServer *server;
    QList<QWebSocket *> clients;
    LuaInterface *luaInterface;
    Radio *m_radio;
};

#pragma once

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include "luainterface.hpp"

class WebSocketServer : public QObject {
    Q_OBJECT

public:
    explicit WebSocketServer(LuaInterface* _luaInterface, QObject *parent = nullptr);
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
};

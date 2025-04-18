#include "WebSocketServer.hpp"

WebSocketServer::WebSocketServer(LuaInterface* _luaInterface, QObject *parent)
    : luaInterface(_luaInterface), QObject(parent),
      server(new QWebSocketServer(QStringLiteral("SSL Server"),
                                  QWebSocketServer::NonSecureMode,
                                  this)) {
    if (server->listen(QHostAddress::Any, 9001)) {
        connect(server, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        qDebug() << "[WebSocket] Listening on port" << server->serverPort();
    } else {
        qDebug() << "[WebSocket] Failed to listen on port 9001";
    }
}

WebSocketServer::~WebSocketServer() {
    server->close();
    qDeleteAll(clients);
}

void WebSocketServer::onNewConnection() {
    QWebSocket *client = server->nextPendingConnection();
    connect(client, &QWebSocket::disconnected, this, &WebSocketServer::onDisconnected);
    clients.append(client);
    qDebug() << "[WebSocket] Client connected";

    connect(client, &QWebSocket::textMessageReceived,
        this, &WebSocketServer::onTextMessageReceived);
    connect(client, &QWebSocket::disconnected,
            this, &WebSocketServer::onDisconnected);
}

void WebSocketServer::onDisconnected() {
    QWebSocket *client = qobject_cast<QWebSocket *>(sender());
    if (client) {
        clients.removeAll(client);
        client->deleteLater();
        qDebug() << "[WebSocket] Client disconnected";
    }
}

void WebSocketServer::broadcast(const QJsonObject &message) {
    QJsonDocument doc(message);
    QString jsonString = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    for (QWebSocket *client : std::as_const(clients)) {
        if (client->isValid()) {
            client->sendTextMessage(jsonString);
        }
    }
}


void WebSocketServer::onTextMessageReceived(const QString &message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    QString type = obj.value("type").toString();

    if (type == "runScript" && luaInterface) {
        luaInterface->runScript();  // Trigger script!
    }
}
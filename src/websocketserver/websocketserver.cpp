#include "websocketserver.hpp"

WebSocketServer::WebSocketServer(Radio* _radio, LuaInterface* _luaInterface, QObject *parent)
    : m_radio(_radio), luaInterface(_luaInterface), QObject(parent),
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
        QString scriptPath = obj.value("path").toString(); // Extract the script path
        if (!scriptPath.isEmpty()) {
            luaInterface->runScript(scriptPath);  // Run the specified script
        } else {
            qWarning() << "runScript requested but no path provided.";
        }
    }
    else if (type == "pauseScript") {
        luaInterface->pauseScript(); // implement pause behavior in LuaInterface
        qDebug() << "Script paused";
    }
else if (type == "joystickCommand") {
    int id = obj.value("id").toInt(-1);
    int team = obj.value("team").toInt(-1);
    double vx = obj.value("vx").toDouble(0.0);
    double vy = obj.value("vy").toDouble(0.0);
    double omega = obj.value("omega").toDouble(0.0);
    bool kick = obj.value("kick").toBool(false);
    double dribbler = obj.value("dribbler").toDouble(0.0); // ✅ New field

    if (id < 0 || team < 0) {
        qWarning() << "Invalid joystickCommand: missing id or team";
        return;
    }

    // Create and send motion command
    MotionCommand cmd(id, team, vx, vy);
    cmd.setAngular(omega);

    KickerCommand kicker(id, team);
    kicker.setKickX(kick);
    kicker.setDribbler(dribbler); // ✅ Apply dribbler

    if (m_radio) {
        m_radio->addMotionCommand(cmd);
        m_radio->addKickerCommand(kicker);
    }
}

}





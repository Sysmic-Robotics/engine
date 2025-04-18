#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include "vision.hpp"
#include "world.hpp"
#include "radio.hpp"
#include "luainterface.hpp"
#include "websocketserver.hpp"
#include <QElapsedTimer>
#include <QDebug>
#include <QJsonObject>

class MainApp : public QObject {
    Q_OBJECT

public:
    MainApp() {
        // Setup threads and vision
        m_visionThread = new QThread(this);
        m_vision = new Vision("224.5.23.2", 10020);
        m_vision->moveToThread(m_visionThread);

        m_worldThread = new QThread(this);
        m_world = new World(4, 4);
        m_world->moveToThread(m_worldThread);

        // Setup Lua + WebSocket
        radio = new Radio();
        luaInterface = new LuaInterface(radio, m_world);
        m_webSocketServer = new WebSocketServer(luaInterface, this);

        // Connect vision to world
        connect(m_visionThread, &QThread::started, m_vision, &Vision::startListen);
        connect(m_vision, &Vision::robotReceived, m_world, &World::updateRobot);
        connect(m_vision, &Vision::ballReceived, m_world, &World::updateBall);

        // Start threads and update loop
        m_visionThread->start();
        m_worldThread->start();

        m_updateTimer = new QTimer(this);
        connect(m_updateTimer, &QTimer::timeout, this, &MainApp::update);
        m_updateTimer->start(16); // ~60 FPS
    }

    ~MainApp() {
        if (m_visionThread->isRunning()) {
            m_visionThread->quit();
            m_visionThread->wait();
        }
        m_vision->deleteLater();

        if (m_worldThread->isRunning()) {
            m_worldThread->quit();
            m_worldThread->wait();
        }
        m_world->deleteLater();

        delete radio;
        delete luaInterface;
    }

private slots:
    void update() {
        QElapsedTimer timer;
        timer.start();

        m_world->update();

        if (luaInterface->haveScript()) {
            luaInterface->callProcess();
        }

        radio->sendCommands();

        QJsonObject worldState = m_world->toJson();

        QJsonObject metrics;
        metrics["updateTimeUs"] = static_cast<int>(timer.nsecsElapsed() / 1000);
        worldState["metrics"] = metrics;

        m_webSocketServer->broadcast(worldState);
    }

private:
    QThread *m_visionThread;
    Vision *m_vision;
    QThread *m_worldThread;
    World *m_world;
    WebSocketServer *m_webSocketServer;
    QTimer *m_updateTimer;
    Radio *radio;
    LuaInterface *luaInterface;
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    MainApp app;
    return a.exec();
}

#include "main.moc"

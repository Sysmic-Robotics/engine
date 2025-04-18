#include <QApplication>
#include <QThread>
#include <QTimer>
#include "vision.hpp"
#include "world.hpp"
#include "radio.hpp"
#include "luainterface.hpp"
#include "process.hpp"
#include "websocketserver.hpp"
#include <QElapsedTimer>
#include <QDebug>
#include <QJsonObject>


class MainApp : public QObject {
    Q_OBJECT

public:
    MainApp() {
        initThreads();
        initLuaInterface();
        initConnections();
        initProcess();
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
        delete process;
    }

private slots:
void update() {
    QElapsedTimer timer;
    timer.start();

    m_world->update();
    process->update();
    radio->sendCommands();

    QJsonObject worldState = m_world->toJson();

    // Add performance metrics
    qint64 elapsedMicroseconds = timer.nsecsElapsed() / 1000;
    QJsonObject metrics;
    metrics["updateTimeUs"] = static_cast<int>(elapsedMicroseconds);  // safe to cast, value is small

    worldState["metrics"] = metrics;

    m_webSocketServer->broadcast(worldState);
}



private:
    void initThreads() {
        m_visionThread = new QThread(this);
        m_vision = new Vision("224.5.23.2", 10020);
        m_vision->moveToThread(m_visionThread);

        m_worldThread = new QThread(this);
        m_world = new World(4, 4);
        m_world->moveToThread(m_worldThread);
    }

    void initConnections() {
        connect(m_visionThread, &QThread::started, m_vision, &Vision::startListen);
        connect(m_vision, &Vision::robotReceived, m_world, &World::updateRobot);
        connect(m_vision, &Vision::ballReceived, m_world, &World::updateBall);
    }

    void initLuaInterface() {
        radio = new Radio();
        luaInterface = new LuaInterface(radio, m_world);
        
        m_webSocketServer = new WebSocketServer(luaInterface ,this);
    }

    void initProcess() {
        process = new Process(m_world, radio, luaInterface, this);

        m_visionThread->start();
        m_worldThread->start();

        m_updateTimer = new QTimer(this);
        connect(m_updateTimer, &QTimer::timeout, this, &MainApp::update);
        m_updateTimer->start(16);
    }

    // Member variables
    QThread *m_visionThread;
    Vision *m_vision;
    QThread *m_worldThread;
    World *m_world;

    WebSocketServer *m_webSocketServer;

    QTimer *m_updateTimer;

    Radio *radio;
    LuaInterface *luaInterface;
    Process *process;
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainApp app;
    return a.exec();
}

#include "main.moc"

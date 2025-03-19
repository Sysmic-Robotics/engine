#include <QApplication>
#include <QThread>
#include <QTimer>
#include "vision.hpp"
#include "world.hpp"
#include "mainwindow.hpp"
#include "radio.hpp"
#include "luainterface.hpp"
#include "process.hpp"

class MainApp : public QObject {
    Q_OBJECT

public:
    MainApp() {
        initThreads();
        initGui();
        initConnections();
        initLuaInterface();
        initprocess();
        initProcess();
    }

    ~MainApp() {
        // Stop the Vision thread properly.
        if (m_visionThread->isRunning()) {
            m_visionThread->quit();
            m_visionThread->wait();
        }
        m_vision->deleteLater();

        // Stop the World thread properly.
        if (m_worldThread->isRunning()) {
            m_worldThread->quit();
            m_worldThread->wait();
        }
        m_world->deleteLater();

        // m_mainWindow and m_updateTimer are children of MainApp and are deleted automatically.
        // Explicitly delete Radio, LuaInterface, and the process.
        delete radio;
        delete luaInterface;
        delete process;
    }

private slots:
    // Main update cycle.
    void update() {
        // Update world simulation.
        m_world->update();

        // Delegate business logic (robot control decision making) to the process.
        process->update();

        // Finally, send accumulated motion commands via the radio.
        radio->sendCommands();
    }

    // UI event handlers which forward events to the process.
    void onFaceToDebug(QVector2D point) {
        process->setFaceToTarget(point);
    }

    void onRobotSelected(int id, int team) {
        process->setRobotSelected(id, team);
    }

    void onSetRobotControl(bool flag) {
        process->setDebugControl(flag);
    }

    void onTargetPointSelected(QVector2D point) {
        process->setTargetPoint(point);
    }

    void onScriptRunRequested() {
        luaInterface->runScript("/home/gerson/Sysmic/CondorSSL/src/luainterface/script.lua");
    }

private:
    // Initialization helpers:

    void initThreads() {
        // Initialize Vision thread and object.
        m_visionThread = new QThread(this);
        m_vision = new Vision("224.5.23.2", 10020);
        m_vision->moveToThread(m_visionThread);

        // Initialize World thread and object.
        m_worldThread = new QThread(this);
        m_world = new World(4, 4);
        m_world->moveToThread(m_worldThread);
    }

    void initGui() {
        // Create and show the main GUI window (with MainApp as its parent).
        m_mainWindow = new MainWindow();
        m_mainWindow->show();
    }

    void initConnections() {
        // Connect MainWindow signals to MainApp slots.
        connect(m_mainWindow, &MainWindow::robotSelected, this, &MainApp::onRobotSelected);
        connect(m_mainWindow, &MainWindow::targetPointSelected, this, &MainApp::onTargetPointSelected);
        connect(m_mainWindow, &MainWindow::faceToDebug, this, &MainApp::onFaceToDebug);
        connect(m_mainWindow, &MainWindow::setRobotControl, this, &MainApp::onSetRobotControl);
        connect(m_mainWindow, &MainWindow::scriptRunRequested, this, &MainApp::onScriptRunRequested);

        // Connect Vision signals to World slots.
        connect(m_visionThread, &QThread::started, m_vision, &Vision::startListen);
        connect(m_vision, &Vision::robotReceived, m_world, &World::updateRobot);
        connect(m_vision, &Vision::ballReceived, m_world, &World::updateBall);

        // Connect World signals to MainWindow slots.
        connect(m_world, &World::robotUpdated, m_mainWindow, &MainWindow::updateRobot);
        connect(m_world, &World::ballUpdated, m_mainWindow, &MainWindow::updateBall);
    }

    void initLuaInterface() {
        // Initialize Radio and LuaInterface.
        radio = new Radio();
        luaInterface = new LuaInterface(radio, m_world);
    }

    void initprocess() {
        // Create the Robotprocess, providing it with pointers to the World, Radio, and LuaInterface.
        process = new Process(m_world, radio, luaInterface, this);
    }

    void initProcess() {
        // Start the Vision and World threads.
        m_visionThread->start();
        m_worldThread->start();

        // Setup a timer to call update() at approximately 60 FPS (every 16 ms).
        m_updateTimer = new QTimer(this);
        connect(m_updateTimer, &QTimer::timeout, this, &MainApp::update);
        m_updateTimer->start(16);
    }

    // Member variables:
    QThread *m_visionThread;
    Vision *m_vision;
    QThread *m_worldThread;
    World *m_world;

    MainWindow *m_mainWindow;
    QTimer *m_updateTimer;

    Radio *radio;
    LuaInterface *luaInterface;
    Process *process;
};

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainApp app;
    return a.exec();
}

#include "main.moc"

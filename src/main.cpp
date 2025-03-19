//#include <QApplication>
#include <QApplication>
#include <QThread>
#include <QTimer>
#include "vision.hpp"
#include "world.hpp"
#include "mainwindow.hpp"
#include "radio.hpp"
#include "motion.hpp"
#include "luainterface.hpp"

class MainApp : public QObject{
    Q_OBJECT

public:
    MainApp() {
        m_visionThread = new QThread(this);
        m_vision = new Vision("224.5.23.2", 10020); 
        m_vision->moveToThread(m_visionThread);

        m_worldThread = new QThread(this);
        m_world = new World(4,4);
        m_world->moveToThread(m_worldThread);
        
        // Create the GUI
        m_mainWindow = new MainWindow();
        m_mainWindow->show();
        connect(m_mainWindow, &MainWindow::robotSelected, this, &MainApp::onRobotSelected);
        connect(m_mainWindow, &MainWindow::targetPointSelected, this, &MainApp::onTargetPointSelected);
        connect(m_mainWindow, &MainWindow::faceToDebug, this, &MainApp::onFaceToDebug);
        connect(m_mainWindow, &MainWindow::setRobotControl, this, &MainApp::onSetRobotControl);
        // Connect vision to world
        connect(m_visionThread, &QThread::started, m_vision, &Vision::startListen);
        connect(m_vision, &Vision::robotReceived, m_world, &World::updateRobot);
        connect(m_vision, &Vision::ballReceived, m_world, &World::updateBall);

        // Connect world to GUI
        connect(m_world, &World::robotUpdated, m_mainWindow, &MainWindow::updateRobot);
        connect(m_world, &World::ballUpdated, m_mainWindow, &MainWindow::updateBall);


        // Start the thread
        m_visionThread->start();
        m_worldThread->start();

        // Setup lua interface
        radio = new Radio();
        luaInterface = new LuaInterface(radio, m_world);

        connect(m_mainWindow, &MainWindow::scriptLoaded, this, &MainApp::onScriptLoaded);
        connect(m_mainWindow, &MainWindow::scriptRunRequested, this, &MainApp::onScriptRunRequested);
        // Setup Timer for updateWorld()
        m_updateTimer = new QTimer(this);
        connect(m_updateTimer, &QTimer::timeout, this, &MainApp::update);
        m_updateTimer->start(16);  // Approx. 60 FPS (16ms per frame)
    }

    ~MainApp() {
        // Stop threads properly
        m_visionThread->quit();
        m_visionThread->wait();
        m_vision->deleteLater();

        m_worldThread->quit();
        m_worldThread->wait();
        m_world->deleteLater();

        //delete m_mainWindow;
        delete m_updateTimer;
        delete luaInterface;
        
    }

private slots:
    void update() {
        
        // Call the World update function
        m_world->update();

        // Get the robot's state from the world

        if(debug_control){ // On Robot Control Panel
            RobotState robotState = m_world->getRobotState(selectedRobotId, selectedTeam);

            static Motion motion;
            MotionCommand cmd(selectedRobotId, selectedTeam);
        
            if (faceToActive) {
                // Apply face_to if enabled
                cmd = motion.face_to(robotState, faceToTarget);
            } else {
                // Apply move_to command
                cmd = motion.to_point(robotState, targetPoint);
            }
            radio->addMotionCommand(cmd);
        }else{ // On Lua Interface Panel
            faceToActive = true;
            if (luaInterface->haveScript()){
                luaInterface->callProcess();
            }
        }

        radio->sendCommands();
    }

void onFaceToDebug(QVector2D point) {
    faceToTarget = point;
    faceToActive = true;  // Enable face_to control
}

void onRobotSelected(int id, int team) {
    selectedRobotId = id;
    selectedTeam = team;
}

void onSetRobotControl(bool flag){
    debug_control = flag;
}

void onTargetPointSelected(QVector2D point) {
    targetPoint = point;
    faceToActive = false;
}

void onScriptLoaded(QString filePath) {
    luaInterface->runScript("/home/gerson/Sysmic/CondorSSL/src/luainterface/script.lua");
}

void onScriptRunRequested() {
    luaInterface->runScript("/home/gerson/Sysmic/CondorSSL/src/luainterface/script.lua");
}

private:
    int selectedRobotId = -1;
    int selectedTeam = -1;
    QVector2D targetPoint;
    bool debug_control = true;

    QThread *m_visionThread;
    Vision *m_vision;
    Radio* radio;
    World *m_world;
    QThread *m_worldThread;
    QTimer *m_updateTimer;
    QVector2D faceToTarget;
    bool faceToActive = false;
    MainWindow *m_mainWindow;  // New GUI window
    LuaInterface* luaInterface;
    
};

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainApp app;
    return a.exec();
}

#include "main.moc"

//#include <QApplication>
#include <QApplication>
#include <QThread>
#include <QTimer>
#include "vision.hpp"
#include "world.hpp"
#include "mainwindow.hpp"
#include "radio.hpp"
#include "motion.hpp"
// Testing move skill
#include "move.hpp"

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
        
    }

private slots:
    void update() {
        // Call the World update function
        m_world->update();

        // Compute control commands based on path

        // Example: Assuming we're controlling a robot with ID 0 in the blue team (team = 0)
        int robotId = 1;
        int team = 0;

        // Get the robot's state from the world
        RobotState robotState = m_world->getRobotState(selectedRobotId, selectedTeam);

        // Compute the motion command using the robot's state
        static Move move;
        MotionCommand cmd = move.process(robotState, targetPoint);
        radio.appendCommand(cmd);

        // Send the computed command
        radio.sendCommands();
    }

void onRobotSelected(int id, int team) {
    selectedRobotId = id;
    selectedTeam = team;
}

void onTargetPointSelected(QVector2D point) {
    targetPoint = point;
}

private:
    int selectedRobotId = -1;
    int selectedTeam = -1;
    QVector2D targetPoint;

    QThread *m_visionThread;
    Vision *m_vision;
    Radio radio;
    World *m_world;
    QThread *m_worldThread;
    QTimer *m_updateTimer;

    MainWindow *m_mainWindow;  // New GUI window
};

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainApp app;
    return a.exec();
}

#include "main.moc"

//#include <QApplication>
#include <QApplication>
#include <QThread>
#include <QTimer>
#include "vision.hpp"
#include "world.hpp"
#include "mainwindow.hpp"
#include "grsim.hpp"

class MainApp : public QObject{
    Q_OBJECT

public:
    MainApp() {
        m_visionThread = new QThread(this);
        m_vision = new Vision("224.5.23.2", 10020); 
        m_vision->moveToThread(m_visionThread);

        m_worldThread = new QThread(this);
        m_world = new World(1,1);
        m_world->moveToThread(m_visionThread);

        // Create the GUI
        m_mainWindow = new MainWindow();
        m_mainWindow->show();

        // Connect signals and slots
        connect(m_visionThread, &QThread::started, m_vision, &Vision::startListen);
        connect(m_vision, &Vision::robotReceived, m_world, &World::updateRobot);
        connect(m_world, &World::robotUpdated, m_mainWindow, &MainWindow::updateRobot); // NEW!
        
        // Start the thread
        m_visionThread->start();
        m_worldThread->start();

        // Setup Timer for updateWorld()
        m_updateTimer = new QTimer(this);
        connect(m_updateTimer, &QTimer::timeout, this, &MainApp::updateWorld);
        m_updateTimer->start(16);  // Approx. 60 FPS (16ms per frame)
    }

    ~MainApp() {
        // Stop threads properly
        m_visionThread->quit();
        m_visionThread->wait();
        delete m_vision;

        m_worldThread->quit();
        m_worldThread->wait();
        delete m_world;
        //delete m_mainWindow;
        delete m_updateTimer;
        
    }

private slots:
    void updateWorld() {
        // Call the World update function
        m_world->update();
        static Grsim grsim;
        grsim.communicate_grsim(1, 0, 1.0, 0.0, 0.0, 1.5, 0.0, 0, false);
    }

private:
    QThread *m_visionThread;
    Vision *m_vision;

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

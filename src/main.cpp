#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <QJsonObject>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include "vision.hpp"
#include "world.hpp"
#include "radio.hpp"
#include "luainterface.hpp"
#include "websocketserver.hpp"
#include "consolereader.hpp"
#include "logger.hpp"

class MainApp : public QObject {
    Q_OBJECT

public:
    MainApp(QObject* parent = nullptr) : QObject(parent) {
        // Leer configuración desde config.ini
        QString configPath = QFileInfo(QCoreApplication::applicationDirPath() + "/../config.ini").absoluteFilePath();
        QSettings settings(configPath, QSettings::IniFormat);

        // Vision
        QString vision_ip = settings.value("Vision/ip_address", "224.5.23.2").toString();
        int vision_port = settings.value("Vision/port", 10020).toInt();
        // World
        int blue_team_size = settings.value("World/blue_team_size", 6).toInt();
        int yellow_team_size = settings.value("World/yellow_team_size", 6).toInt();
        // Performance
        int update_fps = settings.value("Performance/update_fps", 60).toInt();

        // Setup threads and vision
        m_visionThread = new QThread(this);
        m_vision = new Vision(vision_ip, vision_port);
        m_vision->moveToThread(m_visionThread);

        m_worldThread = new QThread(this);
        m_world = new World(blue_team_size, yellow_team_size);
        m_world->moveToThread(m_worldThread);

        // Setup Lua + WebSocket
        // Radio
        bool use_radio = settings.value("Radio/use_radio", true).toBool();
        QString radio_port = settings.value("Radio/port_name", "/dev/ttyUSB0").toString();
        qint32 radio_baud = settings.value("Radio/baud_rate", QSerialPort::Baud115200).toInt();

        // Create Radio and LuaInterface
        radio = new Radio(use_radio, radio_port, radio_baud);
        luaInterface = new LuaInterface(radio, m_world);
        m_webSocketServer = new WebSocketServer(radio, luaInterface, this);

        // Setup console reader
        m_consoleReader = new ConsoleReader(luaInterface);
        m_consoleReader->start();

        logger = new Logger(m_world, radio);
        // Connect to console commands
        connect(m_consoleReader, &ConsoleReader::startRecording,
                logger, &Logger::startLogging);
        connect(m_consoleReader, &ConsoleReader::stopRecording,
                logger, &Logger::stopLogging);
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
        // Cleanly stop threads and components
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

        if (m_consoleReader->isRunning()) {
            m_consoleReader->requestInterruption();  // Signal for graceful stop
            m_consoleReader->wait();
        }
        delete m_consoleReader;

        delete radio;
        delete luaInterface;
    }

    void runScript(const QString& scriptPath) {

        luaInterface->runScript(scriptPath);  // Or whatever method returns status

    }

private slots:
void update() {
    QElapsedTimer timer;
    timer.start();

    m_world->update();

    qint64 processTimeUs = 0;
    luaInterface->callProcess();

    logger->logFrame();
    radio->sendCommands();

    QJsonObject worldState = m_world->toJson();

    QJsonObject metrics;
    qint64 updateTimeUs = timer.nsecsElapsed() / 1000;
    metrics["updateTimeUs"] = static_cast<int>(updateTimeUs);
    metrics["processTimeUs"] = static_cast<int>(processTimeUs);
    worldState["metrics"] = metrics;

    m_webSocketServer->broadcast(worldState);

    // Warn if the frame took too long
    if (updateTimeUs > 16000) {
        qWarning() << "Update() took too long:" << updateTimeUs << "us";
    }
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
    ConsoleReader *m_consoleReader;
    Logger *logger;
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    MainApp app;

    if (argc > 1) {
        QString scriptPath = QString::fromLocal8Bit(argv[1]);
        app.runScript(scriptPath);
        
    }

    return a.exec();
}
#include "main.moc"

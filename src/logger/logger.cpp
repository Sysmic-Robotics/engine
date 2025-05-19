#include "logger.hpp"
#include <QDir>

Logger::Logger(World* world, Radio* radio, QObject* parent)
    : QObject(parent), m_world(world), m_radio(radio) {}

Logger::~Logger() {
    stopLogging();
}

void Logger::startLogging(const QString& filename) {
    if (m_isLogging) return;

    QString filePath = filename;
    if (filePath.isEmpty()) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");
        QDir().mkpath("logs");
        filePath = "logs/log_" + timestamp + ".csv";
    }

    m_logFile.setFileName(filePath);
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_logStream.setDevice(&m_logFile);

        // Updated header
        m_logStream << "ElapsedTime,"  // seconds since logging started
                    << "RobotID,"
                    << "Team,"
                    << "Vx_Command,"
                    << "Vy_Command,"
                    << "Angular_Command,"
                    << "Pos_X,"
                    << "Pos_Y,"
                    << "Orientation,"
                    << "Vx_Actual,"
                    << "Vy_Actual\n";

        m_timer.start();  // ⏱️ Start elapsed timer
        m_isLogging = true;
        qDebug() << "Logger started:" << filePath;
    } else {
        qWarning() << "Failed to open log file:" << filePath;
    }
}


void Logger::stopLogging() {
    if (m_logFile.isOpen()) {
        m_logFile.close();
        m_isLogging = false;
        qDebug() << "Logger stopped.";
    }
}

void Logger::logFrame() {
    if (!m_isLogging || !m_logFile.isOpen()) return;

    double elapsedSeconds = m_timer.elapsed() / 1000.0;
    const QHash<int, RobotCommand>& commands = m_radio->getCommandMap();

    const int numRobots = 6;  // Or use m_world->getNumRobots() if available

    for (int team = 0; team <= 1; ++team) {
        for (int id = 0; id < numRobots; ++id) {
            RobotState state = m_world->getRobotState(id, team);

            // Default to zero motion
            double vx = 0.0, vy = 0.0, angular = 0.0;

            // Only override if a command is found
            int key = id;  // Assuming team & id together map uniquely
            auto it = commands.find(key);
            if (it != commands.end() && it->getId() == id && it->getTeam() == team) {
                const MotionCommand& m = it->getMotionCommand();
                vx = m.getVx();
                vy = m.getVy();
                angular = m.getAngular();
            }

            m_logStream << QString::number(elapsedSeconds, 'f', 3) << ","
                        << id << ","
                        << team << ","
                        << vx << ","           // Commanded Vx
                        << vy << ","           // Commanded Vy
                        << angular << ","      // Commanded Angular
                        << state.getPosition().x() << ","
                        << state.getPosition().y() << ","
                        << state.getOrientation() << ","
                        << state.getVelocity().x() << ","
                        << state.getVelocity().y() << "\n";
        }
    }

    m_logStream.flush();
}





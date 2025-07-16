#include "logger.hpp"
#include <QDir>
#include <QDebug>

Logger::Logger(World* world, Radio* radio, QObject* parent)
    : QObject(parent), m_world(world), m_radio(radio) {}

Logger::~Logger() {
    stopLogging();
}

void Logger::startLogging(const QString& filename) {
    if (m_isLogging) return;

    QString filePath = filename;
    qDebug() << "filename is " << filename;
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

        m_timer.start();
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
    const int numRobots = 6;

    for (int team = 0; team <= 1; ++team) {
        for (int id = 0; id < numRobots; ++id) {
            RobotState state = m_world->getRobotState(id, team);

            double vx = 0.0, vy = 0.0, angular = 0.0;

            int key = id;
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
                        << vx << ","
                        << vy << ","
                        << angular << ","
                        << state.getPosition().x() << ","
                        << state.getPosition().y() << ","
                        << state.getOrientation() << ","
                        << state.getVelocity().x() << ","
                        << state.getVelocity().y() << "\n";
        }
    }

    // Add ball state as a separate row
    BallState ball = m_world->getBallState();
    QVector2D ballPos = ball.getPosition();

    m_logStream << QString::number(elapsedSeconds, 'f', 3) << ","
                << -1 << ","    // RobotID = -1
                << -1 << ","    // Team = -1
                << 0 << ","     // Vx_Command (unused)
                << 0 << ","     // Vy_Command (unused)
                << 0 << ","     // Angular_Command (unused)
                << ballPos.x() << ","
                << ballPos.y() << ","
                << 0 << ","     // Orientation (not applicable)
                << ball.getVelocity().x() << ","
                << ball.getVelocity().y() << "\n";

    m_logStream.flush();
}
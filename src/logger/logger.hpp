#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QElapsedTimer>

#include "world.hpp"
#include "radio.hpp"

class Logger : public QObject
{
    Q_OBJECT

public:
    Logger(World* world, Radio* radio, QObject* parent = nullptr);
    ~Logger();

    void startLogging(const QString& filename = QString());
    void stopLogging();
    void logFrame();

    bool isLogging() const { return m_isLogging; }

private:
    World* m_world;
    Radio* m_radio;
    QFile m_logFile;
    QTextStream m_logStream;
    bool m_isLogging = false;
    QElapsedTimer m_timer;

};

#endif // LOGGER_HPP

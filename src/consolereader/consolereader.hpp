#pragma once

#include <QObject>
#include <QThread>
#include <QString>
#include "luainterface.hpp"

class ConsoleReader : public QThread {
    Q_OBJECT

public:
    explicit ConsoleReader(LuaInterface* luaInterface, QObject* parent = nullptr)
        : QThread(parent), m_luaInterface(luaInterface) {}

protected:
    void run() override;

private:
    LuaInterface* m_luaInterface;
    QString lastScriptPath;  // Stores the last successfully run script for 'reload'

signals:
    void startRecording(const QString& filename = QString());
    void stopRecording();

};

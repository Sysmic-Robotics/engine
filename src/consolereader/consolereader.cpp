#include "consolereader.hpp"
#include <QTextStream>
#include <QDebug>

void ConsoleReader::run()
{
    QTextStream input(stdin);
    qDebug() << "[ConsoleReader] Started. Type 'run <path>', 'pause', 'reload', or 'record [filename]'";

    QString lastScriptPath;

    while (true)
    {
        QString line = input.readLine().trimmed();

        if (line.startsWith("run "))
        {
            QString path = line.mid(4).trimmed();
            if (!path.isEmpty())
            {
                m_luaInterface->runScript(path);
                lastScriptPath = path;
                qDebug() << "Running script:" << path;
            }
            else
            {
                qWarning() << "No path provided after 'run'";
            }
        }
        else if (line == "pause")
        {
            m_luaInterface->stopScript();
            qDebug() << "Script paused";
        }
        else if (line == "reload")
        {
            if (!lastScriptPath.isEmpty())
            {
                m_luaInterface->runScript(lastScriptPath);
                qDebug() << "Reloaded script:" << lastScriptPath;
            }
            else
            {
                qWarning() << "No script has been run yet to reload.";
            }
        }
        else if (line == "exit")
        {
            qDebug() << "Exiting ConsoleReader thread.";
            break;
        }
        else if (line.startsWith("record"))
        {
            QString filename = line.section(' ', 1).trimmed();
            qDebug() << "Started recording with"
                     << (filename.isEmpty() ? "default filename." : "filename: " + filename);
            emit startRecording(filename);
        }
        else if (line == "stoprecord")
        {
            emit stopRecording();
            qDebug() << "Stopped recording.";
        }
        else
        {
            qDebug() << "Unknown command:" << line;
        }
    }
}

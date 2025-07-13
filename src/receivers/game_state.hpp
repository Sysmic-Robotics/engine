#pragma once

#include <QObject>
#include <QString>

class GameState : public QObject {
    Q_OBJECT

public:
    explicit GameState(QObject* parent = nullptr);
    QString GetRefMessage() const; // Getter for referee command

public slots:
    void onRefCommand(QString command);

private:
    QString m_refCommand;
};

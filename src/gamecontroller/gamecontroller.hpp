#pragma once

#include <QObject>
#include <QUdpSocket>
#include "ssl_referee.pb.h"


class GameController : public QObject {
    Q_OBJECT

public:
    explicit GameController(QObject *parent = nullptr);
    ~GameController();

    void startListen();

    // Acceso al último estado
    SSL_Referee::Stage getStage() const;
    SSL_Referee::Command getCommand() const;

signals:
    void refereeUpdated(const SSL_Referee &referee);

private slots:
    void readPendingDatagrams();

private:
    QUdpSocket *m_udpSocket = nullptr;
    SSL_Referee m_lastReferee;
};

#include "game_state.hpp"
#include <QDebug>

GameState::GameState(QObject* parent)
    : QObject(parent)
{
    m_refCommand = "UNKNOWN";
}


void GameState::onRefCommand(QString command){
    // Handle incoming string command (possibly from a signal)
    m_refCommand = command;
}

QString GameState::GetRefMessage() const{
    return m_refCommand;
}
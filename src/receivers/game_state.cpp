#include "game_state.hpp"

Game::Game()
    : m_state(GameState::Unknown) {}

void Game::updateState(const Referee& msg) {
    m_state = mapCommandToGameState(msg.command());
}

GameState Game::state() const {
    return m_state;
}

GameState Game::mapCommandToGameState(Referee::Command cmd) {
    using RC = Referee;

    switch (cmd) {
        case RC::HALT:
        case RC::STOP:
        case RC::TIMEOUT_YELLOW:
        case RC::TIMEOUT_BLUE:
            return GameState::Stopped;

        case RC::BALL_PLACEMENT_YELLOW:
        case RC::BALL_PLACEMENT_BLUE:
            return GameState::BallPlacement;

        case RC::PREPARE_KICKOFF_YELLOW:
        case RC::PREPARE_KICKOFF_BLUE:
        case RC::PREPARE_PENALTY_YELLOW:
        case RC::PREPARE_PENALTY_BLUE:
            return GameState::Ready;

        case RC::NORMAL_START:
        case RC::FORCE_START:
        case RC::DIRECT_FREE_YELLOW:
        case RC::DIRECT_FREE_BLUE:
            return GameState::Play;

        case RC::GOAL_YELLOW:
        case RC::GOAL_BLUE:
            return GameState::GoalScored;

        default:
            return GameState::Unknown;
    }
}

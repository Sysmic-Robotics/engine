#pragma once

#include "../protobuf/ssl_gc_referee_message.pb.h"

enum class GameState {
    Stopped,
    BallPlacement,
    Ready,
    Play,
    GoalScored,
    Unknown
};

class Game {
public:
    Game();

    void updateState(const Referee& msg);
    GameState state() const;

private:
    GameState m_state;

    GameState mapCommandToGameState(Referee::Command cmd);
};

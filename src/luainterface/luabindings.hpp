#pragma once
#include <functional>
#include <string>
#include <sol/sol.hpp>
#include <QVector2D>
#include "radio.hpp"
#include "world.hpp"
#include "game_state.hpp"
#include "motion.hpp"

// Forward declarations of your app types
class World;
class Radio;
class GameState;
class Motion;
class RobotState;
class BallState;
class MotionCommand;
class KickerCommand;
class QVector2D;

struct LuaDeps {
    sol::state&           lua;
    World*                world   = nullptr;
    Radio*                radio   = nullptr;
    GameState*            game    = nullptr;

    // Optional: custom logger (defaults to std::cerr)
    std::function<void(const std::string&)> log = nullptr;
};

class LuaBindings {
public:
    explicit LuaBindings(LuaDeps deps);
    void registerAll(); // call once after opening libs / loading script

private:
    LuaDeps d_;

    // --- small helpers to keep lambdas clean ---
    void logError(const std::string& msg) const;
    bool checkWorld() const;
    bool checkRadio() const;
    bool checkGame() const;

    sol::table makeRobotTable(const RobotState& r) const;
    sol::table makeBallTable(const BallState& b) const;
};

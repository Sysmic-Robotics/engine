#include "luainterface.hpp"
#include "motion.hpp"
#include <iostream>
#include <sstream>
#include <QDir>
#include <QCoreApplication>

LuaInterface::LuaInterface(Radio* radio, World* world, GameState* game_state)
    : m_radio(radio), m_world(world), m_gameState(game_state)
{
    m_lua = sol::state();
    m_lua.open_libraries(
    sol::lib::base,
    sol::lib::debug,
    sol::lib::package,
    sol::lib::io,       // needed for file reading
    sol::lib::string,
    sol::lib::math,
    sol::lib::table,
    sol::lib::os
    );


    // Register your game-specific functions
    register_functions();
}

LuaInterface::~LuaInterface() {}

void LuaInterface::register_functions() {
    LuaDeps deps{
        .lua = m_lua,
        .world = m_world,
        .radio = m_radio,
        .game = m_gameState,
        .log = [](const std::string& s){ std::cerr << s << std::endl; } // optional
    };
    m_bindings = std::make_unique<LuaBindings>(deps);
    m_bindings->registerAll();
}

void LuaInterface::runScript(const QString& scriptPath) {
    m_isPaused = false;
    m_lua = sol::state();
    m_lua.open_libraries(
        sol::lib::base,
        sol::lib::debug,
        sol::lib::package,
        sol::lib::io,       // needed for file reading
        sol::lib::string,
        sol::lib::math,
        sol::lib::table,
        sol::lib::os
    );
    // Normalize script path
    QString normalizedPath = QDir(scriptPath).absolutePath();
    std::string fullScriptPath = scriptPath.toStdString();

    QString scriptDir = QFileInfo(scriptPath).absolutePath();
    std::string luaPathCmd = "package.path = '" + scriptDir.replace("\\", "/").toStdString() + "/?.lua;' .. package.path";
    m_lua.script(luaPathCmd);

    register_functions();

    std::cout << "[Lua] Loading script: " << fullScriptPath << std::endl;

    try {
        m_lua.script_file(fullScriptPath);
        std::cout << "[Lua] Script loaded successfully!" << std::endl;
        m_haveScript = true;
    }
    catch (const sol::error &e) {
        std::cerr << "[Lua] Error loading script: " << e.what() << std::endl;
        m_haveScript = false;
    }
}

void LuaInterface::callProcess() {
    if(m_haveScript){
        if (!m_isPaused) {
            sol::protected_function process = m_lua["process"];
            if (!process.valid()) {
                std::cerr << "[Lua] Error: process() is not defined in script!" << std::endl;
                m_isPaused = true;
                return;
            }

            sol::protected_function_result result = process();
            if (!result.valid()) {
                m_isPaused = false;
                sol::error err = result;
                std::cerr << "[Lua] Runtime error in process(): " << err.what() << std::endl;
                m_isPaused = true;
            }
        }else{

            for(int team = 0; team < 2; team++){
                for(int id = 0; id < 6; id++){
                    MotionCommand cmd(id, team, 0, 0);
                    cmd.setAngular(0);
                    m_radio->addMotionCommand(cmd);
                }
            }
        }
    }
}

void LuaInterface::pauseScript() {
    m_isPaused = true;
    MotionCommand cmd(0, 0, 0,0);
    cmd.setAngular(0);
    m_radio->addMotionCommand(cmd);
    qDebug() << "[Lua] Script stopped.";
}


void LuaInterface::resumeScript() {
    m_isPaused = false;
}

#include "process.hpp"

Process::Process(World* world, Radio* radio, LuaInterface* luaInterface, QObject* parent)
    : QObject(parent),
      m_world(world),
      m_radio(radio),
      m_luaInterface(luaInterface),
      m_selectedRobotId(-1),
      m_selectedTeam(-1),
      m_faceToActive(false),
      m_debugControl(true)
{
}

void Process::update() {
    if (m_debugControl) {
        updateDebugControl();
    } else {
        updateLuaInterface();
    }
}

void Process::setRobotSelected(int id, int team) {
    m_selectedRobotId = id;
    m_selectedTeam = team;
}

void Process::setTargetPoint(const QVector2D &point) {
    m_targetPoint = point;
    m_faceToActive = false;
}

void Process::setFaceToTarget(const QVector2D &point) {
    m_faceToTarget = point;
    m_faceToActive = true;
}

void Process::setDebugControl(bool flag) {
    m_debugControl = flag;
}

void Process::updateDebugControl() {
    // Get the current robot state and compute a command.
    RobotState robotState = m_world->getRobotState(m_selectedRobotId, m_selectedTeam);
    static Motion motion;
    MotionCommand cmd(m_selectedRobotId, m_selectedTeam);

    if (m_faceToActive) {
        cmd = motion.face_to(robotState, m_faceToTarget);
    } else {
        cmd = motion.to_point(robotState, m_targetPoint, m_world); // ✅ pasa World*
    }
    m_radio->addMotionCommand(cmd);
}

void Process::updateLuaInterface() {
    m_faceToActive = true;
    if (m_luaInterface->haveScript()) {
        m_luaInterface->callProcess();
    }
}

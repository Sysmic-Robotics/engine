#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <QObject>
#include <QVector2D>
#include "world.hpp"
#include "radio.hpp"
#include "luainterface.hpp"
#include "motion.hpp"

class Process : public QObject
{
    Q_OBJECT
public:
    explicit Process(World* world, Radio* radio, LuaInterface* luaInterface, QObject* parent = nullptr);

    // Called each update cycle to process robot commands.
    void update();

    // Setters for updating the controller state from UI events.
    void setRobotSelected(int id, int team);
    void setTargetPoint(const QVector2D &point);
    void setFaceToTarget(const QVector2D &point);
    void setDebugControl(bool flag);

private:
    // Business logic for debug control.
    void updateDebugControl();
    // Business logic for Lua control.
    void updateLuaInterface();

    // Internal state for decision making.
    int m_selectedRobotId;
    int m_selectedTeam;
    QVector2D m_targetPoint;
    QVector2D m_faceToTarget;
    bool m_faceToActive;
    bool m_debugControl;

    // Pointers to business objects (not owned by RobotController).
    World* m_world;
    Radio* m_radio;
    LuaInterface* m_luaInterface;
};

#endif // PROCESS_HPP

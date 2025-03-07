#include "bangbangcontrol.hpp"

BangBangControl::BangBangControl(float a_max, float v_max) 
    : m_A_MAX(a_max), m_V_MAX(v_max) {
}

MotionCommand BangBangControl::computeMotion(const RobotState& state, 
    QList<QVector2D> path,
    double delta) {

    // Ensure computation happens every 1/60 seconds
    if (delta < (1.0 / 60.0)) return MotionCommand(0,0);

    if( path.isEmpty() ) return MotionCommand(0,0);
    
    // Set goal point from the path
    QVector2D goal = path.takeFirst();

    // If there are more points and we are near to breaking, take the next one
    if (!path.isEmpty() && isNearToBreak(state, goal)) {
        goal = path.takeFirst();
    }

    //Trajectory2D newTraj(m_A_MAX, m_V_MAX, state.getVelocity(), state.getPosition(), goal);
    //QVector2D newVelocity = newTraj.getNextVelocity();
    QVector2D newVelocity = QVector2D(1.0,1.0);

    // Convert to local frame of reference
    float orientation = state.getOrientation();
    QVector2D localVelocity(
        newVelocity.x() * cos(-orientation) - newVelocity.y() * sin(-orientation),
        newVelocity.x() * sin(-orientation) + newVelocity.y() * cos(-orientation)
    );

    return MotionCommand(state.getId(), state.getTeam(), localVelocity.x(), localVelocity.y());
}

bool BangBangControl::isNearToBreak(const RobotState& robot, const QVector2D& point) const {
    //QVector2D velocity = robot.getVelocity();
    QVector2D velocity = robot.getPosition();
    QVector2D position = robot.getPosition();
    
    float dxToBrake = (velocity.x() * velocity.x()) / (2 * m_A_MAX);
    float dyToBrake = (velocity.y() * velocity.y()) / (2 * m_A_MAX);
    
    return (dxToBrake >= fabs(position.x() - point.x()) ||
            dyToBrake >= fabs(position.y() - point.y()));
}

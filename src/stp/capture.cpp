#include "capture.hpp"

Capture::Capture(QObject *parent) : Skill(parent), completed(false) {}

MotionCommand Capture::process(const RobotState &robotState, const BallState &ball) {

    // Approach until touch the ball with the dribbler
    float distance_to_approach = 0.18;
    double distance = (robotState.getPosition() - ball.getPosition()).length();
    if (distance > distance_to_approach) {
        static Move move;
        return move.process(robotState, ball.getPosition());
    }
    else if(distance <= distance_to_approach){
        // Aim
        
    }
    else{
        // Idle
        static Motion motion;
        completed = true;  // ✅ Keep moving if not reached
        return MotionCommand(robotState.getId(), robotState.getTeam());
    }



}
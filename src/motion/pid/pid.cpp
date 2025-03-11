#include "pid.hpp"

PID::PID(double Kp, double Ki, double Kd) 
    : Kp(Kp), Ki(Ki), Kd(Kd), prevError(0.0), integral(0.0) {}

double PID::compute(double error, double deltaTime) {
    // Compute integral term
    integral += error * deltaTime;

    // Compute derivative term
    double derivative = (error - prevError) / deltaTime;

    // Compute PID output
    double output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // Store current error for next iteration
    prevError = error;

    return output;
}
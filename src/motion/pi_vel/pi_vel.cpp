#include "pi_vel.hpp"

PI_VEL::PI_VEL(double Kp, double Ki) 
    : Kp(Kp), Ki(Ki), prevError(0.0), integral(0.0) {}

double PI_VEL::compute(double error, double deltaTime) {
    // Compute integral term
    integral += error * deltaTime;


    // Compute PID output
    double output = (Kp * error) + (Ki * integral) ;

    // Store current error for next iteration
    prevError = error;

    return output;
}
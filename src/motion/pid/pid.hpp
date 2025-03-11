#ifndef PID_H
#define PID_H

class PID {
public:
    // Constructor to initialize PID gains
    PID(double Kp, double Ki, double Kd);

    // Compute PID output given the error and time step
    double compute(double error, double deltaTime);

private:
    double Kp, Ki, Kd; // PID gains
    double prevError;  // Previous error for derivative calculation
    double integral;   // Integral term for integral control
};

#endif // PID_H

#ifndef PI_VEL_H
#define PI_VEL_H
class PI_VEL {
public:
    // Constructor to initialize PID gains
    PI_VEL(double Kp, double Ki);

    // Compute PID output given the error and time step
    double compute(double error, double deltaTime);

private:
    double Kp, Ki; // PID gains
    double prevError;  // Previous error for derivative calculation
    double integral;   // Integral term for integral control
};

#endif // PI_VEL_H

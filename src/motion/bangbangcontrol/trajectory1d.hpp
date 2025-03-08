#ifndef TRAJECTORY1D_HPP
#define TRAJECTORY1D_HPP

#include <QVector2D>
#include <QList>
#include <QtMath>
#include <stdexcept>
#include <vector>

// Constraints class
class Constraints {
public:
    double a_max;
    double v_max;

    Constraints(double a, double v) : a_max(a), v_max(v) {}
};


    
// State class
class State {
public:
    double v; // Final velocity
    double t; // Final time
    double a; // Acceleration applied from last state to this state
    double d; // Distance traveled from last state to this state

    State() : v(0), t(0), a(0), d(0) {}  // ✅ Default constructor
    State(double velocity, double time, double acceleration, double distance)
        : v(velocity), t(time), a(acceleration), d(distance) {}

    QString toString() const {
        return QString("(vf: %1, t: %2, a: %3, d: %4)").arg(v).arg(t).arg(a).arg(d);
    }
};

// Cases class with static functions
class Cases {
public:
    static State case1(const State& lastState, const Constraints& c);
    static State case2_1(const State& lastState, const Constraints& c, double wf);
    static State case2_2(const State& lastState, const Constraints& c, double wf);
    static State case2_3(const State& lastState, const Constraints& c);
    static State case3(const State& lastState, const Constraints& c);
};

// Trajectory1D class
class Trajectory1D {
public:
    Trajectory1D() = default;  // ✅ Default constructor
    Trajectory1D(double a_max, double v_max, double v0, double wf);
    
    double tf() const; // Returns final time
    QVector2D getSolution() const; // Returns time and velocity as a QVector2D

private:
    std::vector<State> states;
};

#endif // TRAJECTORY1D_HPP

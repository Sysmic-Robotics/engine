#include "trajectory1d.hpp"
#include <QDebug>

// ---- Case Implementations ----

// Case 1: Initial velocity is negative, accelerate to zero
State Cases::case1(const State& lastState, const Constraints& c) {
    double t = -lastState.v / c.a_max;
    double d = -(lastState.v * lastState.v) / (2 * c.a_max);
    return State(0, t, c.a_max, d);
}

// Case 2.1: Accelerate to reach max velocity or until deceleration is needed
State Cases::case2_1(const State& lastState, const Constraints& c, double wf) {
    double tI = (c.v_max - lastState.v) / c.a_max;
    double v1 = qSqrt(wf * c.a_max + (lastState.v * lastState.v) / 2);
    double tII = (v1 - lastState.v) / c.a_max;

    if (tI < tII) {
        double d = (c.v_max * c.v_max - lastState.v * lastState.v) / (2 * c.a_max);
        return State(c.v_max, tI, c.a_max, d);
    } else {
        double d = wf / 2 + (lastState.v * lastState.v) / (2 * c.a_max);
        return State(v1, tII, c.a_max, d);
    }
}

// Case 2.2: Cruise at max velocity until deceleration
State Cases::case2_2(const State& lastState, const Constraints& c, double wf) {
    double t = (wf / c.v_max) - (c.v_max / (2 * c.a_max));
    double d = wf - ((lastState.v * lastState.v) / (2 * c.a_max));
    return State(c.v_max, t, 0, d);
}

// Case 2.3: Decelerate to stop
State Cases::case2_3(const State& lastState, const Constraints& c) {
    double t = lastState.v / c.a_max;
    double d = (lastState.v * lastState.v) / (2 * c.a_max);
    return State(0, t, -c.a_max, d);
}

// Case 3: Reduce speed to the allowed maximum velocity
State Cases::case3(const State& lastState, const Constraints& c) {
    double t = (lastState.v - c.v_max) / c.a_max;
    double d = (1 / (2 * c.a_max)) * (lastState.v * lastState.v - c.v_max * c.v_max);
    return State(c.v_max, t, -c.a_max, d);
}

// ---- Trajectory1D Constructor ----
Trajectory1D::Trajectory1D(double a_max, double v_max, double v0, double wf) {
    if (a_max <= 0 || v_max <= 0) {
        throw std::invalid_argument("Max acceleration and velocity must be greater than 0.");
    }

    if (wf == 0) {
        states.push_back(State(v0, 0, 0, 0));
        return;
    }

    // Normalize
    int wf_sign = (wf < 0) ? -1 : 1;
    wf = wf_sign * wf;
    v0 = wf_sign * v0;

    State state_0(v0, 0, 0, 0);
    Constraints constraints(a_max, v_max);
    states.push_back(state_0);

    // Determine case
    if (v0 < 0) {
        states.push_back(Cases::case1(state_0, constraints));
    } else if (wf > (v0 * v0) / (2 * a_max) && v_max > v0 && v0 >= 0) {
        states.push_back(Cases::case2_1(state_0, constraints, wf));
    } else if (wf > (v0 * v0) / (2 * a_max) && qAbs(v_max - v0) <= 0.002) {
        states.push_back(Cases::case2_2(state_0, constraints, wf));
    } else if (wf <= (v0 * v0) / (2 * a_max) && 0 < v0 && v0 <= v_max) {
        states.push_back(Cases::case2_3(state_0, constraints));
    } else if (v0 > v_max) {
        states.push_back(Cases::case3(state_0, constraints));
    }

    // Normalize states if needed
    if (wf_sign == -1) {
        for (State& s : states) {
            s.v = -s.v;
        }
    }
}

// ---- Trajectory1D Methods ----
double Trajectory1D::tf() const {
    return states.empty() ? 0.0 : states.back().t;
}

QVector2D Trajectory1D::getSolution() const {
    if (states.empty()) {
        return QVector2D(0, 0);
    }
    return QVector2D(states.back().t, states.back().v);
}


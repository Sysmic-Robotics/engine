#include "tracker.hpp"
#include <cmath>

Tracker::Tracker() {}

ExtendedKalmanFilter Tracker::createInitialFilter() const {
    using EKF = ExtendedKalmanFilter;
    EKF::Vector7d initial_state;
    initial_state << 0.0, 0.0, std::sin(0.0), std::cos(0.0), 0.0, 0.0, 0.0;

    // Initial state covariance (uncertainty about initial guess)
    EKF::Matrix7d P = EKF::Matrix7d::Zero();
    P(0, 0) = 1e-4;  // x
    P(1, 1) = 1e-4;  // y
    P(2, 2) = 1e-6;  // sin(theta)
    P(3, 3) = 1e-6;  // cos(theta)
    P(4, 4) = 1.0;   // vx
    P(5, 5) = 1.0;   // vy
    P(6, 6) = 1.0;   // omega

    // Process noise (model uncertainty)
    EKF::Matrix7d Q = EKF::Matrix7d::Zero();
    Q(0, 0) = 1e-6;  // x
    Q(1, 1) = 1e-6;  // y
    Q(4, 4) = 1.0;  // vx
    Q(5, 5) = 1.0;  // vy
    Q(6, 6) = 1.0;  // omega

    // Measurement noise (sensor accuracy)
    EKF::Matrix3d R = EKF::Matrix3d::Identity() * 1e-12;  // near-perfect

    return EKF(initial_state, P, Q, R);
}


std::tuple<double, double, double, double, double, double>
Tracker::track(int team, int id, double x, double y, double theta, double dt) {
    RobotKey key = {team, id};

    auto it = filters_.find(key);
    if (it == filters_.end()) {
        auto result = filters_.emplace(key, createInitialFilter());
        it = result.first;
    }

    return it->second.filterPose(x, y, theta, dt);
    

}

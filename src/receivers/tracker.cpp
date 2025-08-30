#include "tracker.hpp"
#include <cmath>

Tracker::Tracker() {}

ExtendedKalmanFilter Tracker::createInitialFilter() const {
    using EKF = ExtendedKalmanFilter;
    EKF::Vector7d initial_state;
    initial_state << 0.0, 0.0, std::sin(0.0), std::cos(0.0), 0.0, 0.0, 0.0;

    // Initial state covariance: high uncertainty for vx, vy, omega; low elsewhere
    EKF::Matrix7d P = EKF::Matrix7d::Zero();
    P(0, 0) = 1e-7;  // x
    P(1, 1) = 1e-7;  // y
    P(2, 2) = 1e-7;  // sin(theta)
    P(3, 3) = 1e-7;  // cos(theta)
    P(4, 4) = 1.0;   // vx
    P(5, 5) = 1.0;   // vy
    P(6, 6) = 1.0;   // omega

    // Process noise: moderate evolution allowed for vx, vy, omega
    EKF::Matrix7d Q = EKF::Matrix7d::Zero();
    Q(0, 0) = 1e-7;  // x
    Q(1, 1) = 1e-7;  // y
    Q(2, 2) = 1e-4;  // sin(theta)
    Q(3, 3) = 1e-4;  // cos(theta)
    Q(4, 4) = 1e-4;  // vx
    Q(5, 5) = 1e-4;  // vy
    Q(6, 6) = 1e-2;  // omega

    // Measurement noise: x, y, theta sensed accurately
    EKF::Matrix3d R = EKF::Matrix3d::Zero();
    R(0, 0) = 1e-6;  // x
    R(1, 1) = 1e-6;  // y
    R(2, 2) = 1e-6;  // theta

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

    // Call filter to update internal state and get velocity
    auto [_, __, ___, vx, vy, omega] = it->second.filterPose(x, y, theta, dt);

    // Return unfiltered pose + filtered velocity
    return std::make_tuple(x, y, theta, vx, vy, omega);
}


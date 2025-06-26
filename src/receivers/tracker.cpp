#include "tracker.hpp"
#include <cmath>

Tracker::Tracker() {}

ExtendedKalmanFilter Tracker::createInitialFilter() const {
    using EKF = ExtendedKalmanFilter;
    EKF::Vector7d initial_state;
    initial_state << 0.0, 0.0, std::sin(0.0), std::cos(0.0), 0.0, 0.0, 0.0;

    EKF::Matrix7d P = EKF::Matrix7d::Identity() * 0.1;
    EKF::Matrix7d Q = EKF::Matrix7d::Identity() * 0.01;
    EKF::Matrix3d R = EKF::Matrix3d::Identity() * 0.05;

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

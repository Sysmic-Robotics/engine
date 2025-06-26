#ifndef TRACKER_HPP
#define TRACKER_HPP

#include "ekf.hpp"
#include <map>
#include <tuple>
#include <utility>

class Tracker {
public:
    Tracker();

    // Track a robot using team + id
    std::tuple<double, double, double, double, double, double>
    track(int team, int id, double x, double y, double theta, double dt);

private:
    using RobotKey = std::pair<int, int>; // (team, id)
    std::map<RobotKey, ExtendedKalmanFilter> filters_;

    ExtendedKalmanFilter createInitialFilter() const;
};

#endif // TRACKER_HPP

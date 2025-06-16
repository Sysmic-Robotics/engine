// path_planner.cpp
#include "path_planner.hpp"
#include <cmath>


namespace {
    QVector2D rotateVector(const QVector2D& v, float angle) {
        float cos_a = std::cos(angle);
        float sin_a = std::sin(angle);
        return QVector2D(
            cos_a * v.x() - sin_a * v.y(),
            sin_a * v.x() + cos_a * v.y()
        );
    }
}

FastPathPlanner::FastPathPlanner(int max_depth) : max_depth(max_depth) {}

FastPathPlanner::Environment::Environment(const std::vector<QVector2D>& robots) : robots(robots) {}

bool FastPathPlanner::Environment::collides(const QVector2D& point) const {
    if (point.x() < -4.5 || point.x() > 4.5 || point.y() < -3.0 || point.y() > 3.0)
        return true;
    /*
    if ((point.x() >= -4.5 && point.x() <= -4 && point.y() >= -0.5 && point.y() <= 0.5) ||
        (point.x() >= 4 && point.x() <= 4.5 && point.y() >= -0.5 && point.y() <= 0.5))
        return true;
    */  
    for (const auto& robot : robots) {
        if ((point - robot).length() <= 0.2)  // Aumentado de 0.18 a 0.35
            return true;
    }
    return false;
}

bool FastPathPlanner::trajectoryCollides(const Trajectory& traj, const Environment& env) const {
    for (int i = 0; i < 200; ++i) {  // Aumentado de 100 a 200
        float t = i / 200.0f;
        QVector2D point = traj.start + t * (traj.goal - traj.start);
        if (env.collides(point)) return true;
    }
    return false;
}

QVector2D FastPathPlanner::searchSubgoal(const Trajectory& traj, const QString& obstacle, const Environment& env, float robot_diameter, int direction) {
    QVector2D obs_point = traj.goal;
    QVector2D dir_vec = (obs_point - traj.start).normalized();

    for (int i = 0; i < 10; ++i) {
        QVector2D offset = QVector2D(-dir_vec.y(), dir_vec.x()) * (direction * robot_diameter);
        QVector2D subgoal = obs_point + offset;
        if (!env.collides(subgoal) && subgoal.x() >= -4.5 && subgoal.x() <= 4.5 && subgoal.y() >= -3 && subgoal.y() <= 3)
            return subgoal;
        dir_vec = rotateVector(dir_vec, M_PI / 4);
    }
    return QVector2D(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
}

FastPathPlanner::Trajectory FastPathPlanner::makeSegment(const QVector2D& from, const QVector2D& to) {
    return Trajectory(from, to);
}

std::vector<FastPathPlanner::Trajectory> FastPathPlanner::createPath(const QVector2D& start, const QVector2D& goal, const Environment& env) {
    std::vector<Trajectory> resultA, resultB;
    std::vector<std::pair<Trajectory, int>> stack = {{Trajectory(start, goal), 0}};

    while (!stack.empty()) {
        auto [traj, depth] = stack.back(); stack.pop_back();
        if (trajectoryCollides(traj, env) && depth < max_depth) {
            QVector2D sub = searchSubgoal(traj, "", env);
            if (std::isnan(sub.x())) return {};
            stack.emplace_back(Trajectory(sub, traj.goal), depth + 1);
            stack.emplace_back(Trajectory(traj.start, sub), depth + 1);
        } else {
            resultA.push_back(traj);
        }
    }

    stack = {{Trajectory(start, goal), 0}};
    while (!stack.empty()) {
        auto [traj, depth] = stack.back(); stack.pop_back();
        if (trajectoryCollides(traj, env) && depth < max_depth) {
            QVector2D sub = searchSubgoal(traj, "", env, 0.36f, -1);
            if (std::isnan(sub.x())) return {};
            stack.emplace_back(Trajectory(sub, traj.goal), depth + 1);
            stack.emplace_back(Trajectory(traj.start, sub), depth + 1);
        } else {
            resultB.push_back(traj);
        }
    }

    return (resultA.size() <= resultB.size() && !resultA.empty()) ? resultA : resultB;
}

std::vector<QVector2D> FastPathPlanner::simplifyPath(const std::vector<QVector2D>& path, const Environment& env) {
    if (path.size() <= 2) return path;

    std::vector<QVector2D> result;
    result.push_back(path[0]);

    size_t i = 0;
    while (i < path.size() - 1) {
        size_t j = path.size() - 1;
        while (j > i + 1) {
            if (!trajectoryCollides(Trajectory(path[i], path[j]), env)) break;
            --j;
        }
        result.push_back(path[j]);
        i = j;
    }
    return result;
}

std::vector<QVector2D> FastPathPlanner::getPath(const QVector2D& from, const QVector2D& to, const std::vector<QVector2D>& robot_positions) {
    Environment env(robot_positions);
    auto raw_path = createPath(from, to, env);
    if (raw_path.empty()) return {};

    std::vector<QVector2D> points;
    for (const auto& seg : raw_path) points.push_back(seg.start);
    points.push_back(raw_path.back().goal);

    return simplifyPath(points, env);
}

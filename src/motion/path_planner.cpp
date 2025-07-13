#include "path_planner.hpp"
#include <cmath>
#include <limits>

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

bool FastPathPlanner::trajectoryCollides(const Trajectory& traj, const Environment& env) const {
    for (int i = 0; i < 200; ++i) {
        float t = i / 200.0f;
        QVector2D point = traj.start + t * (traj.goal - traj.start);
        if (env.collides(point)) return true;
    }
    return false;
}

QVector2D FastPathPlanner::searchSubgoal(const Trajectory& traj, const QString&, const Environment& env, float robot_diameter, int direction) {
    QVector2D obs_point = traj.goal;
    QVector2D dir_vec = (obs_point - traj.start).normalized();

    for (int i = 0; i < 10; ++i) {
        QVector2D offset = QVector2D(-dir_vec.y(), dir_vec.x()) * (direction * robot_diameter);
        QVector2D subgoal = obs_point + offset;
        if (!env.collides(subgoal) &&
            subgoal.x() >= -4.5 && subgoal.x() <= 4.5 &&
            subgoal.y() >= -3.0 && subgoal.y() <= 3.0) {
            return subgoal;
        }
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

    // Path A (right-hand rule)
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

    // Path B (left-hand rule)
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

QList<QVector2D> FastPathPlanner::getPath(const QVector2D& from, const QVector2D& to, const Environment& env) {
    auto raw_path = createPath(from, to, env);
    if (raw_path.empty()) {
        return { from, to }; // Fallback if path is invalid
    }

    std::vector<QVector2D> points;
    for (const auto& seg : raw_path) {
        points.push_back(seg.start);
    }
    points.push_back(raw_path.back().goal);

    // Simplify the path
    std::vector<QVector2D> simplified = simplifyPath(points, env);

    // Convert to QList
    QList<QVector2D> path;
    for (const QVector2D& p : simplified) {
        path.append(p);
    }

    return path;
}


// path_planner.hpp
#pragma once

#include <QVector2D>
#include <vector>

class FastPathPlanner {
public:
    FastPathPlanner(int max_depth = 10);
    std::vector<QVector2D> getPath(const QVector2D& from, const QVector2D& to, const std::vector<QVector2D>& robot_positions);

private:
    struct Trajectory {
        QVector2D start;
        QVector2D goal;
        Trajectory(const QVector2D& s, const QVector2D& g) : start(s), goal(g) {}
    };

    class Environment {
    public:
        Environment(const std::vector<QVector2D>& robots);
        bool collides(const QVector2D& point) const;
    private:
        std::vector<QVector2D> robots;
    };

    bool trajectoryCollides(const Trajectory& traj, const Environment& env) const;
    QVector2D searchSubgoal(const Trajectory& traj, const QString& obstacle, const Environment& env, float robot_diameter = 0.36f, int direction = 1);
    Trajectory makeSegment(const QVector2D& from, const QVector2D& to);
    std::vector<Trajectory> createPath(const QVector2D& start, const QVector2D& goal, const Environment& env);
    std::vector<QVector2D> simplifyPath(const std::vector<QVector2D>& path, const Environment& env);

    int max_depth;
};

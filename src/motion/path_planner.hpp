#pragma once

#include <QVector2D>
#include <vector>
#include <QString>
#include "environment.hpp"  // Now using the standalone Environment class

class FastPathPlanner {
public:
    FastPathPlanner(int max_depth = 10);

    QList<QVector2D> getPath(const QVector2D& from, const QVector2D& to, const Environment& env);

private:
    struct Trajectory {
        QVector2D start;
        QVector2D goal;
        Trajectory(const QVector2D& s, const QVector2D& g) : start(s), goal(g) {}
    };

    bool trajectoryCollides(const Trajectory& traj, const Environment& env) const;
    QVector2D searchSubgoal(const Trajectory& traj, const QString& obstacle, const Environment& env, float robot_diameter = 0.36f, int direction = 1);
    Trajectory makeSegment(const QVector2D& from, const QVector2D& to);
    std::vector<Trajectory> createPath(const QVector2D& start, const QVector2D& goal, const Environment& env);
    std::vector<QVector2D> simplifyPath(const std::vector<QVector2D>& path, const Environment& env);

    int max_depth;
};

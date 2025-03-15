#include "pathplanning.hpp"

PathPlanning::PathPlanning() {}

/**
 * Generates a linear path between two points using QList.
 */
QList<QVector2D> PathPlanning::generateLinearPath(const QVector2D& start, const QVector2D& end, int steps) {
    QList<QVector2D> path;
    if (steps < 2) {
        path.append(start);
        path.append(end);
        return path;
    }

    for (int i = 0; i <= steps; i++) {
        float t = static_cast<float>(i) / steps;
        float x = (1 - t) * start.x() + t * end.x();
        float y = (1 - t) * start.y() + t * end.y();
        path.append(QVector2D(x, y));
    }
    return path;
}

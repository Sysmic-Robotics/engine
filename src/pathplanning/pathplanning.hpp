#ifndef PATHPLANNING_HPP
#define PATHPLANNING_HPP

#include <QVector2D>
#include <QList>

class PathPlanning {
public:
    PathPlanning();

    /**
     * @brief Generates a linear path between two points.
     * @param start The starting point.
     * @param end The destination point.
     * @param steps Number of intermediate points.
     * @return A QList containing waypoints from start to end.
     */
    QList<QVector2D> generateLinearPath(const QVector2D& start, const QVector2D& end, int steps = 10);
};

#endif // PATHPLANNING_HPP

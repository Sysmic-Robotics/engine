#ifndef TRAJECTORY2D_HPP
#define TRAJECTORY2D_HPP

#include <QVector2D>
#include <QtMath>
#include "trajectory1d.hpp"  // Assuming you have a C++ equivalent for Trajectory1D

class Trajectory2D {
public:
    Trajectory2D(double a_max, double v_max, 
        const QVector2D& v0, const QVector2D& from_point, const QVector2D& to_point);
    
    QVector2D getNextVelocity() const;

private:
    Trajectory1D traj_x;
    Trajectory1D traj_y;
    bool valid = false;
};

#endif // TRAJECTORY2D_HPP

#include "trajectory2d.hpp"

Trajectory2D::Trajectory2D(
    double a_max, 
    double v_max, 
    const QVector2D& v0, 
    const QVector2D& from_point, 
    const QVector2D& to_point)
    : traj_x(), traj_y(), valid(false) {  // ✅ Initialize members

    double min_alpha = 0;
    double max_alpha = M_PI / 2;
    const double epsilon = 0.05;
    
    double wfx = to_point.x() - from_point.x();
    double wfy = to_point.y() - from_point.y();

    for (int i = 0; i < 20; ++i) {
        double mid_alpha = (min_alpha + max_alpha) / 2;

        Trajectory1D traj_x_candidate(a_max * qCos(mid_alpha), 
                                      v_max * qCos(mid_alpha), 
                                      v0.x(), wfx);
        Trajectory1D traj_y_candidate(a_max * qSin(mid_alpha), 
                                      v_max * qSin(mid_alpha), 
                                      v0.y(), wfy);

        if (traj_x_candidate.tf() == 0.0 || traj_y_candidate.tf() == 0.0) {
            traj_x = traj_x_candidate;
            traj_y = traj_y_candidate;
            valid = true;
            break;
        }

        if (qAbs(traj_x_candidate.tf() - traj_y_candidate.tf()) < epsilon) {
            traj_x = traj_x_candidate;
            traj_y = traj_y_candidate;
            valid = true;
            break;
        }

        if (traj_x_candidate.tf() > traj_y_candidate.tf()) {
            max_alpha = mid_alpha;
        } else {
            min_alpha = mid_alpha;
        }
    }
}

QVector2D Trajectory2D::getNextVelocity() const {
    if (!valid) {
        return QVector2D(0, 0);
    }
    
    QVector2D x_sol = traj_x.getSolution();
    QVector2D y_sol = traj_y.getSolution();
    
    return QVector2D(x_sol.y(), y_sol.y());  // ✅ Corrected
}

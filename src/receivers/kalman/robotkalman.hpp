#ifndef ROBOTKALMAN_H
#define ROBOTKALMAN_H

#include <QVector>
#include <QtMath>

class RobotKalman
{
public:
    RobotKalman(double dt, double sigmaX, double sigmaY, double sigmaTheta, double q);
    void initialize(double x, double y, double theta);
    void predict();
    void update(double z_x, double z_y, double z_theta);
    QVector<double> getState() const;

private:
    double normalizeAngle(double angle);

    QVector<double> x_;                // Estado [x, vx, y, vy, theta, omega]
    QVector<QVector<double>> P_, A_, Q_, Ht, I_, H, R;
    double dt_;
};

#endif // ROBOTKALMAN_H

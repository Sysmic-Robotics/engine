#include <cmath>
#include "ekf.hpp"

ExtendedKalmanFilter::ExtendedKalmanFilter(const Vector7d& initial_state,
                                           const Matrix7d& initial_covariance,
                                           const Matrix7d& process_noise,
                                           const Matrix3d& measurement_noise)
    : x_(initial_state), P_(initial_covariance), Q_(process_noise), R_(measurement_noise) {}

void ExtendedKalmanFilter::predict(double dt) {
    Matrix7d F = jacobian_f(x_, dt);
    x_ = f(x_, dt);
    P_ = F * P_ * F.transpose() + Q_;
}

void ExtendedKalmanFilter::update(const Vector3d& z) {
    Matrix3x7d H = jacobian_h(x_);
    Vector3d y = z - h(x_);
    y(2) = normalize_angle(y(2));

    Matrix3d S = H * P_ * H.transpose() + R_;
    Eigen::Matrix<double, 7, 3> K = P_ * H.transpose() * S.inverse();

    x_ = x_ + K * y;
    P_ = (Matrix7d::Identity() - K * H) * P_;
}

ExtendedKalmanFilter::Vector7d ExtendedKalmanFilter::get_state() const {
    return x_;
}

ExtendedKalmanFilter::Matrix7d ExtendedKalmanFilter::get_covariance() const {
    return P_;
}

ExtendedKalmanFilter::Vector7d ExtendedKalmanFilter::f(const Vector7d& x, double dt) {
    double sin_theta = x(2);
    double cos_theta = x(3);
    double vx = x(4);
    double vy = x(5);
    double omega = x(6);
    double theta = std::atan2(sin_theta, cos_theta);
    theta += omega * dt;

    Vector7d new_x = x;
    new_x(0) += vx * dt;
    new_x(1) += vy * dt;
    new_x(2) = std::sin(theta);
    new_x(3) = std::cos(theta);
    // vx, vy, omega remain unchanged
    return new_x;
}

ExtendedKalmanFilter::Vector3d ExtendedKalmanFilter::h(const Vector7d& x) {
    double theta = std::atan2(x(2), x(3));
    Vector3d z;
    z << x(0), x(1), theta;
    return z;
}

ExtendedKalmanFilter::Matrix7d ExtendedKalmanFilter::jacobian_f(const Vector7d& x, double dt) {
    Matrix7d F = Matrix7d::Identity();
    F(0, 4) = dt;
    F(1, 5) = dt;

    double sin_theta = x(2);
    double cos_theta = x(3);
    double omega = x(6);
    double theta = std::atan2(sin_theta, cos_theta);
    double theta_new = theta + omega * dt;

    F(2, 6) = dt * std::cos(theta_new);
    F(3, 6) = -dt * std::sin(theta_new);

    return F;
}

ExtendedKalmanFilter::Matrix3x7d ExtendedKalmanFilter::jacobian_h(const Vector7d& x) {
    Matrix3x7d H = Matrix3x7d::Zero();
    H(0, 0) = 1;
    H(1, 1) = 1;

    double sin_theta = x(2);
    double cos_theta = x(3);
    double denom = sin_theta * sin_theta + cos_theta * cos_theta;

    if (denom != 0.0) {
        H(2, 2) = cos_theta / denom;
        H(2, 3) = -sin_theta / denom;
    }

    return H;
}

double ExtendedKalmanFilter::normalize_angle(double angle) {
    while (angle > M_PI) angle -= 2 * M_PI;
    while (angle < -M_PI) angle += 2 * M_PI;
    return angle;
}

std::tuple<double, double, double, double, double, double>
ExtendedKalmanFilter::filterPose(double x_meas, double y_meas, double theta_meas, double dt) {
    // Predict
    predict(dt);

    // Prepare measurement vector
    Vector3d z;
    z << x_meas, y_meas, theta_meas;

    // Update
    update(z);

    // Extract filtered state
    double x_filtered = x_(0);
    double y_filtered = x_(1);
    double theta_filtered = std::atan2(x_(2), x_(3)); // from sinθ and cosθ
    double vx = x_(4);
    double vy = x_(5);
    double omega = x_(6);

    return {x_filtered, y_filtered, theta_filtered, vx, vy, omega};
}
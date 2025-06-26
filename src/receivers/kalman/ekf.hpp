#ifndef EXTENDED_KALMAN_FILTER_HPP
#define EXTENDED_KALMAN_FILTER_HPP

#include <Eigen/Dense>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class ExtendedKalmanFilter {
public:
    using Vector7d = Eigen::Matrix<double, 7, 1>;
    using Matrix7d = Eigen::Matrix<double, 7, 7>;
    using Matrix3x7d = Eigen::Matrix<double, 3, 7>;
    using Matrix3d = Eigen::Matrix<double, 3, 3>;
    using Vector3d = Eigen::Vector3d;

    ExtendedKalmanFilter(const Vector7d& initial_state,
                         const Matrix7d& initial_covariance,
                         const Matrix7d& process_noise,
                         const Matrix3d& measurement_noise);

    void predict(double dt);
    void update(const Vector3d& z);

    Vector7d get_state() const;
    Matrix7d get_covariance() const;
        // New method in header
    std::tuple<double, double, double, double, double, double>  filterPose(double x_meas, double y_meas, double theta_meas, double dt);

private:
    Vector7d x_;
    Matrix7d P_;
    Matrix7d Q_;
    Matrix3d R_;

    Vector7d f(const Vector7d& x, double dt);
    Vector3d h(const Vector7d& x);
    Matrix7d jacobian_f(const Vector7d& x, double dt);
    Matrix3x7d jacobian_h(const Vector7d& x);
    double normalize_angle(double angle);
};

#endif // EXTENDED_KALMAN_FILTER_HPP

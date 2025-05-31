#include "robotkalman.hpp"

RobotKalman::RobotKalman(double dt, double sigmaX, double sigmaY, double sigmaTheta, double q)
    : dt_(dt)
{
    x_ = QVector<double>(6, 0.0);
    P_ = QVector<QVector<double>>(6, QVector<double>(6, 0.0));
    A_ = QVector<QVector<double>>(6, QVector<double>(6, 0.0));
    Q_ = QVector<QVector<double>>(6, QVector<double>(6, 0.0));
    H = QVector<QVector<double>>(3, QVector<double>(6, 0.0));
    R = QVector<QVector<double>>(3, QVector<double>(3, 0.0));
    Ht = QVector<QVector<double>>(6, QVector<double>(3, 0.0));
    I_ = QVector<QVector<double>>(6, QVector<double>(6, 0.0));

    for (int i = 0; i < 6; ++i)
        I_[i][i] = 1.0;

    // Matriz A (modelo de movimiento)
    for (int i = 0; i < 6; ++i)
        A_[i][i] = 1.0;

    A_[0][1] = dt_;
    A_[2][3] = dt_;
    A_[4][5] = dt_;

    // Ruido de proceso Q
    for (int i = 0; i < 6; ++i)
        Q_[i][i] = q;

    // Matriz H
    H[0][0] = 1.0; // observamos x
    H[1][2] = 1.0; // observamos y
    H[2][4] = 1.0; // observamos theta

    // Transpuesta de H
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 6; ++j)
            Ht[j][i] = H[i][j];

    // Ruido de medición R
    R[0][0] = sigmaX * sigmaX;
    R[1][1] = sigmaY * sigmaY;
    R[2][2] = sigmaTheta * sigmaTheta;
}

void RobotKalman::initialize(double x, double y, double theta)
{
    x_[0] = x;
    x_[1] = 0.0;
    x_[2] = y;
    x_[3] = 0.0;
    x_[4] = theta;
    x_[5] = 0.0;

    for (int i = 0; i < 6; ++i)
        P_[i][i] = 1.0;
}

void RobotKalman::predict()
{
    QVector<double> x_pred(6, 0.0);

    // x' = A * x
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            x_pred[i] += A_[i][j] * x_[j];

    x_pred[4] = normalizeAngle(x_pred[4]);

    x_ = x_pred;

    // P' = A * P * A^T + Q
    QVector<QVector<double>> AP(6, QVector<double>(6, 0.0));
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            for (int k = 0; k < 6; ++k)
                AP[i][j] += A_[i][k] * P_[k][j];

    QVector<QVector<double>> P_pred(6, QVector<double>(6, 0.0));
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            for (int k = 0; k < 6; ++k)
                P_pred[i][j] += AP[i][k] * A_[j][k]; // A^T

    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            P_[i][j] = P_pred[i][j] + Q_[i][j];
}

QVector<QVector<double>> invert3x3(const QVector<QVector<double>>& m) {
    QVector<QVector<double>> inv(3, QVector<double>(3, 0.0));

    // Compute determinant
    double det =
        m[0][0] * (m[1][1]*m[2][2] - m[1][2]*m[2][1]) -
        m[0][1] * (m[1][0]*m[2][2] - m[1][2]*m[2][0]) +
        m[0][2] * (m[1][0]*m[2][1] - m[1][1]*m[2][0]);

    if (qFuzzyIsNull(det) || qAbs(det) < 1e-12) {
        qWarning("Matrix is singular or nearly singular, inversion failed.");
        return inv;  // Return zero matrix (or handle error as needed)
    }

    double invDet = 1.0 / det;

    // Compute adjugate and divide by determinant
    inv[0][0] =  (m[1][1]*m[2][2] - m[1][2]*m[2][1]) * invDet;
    inv[0][1] = -(m[0][1]*m[2][2] - m[0][2]*m[2][1]) * invDet;
    inv[0][2] =  (m[0][1]*m[1][2] - m[0][2]*m[1][1]) * invDet;

    inv[1][0] = -(m[1][0]*m[2][2] - m[1][2]*m[2][0]) * invDet;
    inv[1][1] =  (m[0][0]*m[2][2] - m[0][2]*m[2][0]) * invDet;
    inv[1][2] = -(m[0][0]*m[1][2] - m[0][2]*m[1][0]) * invDet;

    inv[2][0] =  (m[1][0]*m[2][1] - m[1][1]*m[2][0]) * invDet;
    inv[2][1] = -(m[0][0]*m[2][1] - m[0][1]*m[2][0]) * invDet;
    inv[2][2] =  (m[0][0]*m[1][1] - m[0][1]*m[1][0]) * invDet;

    return inv;
}

void RobotKalman::update(double z_x, double z_y, double z_theta)
{
    QVector<double> z = { z_x, z_y, z_theta };

    // y = z - H * x
    QVector<double> y(3, 0.0);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 6; ++j)
            y[i] += H[i][j] * x_[j];

    for (int i = 0; i < 3; ++i)
        y[i] = z[i] - y[i];

    y[2] = normalizeAngle(y[2]);

    // S = H * P * H^T + R
    QVector<QVector<double>> HP(3, QVector<double>(6, 0.0));
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 6; ++j)
            for (int k = 0; k < 6; ++k)
                HP[i][j] += H[i][k] * P_[k][j];

    QVector<QVector<double>> S(3, QVector<double>(3, 0.0));
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 6; ++k)
                S[i][j] += HP[i][k] * Ht[k][j];

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            S[i][j] += R[i][j];

    // --- INVERT S (basic 3x3 inversion or use helper) ---
    QVector<QVector<double>> S_inv = invert3x3(S); // ← you must implement this

    // --- Compute Kalman Gain: K = P * H^T * S^-1 ---
    QVector<QVector<double>> PHt(6, QVector<double>(3, 0.0));
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 6; ++k)
                PHt[i][j] += P_[i][k] * Ht[k][j];

    QVector<QVector<double>> K(6, QVector<double>(3, 0.0));
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                K[i][j] += PHt[i][k] * S_inv[k][j];

    // --- Update state: x = x + K * y ---
    QVector<double> x_new(6, 0.0);
    for (int i = 0; i < 6; ++i) {
        x_new[i] = x_[i];
        for (int j = 0; j < 3; ++j)
            x_new[i] += K[i][j] * y[j];
    }
    x_new[4] = normalizeAngle(x_new[4]); // normalize theta

    x_ = x_new;

    // --- Update covariance: P = (I - K * H) * P ---
    QVector<QVector<double>> KH(6, QVector<double>(6, 0.0));
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            for (int k = 0; k < 3; ++k)
                KH[i][j] += K[i][k] * H[k][j];

    QVector<QVector<double>> I_minus_KH(6, QVector<double>(6, 0.0));
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            I_minus_KH[i][j] = (i == j ? 1.0 : 0.0) - KH[i][j];

    QVector<QVector<double>> P_new(6, QVector<double>(6, 0.0));
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            for (int k = 0; k < 6; ++k)
                P_new[i][j] += I_minus_KH[i][k] * P_[k][j];

    P_ = P_new;
}




QVector<double> RobotKalman::getState() const
{
    return x_;
}

double RobotKalman::normalizeAngle(double angle)
{
    return std::fmod(angle + M_PI, 2 * M_PI) - M_PI;
}

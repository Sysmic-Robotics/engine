#ifndef KALMAN_FILTER_HPP
#define KALMAN_FILTER_HPP

class KalmanFilter {
public:
    KalmanFilter();
    void initializePos(float pos[2]);
    void updatePosition(float z[2], float filtOut[2][2]);

private:
    float Aimp[2][2], Bimp[2], Cimp[2][2], Dimp[2], lossVec[2];
    float CimpInv[2][2];
    float filtState[2][2];
    int usageCount;
};

#endif // KALMAN_FILTER_HPP

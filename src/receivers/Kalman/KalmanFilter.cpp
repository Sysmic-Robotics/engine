// KalmanFilter.cpp sin archivos externos, con parámetros internos suaves
#include "KalmanFilter.hpp"
#include <cmath>

const float VELOCITY_THRES = 0.5f;

KalmanFilter::KalmanFilter() : usageCount(0) {
    // Parámetros para bajo ruido
    //Aimp[0][0] = 0.95f   Aimp[1][1] = 0.95f  
    Aimp[0][0] = 0.94f; Aimp[0][1] = 0.0f;
    Aimp[1][0] = 0.0f;  Aimp[1][1] = 0.94f;

   // Bimp[0] = 0.05f;
   //Bimp[1] = 0.05f;

    Bimp[0] = 0.06f;
    Bimp[1] = 0.06f;

    Cimp[0][0] = 1.0f; Cimp[0][1] = 0.0f;
    Cimp[1][0] = 0.0f; Cimp[1][1] = 1.0f;

    Dimp[0] = 0.0f;
    Dimp[1] = 0.0f;
        //lossVec[0] = 0.9f;  // posición
        //lossVec[1] = 0.1f;  // velocidad
    lossVec[0] = 0.92f;
    lossVec[1] = 0.08;

    // Cimp es identidad → inversa es identidad
    CimpInv[0][0] = 1.0f; CimpInv[0][1] = 0.0f;
    CimpInv[1][0] = 0.0f; CimpInv[1][1] = 1.0f;
}

void KalmanFilter::initializePos(float pos[2]) {
    filtState[0][0] = lossVec[0] * pos[0]; filtState[0][1] = lossVec[1] * pos[0];
    filtState[1][0] = lossVec[0] * pos[1]; filtState[1][1] = lossVec[1] * pos[1];
}

void KalmanFilter::updatePosition(float z[2], float filtOut[2][2]) {
    if (usageCount == 0)
        initializePos(z);
    usageCount++;

    for (int i = 0; i < 2; ++i) {
        filtOut[0][i] = Cimp[i][0]*filtState[0][0] + Cimp[i][1]*filtState[0][1] + Dimp[i]*z[0];
        filtOut[1][i] = Cimp[i][0]*filtState[1][0] + Cimp[i][1]*filtState[1][1] + Dimp[i]*z[1];
    }

    for (int axis = 0; axis < 2; ++axis) {
        // Se omite comparación con segundo filtro, mantenemos filtro único y suave
        float temp[2];
        temp[0] = Aimp[0][0]*filtState[axis][0] + Aimp[0][1]*filtState[axis][1] + Bimp[0]*z[axis];
        temp[1] = Aimp[1][0]*filtState[axis][0] + Aimp[1][1]*filtState[axis][1] + Bimp[1]*z[axis];

        filtState[axis][0] = temp[0];
        filtState[axis][1] = temp[1];
    }
}

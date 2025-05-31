#include "robotstatscollector.hpp"
#include <QtMath>

RobotStatsCollector::RobotStatsCollector(int robotId, int team, float durationThreshold, float epsilon, int minSamples)
    : id_(robotId),
      team_(team),
      timeElapsed_(0.0f),
      durationThreshold_(durationThreshold),
      epsilon_(epsilon),
      minSamples_(minSamples),
      stableCount_(0),
      prevStd_(0, 0, 0) {}

void RobotStatsCollector::update(float x, float y, float theta, float dt) {
    timeElapsed_ += dt;

    x_.append(x);
    y_.append(y);
    theta_.append(normalizeAngle(theta));

    if (x_.size() >= minSamples_) {
        QVector3D currentStd = computeStdDev();
        QVector3D diff = currentStd - prevStd_;

        if (qFabs(diff.x()) < epsilon_ && qFabs(diff.y()) < epsilon_ && qFabs(diff.z()) < epsilon_) {
            stableCount_++;
        } else {
            stableCount_ = 0;
        }

        prevStd_ = currentStd;
    }
}

bool RobotStatsCollector::isReady() const {
    return timeElapsed_ >= durationThreshold_ || stableCount_ >= 5;
}

QVector3D RobotStatsCollector::getStandardDeviations() const {
    return computeStdDev();
}

int RobotStatsCollector::robotId() const { return id_; }
int RobotStatsCollector::team() const { return team_; }

QVector3D RobotStatsCollector::computeStdDev() const {
    auto std = [](const QVector<float>& data) -> float {
        if (data.size() < 2) return 0.0f;
        float mean = std::accumulate(data.begin(), data.end(), 0.0f) / data.size();
        float sumSq = 0.0f;
        for (float val : data)
            sumSq += (val - mean) * (val - mean);
        return std::sqrt(sumSq / (data.size() - 1)) / std::sqrt(2); // Kalman-style std
    };

    return {
        std(x_),
        std(y_),
        std(theta_)
    };
}

float RobotStatsCollector::normalizeAngle(float angle) const {
    return std::fmod(angle + M_PI, 2 * M_PI) - M_PI;
}

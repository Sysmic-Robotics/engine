#include <QVector3D>
#include <QList>

class RobotStatsCollector {
public:
    RobotStatsCollector(int robotId, int team, float durationThreshold = 2.0f, float epsilon = 1e-4, int minSamples = 30);

    void update(float x, float y, float theta, float dt);
    bool isReady() const;
    QVector3D getStandardDeviations() const; // x, y, theta

    int robotId() const;
    int team() const;

private:
    int id_;
    int team_;
    float timeElapsed_;
    float durationThreshold_;
    float epsilon_;
    int minSamples_;

    QList<float> x_, y_, theta_;   // ✅ Correct: separate lists of float values
    QVector3D prevStd_;            // For convergence check
    int stableCount_;

    QVector3D computeStdDev() const;
    float normalizeAngle(float angle) const;
};

#include <QtTest/QtTest>
#include "robotstate.hpp"

class RobotStateTest : public QObject {
    Q_OBJECT
private slots:
    void defaultConstruction();
    void updateFunctions();
};

void RobotStateTest::defaultConstruction() {
    RobotState rs;
    QCOMPARE(rs.getId(), 0);
    QCOMPARE(rs.getTeam(), 0);
    QCOMPARE(rs.getPosition(), QVector2D(0, 0));
    QCOMPARE(rs.getOrientation(), 0.0f);
}

void RobotStateTest::updateFunctions() {
    RobotState rs(1, 0);
    rs.setPosition({1.0f, 2.0f});
    rs.setOrientation(0.5f);
    rs.setVelocity({0.3f, -0.2f});

    QCOMPARE(rs.getPosition(), QVector2D(1.0f, 2.0f));
    QCOMPARE(rs.getOrientation(), 0.5f);
    QCOMPARE(rs.getVelocity(), QVector2D(0.3f, -0.2f));
}

QTEST_MAIN(RobotStateTest)
#include "robotstate_test.moc"

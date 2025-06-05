#include <QtTest/QtTest>
#include "world.hpp"

class WorldTest : public QObject {
    Q_OBJECT
private slots:
    void robotUpdates();
    void jsonSerialization();
};

void WorldTest::robotUpdates() {
    World w(1,1);
    QVector2D p(2.0f,3.0f);
    w.updateRobot(0,0,p,1.0f);
    RobotState r = w.getRobotState(0,0);
    QCOMPARE(r.getPosition(), p);
    QVERIFY(r.isActive());
}

void WorldTest::jsonSerialization() {
    World w(1,0);
    w.updateRobot(0,0,{1.0f,1.0f},0.0f);
    QJsonObject obj = w.toJson();
    QVERIFY(obj.contains("robots"));
    QJsonArray arr = obj["robots"].toArray();
    QCOMPARE(arr.size(), 1);
    QJsonObject r = arr.at(0).toObject();
    QCOMPARE(r["id"].toInt(), 0);
}

QTEST_MAIN(WorldTest)
#include "world_test.moc"

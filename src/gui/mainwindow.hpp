#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QMap>
#include <QVector2D>
#include <QGraphicsPathItem>
#include "robotitem.hpp"
#include "targetmarker.hpp"
#include "robotstate.hpp"
#include "ballstate.hpp"
#include "ballitem.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    

public slots:
    void updateRobot(const RobotState &robotState);
    void updateBall(const BallState &ballState);

signals:
    void robotSelected(int id, int team);
    void targetPointSelected(QVector2D point);
    void faceToDebug(QVector2D target);
    void scriptLoaded(QString filePath);
    void scriptRunRequested();
    void setRobotControl(bool flag);

private:
    void setupLeftPanel();
    void drawTargetMarker(QVector2D point);
    void selectNextRobot(int direction);
    void updateInfoPanel();
    void updateRobotTrace(QVector2D position);
    void clearSelectedRobotTrace();
    void mousePressEvent(QMouseEvent *event);
    void onTargetPointButtonClicked();
    void onFaceToPointButtonClicked();
    void onTraceCheckboxToggled(bool checked);
    void onTabChanged(int index);
    // Script
    void onLoadScriptClicked();
    void onRunScriptClicked();


    QGraphicsScene *scene;
    QGraphicsView *view;
    
    QWidget *leftPanel;
    QVBoxLayout *buttonLayout;
    
    QPushButton *leftArrowBtn;
    QPushButton *rightArrowBtn;
    QPushButton *targetPointBtn;
    QPushButton *faceToBtn;
    QCheckBox *showTraceCheckbox;
    QLabel *robotInfoLabel;
    QPushButton *loadScriptBtn;
    QPushButton *runScriptBtn;

    QMap<int, RobotItem *> blueRobots;
    QMap<int, RobotItem *> yellowRobots;
    BallItem* ball;

    QVector<int> robotIds;
    int selectedRobotIndex = 0;
    int selectedRobotId = -1;
    int selectedTeam = -1;

    TargetMarker *targetMarker = nullptr;
    bool waitingForTargetPoint = false;
    bool waitingForFaceTo = false;

    QPainterPath selectedRobotTrace;   // ✅ Stores the trace for the selected robot
    QGraphicsPathItem *traceItem = nullptr; // ✅ Holds the graphics item for the trace
    bool showTrace = false; // ✅ Tracks whether the trace is enabled
    RobotState selectedRobotState = RobotState(-1, -1);;  // ✅ Store the latest state of the selected robot

};

#endif // MAINWINDOW_HPP

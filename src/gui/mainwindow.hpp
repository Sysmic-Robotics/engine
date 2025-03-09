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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void updateRobot(int id, int team, QVector2D position, float orientation);

signals:
    void robotSelected(int id, int team);
    void targetPointSelected(QVector2D point);

private:
    void setupLeftPanel();
    void drawTargetMarker(QVector2D point);
    void selectNextRobot(int direction);
    void updateInfoPanel();
    void updateRobotTrace(QVector2D position);
    void clearSelectedRobotTrace();
    void mousePressEvent(QMouseEvent *event);
    void onTargetPointButtonClicked();
    void onTraceCheckboxToggled(bool checked);

    QGraphicsScene *scene;
    QGraphicsView *view;
    
    QWidget *leftPanel;
    QVBoxLayout *buttonLayout;
    
    QPushButton *leftArrowBtn;
    QPushButton *rightArrowBtn;
    QPushButton *targetPointBtn;
    QCheckBox *showTraceCheckbox;
    QLabel *robotInfoLabel;

    QMap<int, RobotItem *> blueRobots;
    QMap<int, RobotItem *> yellowRobots;

    QVector<int> robotIds;
    int selectedRobotIndex = 0;
    int selectedRobotId = -1;
    int selectedTeam = -1;

    TargetMarker *targetMarker = nullptr;
    bool waitingForTargetPoint = false;

    QPainterPath selectedRobotTrace;   // ✅ Stores the trace for the selected robot
    QGraphicsPathItem *traceItem = nullptr; // ✅ Holds the graphics item for the trace
    bool showTrace = false; // ✅ Tracks whether the trace is enabled
};

#endif // MAINWINDOW_HPP

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include <QVector2D>
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
    void mousePressEvent(QMouseEvent *event);
    void onTargetPointButtonClicked();

    QGraphicsScene *scene;
    QGraphicsView *view;
    
    QWidget *leftPanel;
    QVBoxLayout *buttonLayout;
    
    QPushButton *leftArrowBtn;
    QPushButton *rightArrowBtn;
    QPushButton *targetPointBtn;
    QLabel *robotInfoLabel;

    QMap<int, RobotItem *> blueRobots;
    QMap<int, RobotItem *> yellowRobots;

    QVector<int> robotIds;
    int selectedRobotIndex = 0;
    int selectedRobotId = -1;
    int selectedTeam = -1;

    TargetMarker *targetMarker = nullptr;
    bool waitingForTargetPoint = false;
};

#endif // MAINWINDOW_HPP

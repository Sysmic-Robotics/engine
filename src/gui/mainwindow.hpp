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
    void updateRobotAppearance(int id, int team, bool isSelected);
    void drawTargetMarker(QVector2D point);
    void selectNextRobot(int direction);
    void updateInfoPanel();

    QGraphicsScene *scene;
    QGraphicsView *view;
    
    QWidget *leftPanel;
    QVBoxLayout *buttonLayout;
    
    QPushButton *leftArrowBtn;
    QPushButton *rightArrowBtn;
    QLabel *robotInfoLabel;

    QMap<int, RobotItem *> blueRobots;
    QMap<int, RobotItem *> yellowRobots;

    QVector<int> robotIds; // Store available robot IDs
    int selectedRobotIndex = 0;
    int selectedRobotId = -1;
    int selectedTeam = -1;

    TargetMarker *targetMarker = nullptr;
};

#endif // MAINWINDOW_HPP

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QMap>
#include "world.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void updateRobot(int id, int team, QVector2D position, float orientation);

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QMap<int, QGraphicsEllipseItem *> blueRobots;
    QMap<int, QGraphicsEllipseItem *> yellowRobots;
};

#endif // MAINWINDOW_HPP

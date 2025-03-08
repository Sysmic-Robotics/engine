#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QMap>
#include <QVector2D>
#include "world.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    
    // Updates robot position, ID, and orientation
    void updateRobot(int id, int team, QVector2D position, float orientation);

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    
    // Maps for tracking robots
    QMap<int, QGraphicsItemGroup *> blueRobots;
    QMap<int, QGraphicsItemGroup *> yellowRobots;
};

#endif // MAINWINDOW_HPP

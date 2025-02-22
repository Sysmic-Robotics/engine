#include "mainwindow.hpp"
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Create scene (9x6 field)
    scene = new QGraphicsScene(0, 0, 900, 600, this);  // 900x600 pixels
    scene->setBackgroundBrush(QBrush(Qt::lightGray)); // Set bright background color
    // Create view
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);

    // Draw field boundaries
    QPen pen(Qt::white);
    scene->addRect(0, 0, 900, 600, pen);

    // Initialize robot items (empty)
}

void MainWindow::updateRobot(int id, int team, QVector2D position, float orientation) {
    QMap<int, QGraphicsEllipseItem *> &robots = (team == 0) ? blueRobots : yellowRobots;
    QColor color = (team == 0) ? Qt::blue : Qt::yellow;

    if (!robots.contains(id)) {
        // Create a new robot item
        QGraphicsEllipseItem *robot = scene->addEllipse(0, 0, 20, 20, QPen(Qt::black), QBrush(color));
        robots.insert(id, robot);
    }

    // Convert 9x6 field coordinates to 900x600 pixel space
    float x = (position.x() + 4.5) * 100; // Centered at 450
    float y = (-position.y() + 3.0) * 100; // Centered at 300

    robots[id]->setPos(x, y);
}

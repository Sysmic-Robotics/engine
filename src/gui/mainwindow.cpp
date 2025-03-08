#include "mainwindow.hpp"
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QBrush>
#include <QPen>
#include <QTransform>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Create scene (9x6 field)
    scene = new QGraphicsScene(0, 0, 900, 600, this);
    scene->setBackgroundBrush(QBrush(Qt::lightGray));

    // Create view
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);

    // Draw field boundaries
    QPen pen(Qt::white);
    scene->addRect(0, 0, 900, 600, pen);
}

void MainWindow::updateRobot(int id, int team, QVector2D position, float orientation) {
    QMap<int, QGraphicsItemGroup *> &robots = (team == 0) ? blueRobots : yellowRobots;
    QColor color = (team == 0) ? Qt::blue : Qt::yellow;

    QGraphicsItemGroup *robotGroup;

    if (!robots.contains(id)) {
        // Create a group for the robot
        robotGroup = new QGraphicsItemGroup();

        // Create robot ellipse
        QGraphicsEllipseItem *robot = new QGraphicsEllipseItem(-10, -10, 20, 20); // Centered at (0,0)
        robot->setBrush(QBrush(color));
        robot->setPen(QPen(Qt::black));

        // Create robot ID text
        QGraphicsTextItem *text = new QGraphicsTextItem(QString::number(id));
        text->setDefaultTextColor(Qt::white);
        text->setFont(QFont("Arial", 10, QFont::Bold));
        text->setPos(-6, -6); // Center text

        // Create orientation line (initially straight up)
        QGraphicsLineItem *orientationLine = new QGraphicsLineItem(0, 0, 10, 0, robotGroup); // ✅ Inside the group
        orientationLine->setPen(QPen(Qt::red, 2));  // Red line for visibility

        // Add items to group
        robotGroup->addToGroup(robot);
        robotGroup->addToGroup(text);
        robotGroup->addToGroup(orientationLine);

        // Add to scene and map
        scene->addItem(robotGroup);
        robots.insert(id, robotGroup);
    } else {
        robotGroup = robots[id]; // Retrieve existing group
    }

    // Convert 9x6 field coordinates to 900x600 pixel space
    float x = (position.x() + 4.5) * 100; // Centered at 450
    float y = (-position.y() + 3.0) * 100; // Centered at 300

    // Move robot group
    robotGroup->setPos(x, y);

    // Find the orientation line inside the group (it was added first)
    QGraphicsLineItem *orientationLine = static_cast<QGraphicsLineItem *>(robotGroup->childItems().last());

    // Calculate new end point based on orientation
    float lineLength = 10; // Length of the orientation line
    float dx = lineLength * qCos(orientation);
    float dy = lineLength * qSin(orientation);

    // Update line position (center remains at (0,0) relative to the group)
    orientationLine->setLine(0, 0, dx, -dy);
}

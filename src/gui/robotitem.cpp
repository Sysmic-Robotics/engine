#include "robotitem.hpp"
#include <QBrush>
#include <QPen>
#include <QtMath>
#include <QFont>

RobotItem::RobotItem(int id, QColor color) : robotId(id) {
    // Create the robot shape
    robotShape = new QGraphicsEllipseItem(-10, -10, 20, 20);
    robotShape->setBrush(QBrush(color));
    robotShape->setPen(QPen(Qt::black));
    robotShape->setData(0, id); // Store ID inside the item

    // Create orientation line
    orientationLine = new QGraphicsLineItem(0, 0, 10, 0);
    orientationLine->setPen(QPen(Qt::red, 2));

    // Create ID text
    idText = new QGraphicsTextItem(QString::number(id));
    idText->setDefaultTextColor(Qt::white);
    idText->setFont(QFont("Arial", 10, QFont::Bold));
    idText->setPos(-6, -6);

    // Add items to the group
    addToGroup(robotShape);
    addToGroup(orientationLine);
    addToGroup(idText);
}

void RobotItem::setPosition(QVector2D position) {
    float x = (position.x() + 4.5) * 100;
    float y = (-position.y() + 3.0) * 100;
    setPos(x, y);
}

void RobotItem::setOrientation(float angle) {
    float lineLength = 10;
    float dx = lineLength * qCos(angle);
    float dy = lineLength * qSin(angle);
    orientationLine->setLine(0, 0, dx, -dy);
}

void RobotItem::setSelected(bool selected) {
    QColor defaultColor = (robotId < 6) ? Qt::blue : Qt::yellow;
    robotShape->setBrush(selected ? QBrush(Qt::red) : QBrush(defaultColor));
}


#include "colors.hpp"
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
    orientationLine->setPen(QPen(GUIColors::ORIENTATION_COLOR, 1));

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
    QColor defaultColor = (robotId < 6) ? GUIColors::BLUE_ROBOT : GUIColors::YELLOW_ROBOT;

    // ✅ Make the color brighter when selected
    QColor highlightColor = defaultColor.lighter(150); // 150% brightness

    robotShape->setBrush(QBrush(selected ? highlightColor : defaultColor));
}

QVector2D RobotItem::getPosition() {
    return QVector2D(this->x(), this->y());  // ✅ Use `this->x()` and `this->y()` to get scene position
}



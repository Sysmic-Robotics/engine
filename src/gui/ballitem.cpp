#include "ballitem.hpp"
#include "colors.hpp"
#include <QBrush>

BallItem::BallItem() : QGraphicsEllipseItem(-5, -5, 5, 5) {
    setBrush(QBrush(GUIColors::BALL));  // Ball color (white)
}

void BallItem::setPosition(QVector2D position) {
    float x = (position.x() + 4.5) * 100;
    float y = (-position.y() + 3.0) * 100;
    setPos(x, y);
}



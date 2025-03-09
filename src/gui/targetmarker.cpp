#include "targetmarker.hpp"
#include <QBrush>  // ✅ Include QBrush to avoid incomplete type error
#include <QPen>    // ✅ Include QPen

TargetMarker::TargetMarker() : QGraphicsEllipseItem(-5, -5, 10, 10) {
    setPen(QPen(Qt::black));
    setBrush(QBrush(Qt::green));
}

void TargetMarker::setPosition(QVector2D point) {
    float x = (point.x() + 4.5) * 100;
    float y = (-point.y() + 3.0) * 100;
    setPos(x, y);
}

#ifndef TARGETMARKER_HPP
#define TARGETMARKER_HPP

#include <QGraphicsEllipseItem>
#include <QVector2D>

class TargetMarker : public QGraphicsEllipseItem {
public:
    TargetMarker();
    void setPosition(QVector2D point);
};

#endif // TARGETMARKER_HPP

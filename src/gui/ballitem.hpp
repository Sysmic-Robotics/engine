#ifndef BALLITEM_HPP
#define BALLITEM_HPP

#include <QGraphicsEllipseItem>
#include <QVector2D>

class BallItem : public QGraphicsEllipseItem {
public:
    BallItem();  // Constructor

    void setPosition(QVector2D position);

private:
    QGraphicsEllipseItem *ballShape;  // Represents the ball
};

#endif // BALLITEM_HPP

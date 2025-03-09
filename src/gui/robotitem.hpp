#ifndef ROBOTITEM_HPP
#define ROBOTITEM_HPP

#include <QGraphicsItemGroup>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QVector2D>

class RobotItem : public QGraphicsItemGroup {
public:
    RobotItem(int id, QColor color);

    void setPosition(QVector2D position);
    void setOrientation(float angle);
    void setSelected(bool selected);

    int getId() const { return robotId; }
    QVector2D getPosition();

private:
    int robotId;
    QGraphicsEllipseItem *robotShape;
    QGraphicsLineItem *orientationLine;
    QGraphicsTextItem *idText;
};

#endif // ROBOTITEM_HPP

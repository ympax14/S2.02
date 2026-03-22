#ifndef CITYITEM_HPP
#define CITYITEM_HPP

#include "../Map/City/City.hpp"

#include <QBrush>
#include <QGraphicsEllipseItem>

class CityItem : public QGraphicsEllipseItem
{
public:
    const City* const city;
    const double x, y, size;
    const QColor color;

    CityItem(const City* const _city, double _x, double _y, double size = 20, const QColor& _color = Qt::blue);

    void setLabelVisible(bool visible);
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QGraphicsTextItem* label;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
};

#endif // CITYITEM_HPP

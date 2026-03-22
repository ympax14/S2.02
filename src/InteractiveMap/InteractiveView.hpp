#ifndef INTERACTIVEVIEW_HPP
#define INTERACTIVEVIEW_HPP

#include <QGraphicsView>
#include <QWheelEvent>

class InteractiveView : public QGraphicsView {
    Q_OBJECT
public:
    static const double defaultScale;
    static const double scaleFactor;

    InteractiveView(QGraphicsScene* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent* event) override;

signals:
    void zoomChanged(qreal scale);
};

#endif // INTERACTIVEVIEW_HPP

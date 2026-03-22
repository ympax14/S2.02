#include "InteractiveView.hpp"

// Vue panoramique dès l'ouverture de l'application
const double InteractiveView::defaultScale = 0.29;
const double InteractiveView::scaleFactor = 1.15;

InteractiveView::InteractiveView(QGraphicsScene* parent) :
    QGraphicsView(parent)
{
    this->setDragMode(QGraphicsView::ScrollHandDrag); // pour déplacer avec la souris
    this->setRenderHint(QPainter::Antialiasing); // Pour un rendu plus lisse
    this->setTransform(QTransform::fromScale(InteractiveView::defaultScale, InteractiveView::defaultScale));
}

void InteractiveView::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        this->scale(InteractiveView::scaleFactor, InteractiveView::scaleFactor); // zoom avant
    } else {
        this->scale(1.0 / InteractiveView::scaleFactor, 1.0 / InteractiveView::scaleFactor); // zoom arrière
    }

    emit zoomChanged(this->transform().m11()); // m11 = scale X courant
}

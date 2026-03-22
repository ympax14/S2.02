#ifndef RIDETAB_HPP
#define RIDETAB_HPP

#include "../../../Map/Route/Route.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsView>

class RideTab : public QWidget
{
    Q_OBJECT
private:
    static const int RIDE_PREVIEW_CITY_RADIUS;
    static const int RIDE_PREVIEW_CITIES_SPACING;
    static const double RIDE_PREVIEW_ARROW_SIZE;

    QVBoxLayout * const layout;
    QLabel * const label;

    QHBoxLayout * const rideSelectionLayout;
    QComboBox * const comboRideStart, * const comboRideEnd;

    QHBoxLayout * const rideBtnLayout;
    QPushButton * const calculateRideBtn, * const clearRideBtn;

    QVBoxLayout * const ridePreviewLayout;
    QGraphicsScene * const ridePreviewScene;
    QGraphicsView * const ridePreviewView;
public:
    RideTab(QWidget* const parent = nullptr);

    void drawRide(const Route* const ride = nullptr);
    void drawNode(qreal x, qreal y, const QString& text);
    void drawDirectedEdge(QPointF start, QPointF end, int weight);
public slots:
    void calculateRide();
    void clearRide();
};

#endif // RIDETAB_HPP

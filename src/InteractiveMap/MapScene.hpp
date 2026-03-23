#ifndef MAPSCENE_HPP
#define MAPSCENE_HPP

#include "../Map/Map.hpp"
#include "InteractiveView.hpp"

#include <vector>

#include "CityItem.hpp"

#include <QGraphicsScene>

class MapScene : public QGraphicsScene
{
private:
    Map* const map;

    const std::vector<std::vector<std::pair<double, double>>> borderPolygon;
    const std::vector<std::pair<QString, std::vector<std::pair<double, double>>>> regionsPolygons;

    std::vector<QGraphicsPolygonItem*> backgroundItems;
    std::vector<CityItem*> cityItems;
    std::vector<QGraphicsLineItem*> routesItems;
    std::vector<QGraphicsItem*> rideItems;
public:
    static const double MIN_LAT; // sud
    static const double MAX_LAT; // nord
    static const double MIN_LNG; // ouest
    static const double MAX_LNG; // est

    static const double SCENE_WIDTH;
    static const double SCENE_HEIGHT;

    static bool HIDE_CITIES;

    static const int VILLAGE_SIZE;
    static const double VILLAGE_DOT_VISIBLE_AT;
    static const double VILLAGE_LABEL_VISIBLE_AT;

    static const int SMALL_CITY_SIZE;
    static const double SMALL_CITY_DOT_VISIBLE_AT;
    static const double SMALL_CITY_LABEL_VISIBLE_AT;

    static const int MEDIUM_CITY_SIZE;
    static const double MEDIUM_CITY_DOT_VISIBLE_AT;
    static const double MEDIUM_CITY_LABEL_VISIBLE_AT;

    static const int BIG_CITY_SIZE;
    static const double BIG_CITY_DOT_VISIBLE_AT;
    static const double BIG_CITY_LABEL_VISIBLE_AT;

    static bool HIDE_PATHS;
    static double PATH_SIZE;

    MapScene(Map* _map, QObject * const parent = nullptr);

    static QColor colorForAdmin(const QString& adminName);
    static int getCitySize(const double& population);

    inline const qreal& getScale() const { return InteractiveView::scaleFactor; };

    QPointF toScene(double lat, double lng);
    QGraphicsPolygonItem* addPolygon(const std::vector<std::pair<double,double>>& points, const QBrush& brush, const QPen& pen, const int& z = 0);

    void buildBackground(const bool& clearScene = false);
    void buildCities(const bool& clearScene = false);
    void buildRoutes(const bool& clearScene = false);
    void buildRide(const bool& clearScene = false);
    void build();
public slots:
    void onZoomChanged(qreal scale);
};

#endif // MAPSCENE_HPP

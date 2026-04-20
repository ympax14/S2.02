#include "MapScene.hpp"

#include "../NavyraWindow.hpp"
#include "../Map/Country/Country.hpp"
#include "../Map/Administration/Administration.hpp"
#include "../Map/City/City.hpp"
#include "../utils/Deserialize.hpp"

#include <cmath>

// Bornes géographiques approximatives de la France métropolitaine
const double MapScene::MIN_LAT = 41.3; // sud
const double MapScene::MAX_LAT = 51.1; // nord
const double MapScene::MIN_LNG = -5.2; // ouest
const double MapScene::MAX_LNG = 9.7; // est

const double MapScene::SCENE_WIDTH = 3000;
const double MapScene::SCENE_HEIGHT = 2000;

bool MapScene::HIDE_CITIES = false;

const int MapScene::VILLAGE_SIZE = 10;
const double MapScene::VILLAGE_DOT_VISIBLE_AT = 2.5;
const double MapScene::VILLAGE_LABEL_VISIBLE_AT = 3.5;

const int MapScene::SMALL_CITY_SIZE = 16;
const double MapScene::SMALL_CITY_DOT_VISIBLE_AT = 1.2;
const double MapScene::SMALL_CITY_LABEL_VISIBLE_AT = 2.0;

const int MapScene::MEDIUM_CITY_SIZE = 24;
const double MapScene::MEDIUM_CITY_DOT_VISIBLE_AT = 0.0; // Toujours visible...
const double MapScene::MEDIUM_CITY_LABEL_VISIBLE_AT = 0.0; // Idem...

const int MapScene::BIG_CITY_SIZE = 36;
const double MapScene::BIG_CITY_DOT_VISIBLE_AT = 0.0; // Idem...
const double MapScene::BIG_CITY_LABEL_VISIBLE_AT = 0.0; // Idem...

bool MapScene::HIDE_PATHS = false;
double MapScene::PATH_SIZE = 0.7;

MapScene::MapScene(Map* _map, QObject * const parent) :
    QGraphicsScene{parent},
    map(_map),

    borderPolygon(Deserialize::loadGeoJsonBorders(":/assets/datas/metropole-version-simplifiee.geojson")),
    regionsPolygons(Deserialize::loadGeoJsonRegions(":/assets/datas/regions-version-simplifiee.geojson"))
{}

int MapScene::getCitySize(const double& population) {
    if (population > 1'000'000) return MapScene::BIG_CITY_SIZE;
    if (population > 200'000) return MapScene::MEDIUM_CITY_SIZE;
    if (population > 50'000) return MapScene::SMALL_CITY_SIZE;
    return MapScene::VILLAGE_SIZE;
}

// Génère une couleur "stable" par administration
QColor MapScene::colorForAdmin(const QString& adminName) {
    uint hash = qHash(adminName);
    int h = hash % 360;
    return QColor::fromHsv(h, 160, 220); // pastel saturé
}

QPointF MapScene::toScene(double lat, double lng) {
    // Latitude moyenne de la France (environ 46.5 degrés)
    const double meanLatRad = 46.5 * M_PI / 180.0;

    // On ajuste la longitude pour "rétrécir" la carte horizontalement
    // et lui donner sa vraie proportion visuelle
    double lngAdjusted = lng * std::cos(meanLatRad);
    double minLngAdjusted = MapScene::MIN_LNG * std::cos(meanLatRad);
    double maxLngAdjusted = MapScene::MAX_LNG * std::cos(meanLatRad);

    qreal x = (lngAdjusted - minLngAdjusted) / (maxLngAdjusted - minLngAdjusted) * MapScene::SCENE_WIDTH;
    qreal y = MapScene::SCENE_HEIGHT - (lat - MapScene::MIN_LAT) / (MapScene::MAX_LAT - MapScene::MIN_LAT) * MapScene::SCENE_HEIGHT;

    return QPointF(x, y);
}

QGraphicsPolygonItem* MapScene::addPolygon(const std::vector<std::pair<double,double>>& points, const QBrush& brush, const QPen& pen, const int& z) {
    QPolygonF polygon;

    for (const auto& [lat, lng] : points)
        polygon << toScene(lat, lng);

    QGraphicsPolygonItem* item = new QGraphicsPolygonItem(polygon);

    item->setBrush(brush);
    item->setPen(pen);
    item->setZValue(z);

    this->addItem(item);

    return item;
}

void MapScene::buildBackground(const bool& clearScene) {
    if (clearScene) {
        for (QGraphicsPolygonItem* item : this->backgroundItems)
            this->removeItem(item);

        this->backgroundItems.clear();
        this->update();
    }

    // Trait de séparation des régions
    QPen regionPen(QColor(Qt::black), 1);

    for (const std::pair<QString, std::vector<std::pair<double, double>>>& regionPair : this->regionsPolygons) {
        QString nomRegion = regionPair.first;
        auto polygon = regionPair.second;

        // On génère la couleur basée sur le nom de la région
        QColor adminColor = MapScene::colorForAdmin(nomRegion);
        adminColor.setAlpha(120);

        QBrush regionBrush(adminColor);

        this->backgroundItems.push_back(this->addPolygon(polygon, regionBrush, regionPen, 0));
    }

    // Frontières
    QPen borderPen(QColor(Qt::black), 2);
    for (const std::vector<std::pair<double, double>>& poly : this->borderPolygon) {
        this->backgroundItems.push_back(this->addPolygon(poly, Qt::NoBrush, borderPen, 1));
    }
}

void MapScene::buildRoutes(const bool& clearScene) {
    if (clearScene) {
        for (QGraphicsLineItem* item : this->routesItems)
            this->removeItem(item);

        this->routesItems.clear();
        this->update();
    }

    if (MapScene::HIDE_PATHS) return;

    // Un trait gris foncé pour représenter les routes
    QPen routePen(QColor("#7F8C8D"), MapScene::PATH_SIZE);

    // Style de la ligne : pointillés. Ou utiliser Qt::SolidLine pour traits pleins
    routePen.setStyle(Qt::DashLine);

    // L'épaisseur restera de PATH_SIZE pixels, peu importe le zoom
    routePen.setCosmetic(true);

    for (const auto& [countryName, countryPtr] : this->map->getCountries()) {
        for (const auto& [adminName, adminPtr] : countryPtr->getAdministrations()) {
            for (const auto& [cityName, cityPtr] : adminPtr->getCities()) {

                QPointF startPos = this->toScene(cityPtr->getLat(), cityPtr->getLng());

                // Pour chaque voisin de cette ville
                for (const auto& [neighborCity, weight] : cityPtr->getNeighbors()) {

                    // Astuce : pour ne pas tracer la ligne 2 fois (A->B et B->A),
                    // on ne trace que si l'ID de destination est plus grand.
                    if (neighborCity->getId() > cityPtr->getId()) {

                        QPointF endPos = this->toScene(neighborCity->getLat(), neighborCity->getLng());

                        QGraphicsLineItem* line = new QGraphicsLineItem(startPos.x(), startPos.y(), endPos.x(), endPos.y());
                        line->setPen(routePen);

                        // au dessus des régions (1) mais sous les trajets (3) et les villes (4
                        line->setZValue(2);

                        this->routesItems.push_back(line);
                        this->addItem(line);
                    }
                }
            }
        }
    }

    qDebug() << "[InteractiveMap] Routes dessinées ! Nombre de routes : " << this->routesItems.size();
}

void MapScene::buildCities(const bool& clearScene) {
    if (clearScene) {
        for (CityItem* item : this->cityItems)
            this->removeItem(item);

        this->cityItems.clear();
        this->update();
    }

    if (MapScene::HIDE_CITIES) return;

    for (const auto& [countryName, countryPtr] : this->map->getCountries()) {
        for (const auto& [adminName, adminPtr] : countryPtr->getAdministrations()) {
            for (const auto& [cityName, cityPtr] : adminPtr->getCities()) {

                // Conversion latitude/longitude -> coordonnées X/Y
                QPointF pos = this->toScene(cityPtr->getLat(), cityPtr->getLng());

                CityItem* item = new CityItem(cityPtr, pos.x(), pos.y(), MapScene::getCitySize(cityPtr->getPopulation()), MapScene::colorForAdmin(adminName));

                this->addItem(item);
                this->cityItems.push_back(item);
            }
        }
    }

    qDebug() << "[InteractiveMap] Villes dessinées ! Nombre de villes : " << this->cityItems.size();
}

void MapScene::buildRide(const bool& clearScene) {
    if (clearScene) {
        for (QGraphicsItem* item : this->rideItems)
            this->removeItem(item);

        this->rideItems.clear();
        this->update();
    }

    const Route* currentRide = NavyraWindow::getMap().getCurrentRide();

    if (currentRide != nullptr && !currentRide->getSegments().empty()) {
        // Un trait rouge pour représenter le trajet
        QPen ridePen(QColor("#FF0000"), MapScene::PATH_SIZE * 2);

        // Style de la ligne : traits pleins. Ou utiliser Qt::DashLine pour des pointillés
        ridePen.setStyle(Qt::SolidLine);

        // L'épaisseur restera de PATH_SIZE * 2 pixels, peu importe le zoom
        ridePen.setCosmetic(true);

        for (const Path& path : currentRide->getSegments()) {
            const City* from = path.getFrom();
            const City* to = path.getTo();

            QPointF startPos = this->toScene(from->getLat(), from->getLng());
            QPointF endPos = this->toScene(to->getLat(), to->getLng());

            QGraphicsLineItem* line = new QGraphicsLineItem(startPos.x(), startPos.y(), endPos.x(), endPos.y());
            line->setPen(ridePen);
            line->setZValue(3);
            this->addItem(line);
            this->rideItems.push_back(line);

            // Affichage du poids
            QPointF midPoint((startPos.x() + endPos.x()) / 2.0, (startPos.y() + endPos.y()) / 2.0);
            QGraphicsRectItem* anchor = new QGraphicsRectItem(0, 0, 0, 0);
            anchor->setPos(midPoint);
            anchor->setZValue(4); // L'ancre gère le Z-Value pour le texte
            anchor->setFlag(QGraphicsItem::ItemIgnoresTransformations);
            this->addItem(anchor);
            this->rideItems.push_back(anchor);

            // Convertir le poids en chaîne de caractères
            QString weightStr = QString::number(path.getWeight());
            QGraphicsSimpleTextItem* weightText = new QGraphicsSimpleTextItem(weightStr, anchor);

            QFont font = weightText->font();
            font.setBold(true);
            font.setPointSize(10);
            weightText->setFont(font);
            weightText->setBrush(QBrush(Qt::black)); // Texte en noir

            // Centrer parfaitement le texte sur le point milieu
            QRectF textRect = weightText->boundingRect();
            weightText->setPos(-textRect.width() / 2.0, -textRect.height() / 2.0);
        }

        qDebug() << "[InteractiveMap] Trajet dessiné ! De" << currentRide->getSegments()[0].getFrom()->getName() << "à" << currentRide->getSegments()[currentRide->getSegments().size()-1].getTo()->getName() << QString("(%1min)").arg(currentRide->getTotalWeight());
    }
}

void MapScene::build() {
    this->clear();

    this->buildBackground(false);
    this->buildCities(false);
    this->buildRoutes(false);
    this->buildRide(false);

    this->onZoomChanged(InteractiveView::scaleFactor); // Applique le LOD
}

void MapScene::onZoomChanged(qreal scale) {
    for (CityItem* city : this->cityItems) {
        bool visible = true;
        bool labelVisible = false;

        // LOD : Visibilité dynamique selon la taille de la ville ET le niveau de zoom
        if (city->size <= MapScene::VILLAGE_SIZE) {
            visible = (scale >= MapScene::VILLAGE_DOT_VISIBLE_AT);
            labelVisible = (scale >= MapScene::VILLAGE_LABEL_VISIBLE_AT);
        } else if (city->size <= MapScene::SMALL_CITY_SIZE) {
            visible = (scale >= MapScene::SMALL_CITY_DOT_VISIBLE_AT);
            labelVisible = (scale >= MapScene::SMALL_CITY_LABEL_VISIBLE_AT);
        } else if (city->size <= MapScene::MEDIUM_CITY_SIZE) {
            visible = (scale >= MapScene::MEDIUM_CITY_DOT_VISIBLE_AT);
            labelVisible = (scale >= MapScene::MEDIUM_CITY_LABEL_VISIBLE_AT);
        } else {
            visible = (scale >= MapScene::BIG_CITY_DOT_VISIBLE_AT);
            labelVisible = (scale >= MapScene::BIG_CITY_LABEL_VISIBLE_AT);
        }

        city->setVisible(visible);

        // On affiche le label seulement si la ville est elle-même visible
        if (visible) {
            city->setLabelVisible(labelVisible);
        } else {
            city->setLabelVisible(false);
        }
    }

    qDebug() << "[InteractiveMap] Échelle de Zoom : " << scale;
}
#include "CityItem.hpp"

#include "../NavyraWindow.hpp"
#include "../Map/Administration/Administration.hpp"
#include "../Map/Country/Country.hpp"

#include <QFont>

CityItem::CityItem(const City* const _city, double _x, double _y, double _size, const QColor& _color) :
    QGraphicsEllipseItem(-_size/2, -_size/2, _size, _size),  // rect centré sur (0,0) local

    city(_city),
    x(_x),
    y(_y),
    size(_size),
    color(_color),

    label(new QGraphicsTextItem(this->city->getName(), this))
{
    setPos(_x, _y);
    setBrush(QBrush(color));

    // Permet de garder une taille fixe pour les éléments, cela évite d'avoir toutes les villes de collées
    setFlag(QGraphicsItem::ItemIgnoresTransformations);

    setToolTip(this->city->getName());

    setAcceptHoverEvents(true);
    setZValue(5); // passe au-dessus des autres items

    this->label->setFont(QFont("Arial", 7, QFont::Bold));

    // Un gris très foncé/bleuté, beaucoup plus lisible sur fond clair
    this->label->setDefaultTextColor(QColor("#2C3E50"));

    this->label->setPos(size/2 + 2, -size/2);  // relatif au centre de la ville
    this->label->setVisible(false);
}

void CityItem::setLabelVisible(bool visible) {
    label->setVisible(visible);
}

void CityItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    qDebug() << "Ville cliquée:" << this->city->getName();

    QString text = QString(R"(
    --- Ville ---
    Nom: %1
    Latitude: %2
    Longitude: %3
    Densité: %4m²
    Population Ville: %5 habitants
    Population Ville + Agglomération: %6 habitants

    --- Administration ---
    Nom: %7
    Code: %8

    --- Pays ---
    Nom: %9
    Iso2: %10
    Iso3: %11
    )")
    .arg(this->city->getName())
    .arg(this->city->getLat(), 0, 'f', 2)
    .arg(this->city->getLng(), 0, 'f', 2)
    .arg(this->city->getDensity(), 0, 'f', 2)
    .arg(this->city->getPopulationProper())
    .arg(this->city->getPopulation())

    .arg(this->city->getAdministration()->getName())
    .arg(this->city->getAdministration()->getCode())

    .arg(this->city->getCountry()->getName())
    .arg(this->city->getCountry()->getIso2())
    .arg(this->city->getCountry()->getIso3());

    NavyraWindow::getInstance()->getControlPanel()->getCityInformationsTab()->getLabel()->setText(text);

    QGraphicsEllipseItem::mousePressEvent(event);
}

void CityItem::hoverEnterEvent(QGraphicsSceneHoverEvent*) {
    setBrush(QBrush(QColor("#FF6B6B")));
    setScale(1.5);
    label->setVisible(true);
}
void CityItem::hoverLeaveEvent(QGraphicsSceneHoverEvent*) {
    setBrush(QBrush(this->color));
    setScale(1.0);
    // label->setVisible(InteractiveView::scaleFactor > MapScene::CITY_LABEL_VISIBLE_AT);
}
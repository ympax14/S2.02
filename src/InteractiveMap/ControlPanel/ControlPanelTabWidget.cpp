#include "ControlPanelTabWidget.hpp"

ControlPanelTabWidget::ControlPanelTabWidget(QWidget* const parent) :
    QTabWidget{parent},

    cityInformationsTab(new CityInformationsTab(this)),
    rideTab(new RideTab(this)),
    ridesTimeTableTab(new RidesTimeTableTab(this))
{
    this->addTab(this->cityInformationsTab, "Informations de la Ville");
    this->addTab(this->rideTab, "Recherche de Trajet");
    this->addTab(this->ridesTimeTableTab, "Tableau des Temps");

    // On donne une préférée au panneau de droite pour qu'il ne soit pas trop large
    this->setMinimumWidth(300);
    this->setMaximumWidth(400);
}

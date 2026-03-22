#include "CityInformationsTab.hpp"

CityInformationsTab::CityInformationsTab(QWidget *parent) :
    QWidget{parent},

    layout(new QVBoxLayout(this)),
    label(new QLabel("Aucune ville sélectionnée."))
{
    this->layout->addWidget(this->label);
    this->layout->addStretch();
}

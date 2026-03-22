#ifndef CONTROLPANELTABWIDGET_H
#define CONTROLPANELTABWIDGET_H

#include "CityInformations/CityInformationsTab.hpp"
#include "Ride/RideTab.hpp"
#include "RidesTimeTable/RidesTimeTableTab.hpp"

#include <QTabWidget>
#include <QVBoxLayout>

class ControlPanelTabWidget : public QTabWidget
{
    Q_OBJECT
private:
    CityInformationsTab * const cityInformationsTab;
    RideTab * const rideTab;
    RidesTimeTableTab * const ridesTimeTableTab;

public:
    ControlPanelTabWidget(QWidget* const parent);

    inline CityInformationsTab* getCityInformationsTab() const { return this->cityInformationsTab; };
    inline RideTab* getRideTab() const { return this->rideTab; };
    inline RidesTimeTableTab getRidesTimeTableTab() const { return this->ridesTimeTableTab; };
};

#endif // CONTROLPANELTABWIDGET_H

#include "Country.hpp"

Country::Country(const QString& _name, const QString& _iso2, const QString& _iso3) :
    name(_name),
    iso2(_iso2),
    iso3(_iso3)
{}

Country::~Country() {
    for (std::map<QString, Administration*>::iterator it = this->administrations.begin(); it != this->administrations.end(); it++)
        delete it->second;
}

Administration* Country::getAdministration(const QString& adminName) const {
    std::map<QString, Administration*>::const_iterator itAdmin = this->administrations.find(adminName);

    return itAdmin == this->administrations.end() ? nullptr : itAdmin->second;
}

void Country::addAdministration(Administration* const administration) {
    std::map<QString, Administration*>::iterator itAdmin = this->administrations.find(administration->getName());

    if (itAdmin != this->administrations.end()) return;

    this->administrations[administration->getName()] = administration;
}
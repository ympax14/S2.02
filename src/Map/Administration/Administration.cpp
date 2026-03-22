#include "Administration.hpp"
#include "../City/City.hpp"

Administration::Administration(const QString& _name, const QString& _name_ascii, const QString &_code, const Country* _country) :
    name(_name),
    name_ascii(_name_ascii),
    code(_code),
    country(_country)
{}

Administration::~Administration() {
    for (std::map<QString, City*>::iterator it = this->cities.begin(); it != this->cities.end(); it++)
        delete it->second;
};

const City* Administration::getCity(const QString& name) const {
    auto it = this->cities.find(name);

    return (it != this->cities.end()) ? it->second : nullptr;
}

void Administration::addCity(City* const city) {
    this->cities[city->getName()] = city;
}
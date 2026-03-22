#include "City.hpp"

#include "../Country/Country.hpp"
#include "../Administration/Administration.hpp"

City::City(const int& _id, const QString& _name, const double& _lat, const double& _lng, const double _density, const int& _population, const int& _population_proper, const Country* const _country, const Administration* _administration) :
    id(_id),
    name(_name),
    lat(_lat),
    lng(_lng),
    density(_density),
    population(_population),
    population_proper(_population_proper),
    country(_country),
    administration(_administration)
{}

City::~City() = default;

void City::addNeighbor(const City* city, int weight) {
    this->neighbors[city] = weight;
}

int City::getNeighborWeight(const City* const &city) const {
    std::unordered_map<const City*, int>::const_iterator it = neighbors.find(city);

    return it == this->neighbors.end() ? -1 : it->second;
}

bool City::hasNeighbor(const City* const &city) const {
    return this->getNeighborWeight(city) != -1;
}
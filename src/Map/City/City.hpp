#ifndef CITY_HPP
#define CITY_HPP

#include <unordered_map>

#include <QString>

class Country;
class Administration;

class City
{

private:
    const int id;
    const QString name;
    const double lat, lng, density;
    const int population, population_proper;
    const Country* const country;
    const Administration* const administration;

    std::unordered_map<const City*, int> neighbors; // voisin → poids
public:
    City(const int& _id, const QString& _name, const double& _lat = 0.0, const double& _lng = 0.0, const double _density = 0.0, const int& _population = 0, const int& _population_proper = 0, const Country* const _country = nullptr, const Administration* _administration = nullptr);
    ~City();

    inline const int& getId() const { return this->id; };
    inline const QString& getName() const { return this->name; };

    inline const double& getLat() const { return this->lat; };
    inline const double& getLng() const { return this->lng; };
    inline const double& getDensity() const { return this->density; };

    inline const int& getPopulation() const { return this->population; };
    inline const int& getPopulationProper() const { return this->population_proper; };

    void addNeighbor(const City* city, int weight);
    inline const std::unordered_map<const City*, int>& getNeighbors() const { return neighbors; }
    int getNeighborWeight(const City* const &city) const;
    bool hasNeighbor(const City* const &city) const;

    inline const Country* getCountry() const { return this->country; };
    inline const Administration* getAdministration() const { return this->administration; };
};

#endif // CITY_HPP
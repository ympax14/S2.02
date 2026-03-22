#ifndef MAP_H
#define MAP_H

#include "Route/Route.hpp"

#include <map>

#include <QString>

class City;
class Country;

class Map
{
private:
    std::map<QString, Country*> countries;
    std::unordered_map<int, City*> citiesById;
    const Route * currentRide;

    // Structures pour l'algorithme de Floyd-Warshall
    std::vector<City*> cityIndex; // Permet de lier l'index 0..n-1 à un City*
    std::unordered_map<const City*, int> cityToIndex; // L'inverse : City* -> index 0..n-1

    std::vector<std::vector<int>> distMatrix; // Matrice W des poids
    std::vector<std::vector<const City*>> nextMatrix; // Matrice pour reconstruire les chemins
public:
    Map();
    ~Map();

    inline const std::map<QString, Country*>& getCountries() const { return this->countries; };

    Country* getCountry(const QString& countryName) const;
    void addCountry(Country* const country);

    inline const Route* getCurrentRide() const { return this->currentRide; };
    inline void setCurrentRide(const Route* ride) { this->currentRide = ride; };
    inline void clearCurrentRide() { this->currentRide = nullptr; };

    void registerCityId(int id, City* city);
    City* getCityById(int id) const;

    // Méthodes de l'algorithme
    void computeShortestPaths();
    Route* getShortestRoute(const City* from, const City* to);
};

#endif // MAP_H

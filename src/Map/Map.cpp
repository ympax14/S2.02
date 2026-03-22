#include "Map.hpp"

#include "./City/City.hpp"
#include "./Country/Country.hpp"

Map::Map() {}

Map::~Map() {
    for (std::map<QString, Country*>::iterator it = this->countries.begin(); it != this->countries.end(); it++)
        delete it->second;

    if (this->currentRide != nullptr)
        delete this->currentRide;
};

void Map::registerCityId(int id, City* city) {
    this->citiesById[id] = city;
}

City* Map::getCityById(int id) const {
    auto it = citiesById.find(id);
    return it != citiesById.end() ? it->second : nullptr;
}

Country* Map::getCountry(const QString& countryName) const {
    std::map<QString, Country*>::const_iterator itCountry = this->countries.find(countryName);

    return itCountry == this->countries.end() ? nullptr : itCountry->second;
}

void Map::addCountry(Country* const country) {
    std::map<QString, Country*>::iterator itCountry = this->countries.find(country->getName());

    if (itCountry != this->countries.end()) return;

    this->countries[country->getName()] = country;
}

/**
 * Algorithme de Floyd-Warshall
 */

void Map::computeShortestPaths() {
    this->cityIndex.clear();
    this->cityToIndex.clear();

    // 1. Assigner un index numérique de 0 à n-1 pour chaque ville
    int index = 0;
    for (const auto& [countryName, countryPtr] : this->countries) {
        for (const auto& [adminName, adminPtr] : countryPtr->getAdministrations()) {
            for (const auto& [cityName, cityPtr] : adminPtr->getCities()) {
                this->cityIndex.push_back(cityPtr);
                this->cityToIndex[cityPtr] = index++;
            }
        }
    }

    int n = cityIndex.size();
    const int INF = 1e9; // Constante pour simuler l'infini sans "overflow"

    // Initialisation des matrices
    distMatrix.assign(n, std::vector<int>(n, INF));
    nextMatrix.assign(n, std::vector<const City*>(n, nullptr));

    // 2. Initialisation (W0) avec les voisins actuels
    for (int i = 0; i < n; ++i) {
        distMatrix[i][i] = 0;
        nextMatrix[i][i] = cityIndex[i];

        for (int j = 0; j < n; ++j) {
            if (i == j) continue;

            int weight = cityIndex[i]->getNeighborWeight(cityIndex[j]);
            if (weight != -1) { // -1 signifie qu'il n'y a pas d'arête directe
                distMatrix[i][j] = weight;
                nextMatrix[i][j] = cityIndex[j];
            }
        }
    }

    // 3. Boucles de Floyd-Warshall
    for (int k = 0; k < n; ++k) {
        for (int u = 0; u < n; ++u) {
            for (int v = 0; v < n; ++v) {
                // On évite de faire des calculs avec INF
                if (distMatrix[u][k] != INF && distMatrix[k][v] != INF) {

                    if (distMatrix[u][k] + distMatrix[k][v] < distMatrix[u][v]) {

                        // Mise à jour du poids minimum
                        distMatrix[u][v] = distMatrix[u][k] + distMatrix[k][v];

                        // Pour aller de u à v, le prochain sommet est le même que pour aller de u à k
                        nextMatrix[u][v] = nextMatrix[u][k];
                    }
                }
            }
        }
    }
}

// Permet de reconstruire la Route complète après avoir fait tourner l'algo
Route* Map::getShortestRoute(const City* from, const City* to) {
    Route* route = new Route;

    if (!cityToIndex.count(from) || !cityToIndex.count(to)) return route;

    int u = cityToIndex[from];
    int v = cityToIndex[to];

    // S'il n'y a aucun chemin possible entre "from" et "to"
    if (nextMatrix[u][v] == nullptr) return route;

    const City* curr = from;
    while (curr != to) {
        int currIdx = cityToIndex[curr];
        const City* nextCity = nextMatrix[currIdx][v];

        int weight = curr->getNeighborWeight(nextCity);
        route->addSegment(Path(curr, nextCity, weight)); // Création du segment

        curr = nextCity; // On avance
    }

    return route;
}
#include "Deserialize.hpp"

#include "../Map/City/City.hpp"
#include "../Map/Administration/Administration.hpp"
#include "../Map/Country/Country.hpp"

QList<QStringList> Deserialize::readCsv(const QString& filepath) {
    QList<QStringList> rows;
    QFile file(filepath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Impossible d'ouvrir le fichier :" << filepath;
        return rows;
    }

    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        if (line.isEmpty()) continue;

        // Séparé par virgule
        QStringList fields = line.split(',');
        rows.append(fields);
    }

    return rows;
}

void Deserialize::loadCitiesFromCsv(const QString& filepath, Map& map) {
    QList<QStringList> rows = readCsv(filepath);

    if (rows.isEmpty()) return;

    // Supprimer l'en-tête
    rows.removeFirst();

    int count = 0;
    for (const QStringList& fields : rows) {
        if (fields.size() < 19) continue; // On vérifie que la ligne est complète

        QString name = fields[0];
        int id = fields[1].toInt();
        double lat = fields[3].toDouble();
        double lng = fields[4].toDouble();
        QString countryName = fields[5];
        QString iso2 = fields[6];
        QString iso3 = fields[7];
        QString adminName = fields[8];
        QString adminAscii = fields[9];
        QString adminCode = fields[10];
        double density = fields[13].toDouble();
        int population = fields[14].toInt();
        int population_proper = fields[15].toInt();

        Country* country = map.getCountry(countryName);
        if (country == nullptr) {
            country = new Country(countryName, iso2, iso3);
            map.addCountry(country);
        }

        Administration* admin = country->getAdministration(adminName);
        if (admin == nullptr) {
            admin = new Administration(adminName, adminAscii, adminCode);
            country->addAdministration(admin);
        }

        City* city = new City(id, name, lat, lng, density, population, population_proper, country, admin);
        admin->addCity(city);
        map.registerCityId(id, city); // On indexe la ville par son ID
        count++;
    }

    qDebug() << "[SUCCES] Villes chargées ! Nombre de villes :" << count;
}

void Deserialize::loadConnectionsFromCsv(const QString& filepath, Map& map) {
    QList<QStringList> rows = readCsv(filepath);
    if (rows.isEmpty()) return;

    // Si tu as laissé une ligne d'en-tête (from_id,to_id,temps), on la saute
    bool isHeader = !rows[0][0].toInt();
    if (isHeader) rows.removeFirst();

    int count = 0;
    for (const QStringList& fields : rows) {
        if (fields.size() < 3) continue;

        int fromId = fields[0].toInt();
        int toId = fields[1].toInt();
        int temps = fields[2].toInt();

        City* fromCity = map.getCityById(fromId);
        City* toCity = map.getCityById(toId);

        // Si les deux villes existent bien dans la carte, on crée la route
        if (fromCity != nullptr && toCity != nullptr) {
            fromCity->addNeighbor(toCity, temps);
            toCity->addNeighbor(fromCity, temps); // On assume que le trajet se fait dans les deux sens
            count++;
        }
    }

    qDebug() << "[SUCCES] Connexions chargées ! Nombre de routes :" << count;
}

std::vector<std::vector<std::pair<double, double>>> Deserialize::loadGeoJsonBorders(const QString& filepath) {
    std::vector<std::vector<std::pair<double, double>>> polygons;
    QFile file(filepath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[ERREUR] Impossible d'ouvrir le fichier GeoJSON :" << filepath;
        return polygons;
    }

    QByteArray rawData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "[ERREUR] Fichier GeoJSON invalide ou mal formaté.";
        return polygons;
    }

    QJsonObject root = doc.object();
    QJsonArray features;

    // On gère les différents formats possibles du GeoJSON
    if (root.contains("features")) {
        // C'est une FeatureCollection standard
        features = root["features"].toArray();
    } else if (root.value("type").toString() == "Feature") {
        // C'est un Feature unique (arrive souvent)
        features.append(root);
    } else if (root.contains("coordinates")) {
        // C'est une Geometry directe (cas plus rare)
        QJsonObject dummyFeature;
        dummyFeature["geometry"] = root;
        features.append(dummyFeature);
    }

    if (features.isEmpty()) {
        qWarning() << "[ATTENTION] Aucune donnée géographique (feature) trouvée dans le fichier.";
        return polygons;
    }

    // Parsing
    for (const QJsonValue& featureVal : features) {
        QJsonObject feature = featureVal.toObject();
        QJsonObject geometry = feature["geometry"].toObject();
        QString type = geometry["type"].toString();
        QJsonArray coordinates = geometry["coordinates"].toArray();

        if (type == "Polygon") {
            QJsonArray ring = coordinates[0].toArray();
            std::vector<std::pair<double, double>> polyCoords;
            for (const QJsonValue& coordVal : ring) {
                QJsonArray pt = coordVal.toArray();
                polyCoords.push_back({pt[1].toDouble(), pt[0].toDouble()});
            }
            polygons.push_back(polyCoords);
        }
        else if (type == "MultiPolygon") {
            for (const QJsonValue& polyVal : coordinates) {
                QJsonArray ring = polyVal.toArray()[0].toArray();
                std::vector<std::pair<double, double>> polyCoords;
                for (const QJsonValue& coordVal : ring) {
                    QJsonArray pt = coordVal.toArray();
                    polyCoords.push_back({pt[1].toDouble(), pt[0].toDouble()});
                }
                polygons.push_back(polyCoords);
            }
        }
    }

    qDebug() << "[SUCCES] GeoJSON des frontières chargé ! Nombre de polygones lus :" << polygons.size();

    return polygons;
}

std::vector<std::pair<QString, std::vector<std::pair<double, double>>>> Deserialize::loadGeoJsonRegions(const QString& filepath) {
    std::vector<std::pair<QString, std::vector<std::pair<double, double>>>> polygons;
    QFile file(filepath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[ERREUR] Impossible d'ouvrir le GeoJSON des régions :" << filepath;
        return polygons;
    }

    QByteArray rawData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(rawData);
    if (doc.isNull() || !doc.isObject()) return polygons;

    QJsonArray features = doc.object()["features"].toArray();

    for (const QJsonValue& featureVal : features) {
        QJsonObject feature = featureVal.toObject();
        QJsonObject geometry = feature["geometry"].toObject();
        QString type = geometry["type"].toString();

        // On récupère le nom de la région
        QString regionName = feature["properties"].toObject()["nom"].toString();

        QJsonArray coordinates = geometry["coordinates"].toArray();

        if (type == "Polygon") {
            QJsonArray ring = coordinates[0].toArray();
            std::vector<std::pair<double, double>> polyCoords;
            for (const QJsonValue& coordVal : ring) {
                QJsonArray pt = coordVal.toArray();
                polyCoords.push_back({pt[1].toDouble(), pt[0].toDouble()});
            }
            polygons.push_back({regionName, polyCoords});
        }
        else if (type == "MultiPolygon") {
            for (const QJsonValue& polyVal : coordinates) {
                QJsonArray ring = polyVal.toArray()[0].toArray();
                std::vector<std::pair<double, double>> polyCoords;
                for (const QJsonValue& coordVal : ring) {
                    QJsonArray pt = coordVal.toArray();
                    polyCoords.push_back({pt[1].toDouble(), pt[0].toDouble()});
                }
                polygons.push_back({regionName, polyCoords});
            }
        }
    }

    qDebug() << "[SUCCES] GeoJSON des régions chargé ! Nombre de polygones lus :" << polygons.size();

    return polygons;
}
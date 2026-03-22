#ifndef DESERIALIZE_HPP
#define DESERIALIZE_HPP

#include "../Map/Map.hpp"

#include <QList>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Deserialize {
    QList<QStringList> readCsv(const QString& filepath);
    void loadCitiesFromCsv(const QString& filepath, Map& map);
    void loadConnectionsFromCsv(const QString& filepath, Map& map);

    std::vector<std::vector<std::pair<double, double>>> loadGeoJsonBorders(const QString& filepath);
    std::vector<std::pair<QString, std::vector<std::pair<double, double>>>> loadGeoJsonRegions(const QString& filepath);
};

#endif // DESERIALIZE_HPP

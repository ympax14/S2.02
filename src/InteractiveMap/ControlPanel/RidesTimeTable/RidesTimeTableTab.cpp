#include "RidesTimeTableTab.hpp"

#include "../../../NavyraWindow.hpp"
#include "../../../Map/Country/Country.hpp"
#include "../../../Map/Administration/Administration.hpp"
#include "../../../Map/City/City.hpp"

#include <QCompleter>
#include <QHeaderView>

RidesTimeTableTab::RidesTimeTableTab(QWidget* const parent) :
    QWidget{parent},
    layout(new QVBoxLayout(this)),

    selectionLayout(new QHBoxLayout),
    comboCity(new QComboBox),
    addCityBtn(new QPushButton("Ajouter")),

    cityListWidget(new QListWidget),
    removeCityBtn(new QPushButton("Retirer la sélection")),

    generateBtn(new QPushButton("Générer le tableau")),
    tableWidget(new QTableWidget)
{
    // Remplissage la ComboBox (barre de recherche)
    for (const auto& [countryName, countryPtr] : NavyraWindow::getMap().getCountries()) {
        for (const auto& [adminName, adminPtr] : countryPtr->getAdministrations()) {
            for (const auto& [cityName, cityPtr] : adminPtr->getCities()) {
                this->comboCity->addItem(cityPtr->getName(), cityPtr->getId());
            }
        }
    }
    this->comboCity->model()->sort(0);
    this->comboCity->setEditable(true);
    this->comboCity->setInsertPolicy(QComboBox::NoInsert);
    this->comboCity->completer()->setFilterMode(Qt::MatchContains);

    this->selectionLayout->addWidget(new QLabel("Ville :"));
    this->selectionLayout->addWidget(this->comboCity, 1); // "1" permet à la combobox de prendre la place disponible
    this->selectionLayout->addWidget(this->addCityBtn);

    // Permet aux colonnes et aux lignes de s'étirer pour remplir l'espace
    this->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Empêche l'utilisateur d'éditer le texte

    this->layout->addLayout(this->selectionLayout);
    this->layout->addWidget(this->cityListWidget);
    this->layout->addWidget(this->removeCityBtn);
    this->layout->addWidget(this->generateBtn);
    this->layout->addWidget(this->tableWidget);

    QObject::connect(this->addCityBtn, &QPushButton::clicked, this, &RidesTimeTableTab::addCity);
    QObject::connect(this->removeCityBtn, &QPushButton::clicked, this, &RidesTimeTableTab::removeCity);
    QObject::connect(this->generateBtn, &QPushButton::clicked, this, &RidesTimeTableTab::generateTable);
}

void RidesTimeTableTab::addCity() {
    int index = this->comboCity->findText(this->comboCity->currentText());
    if (index == -1) return; // Sécurité si le texte est invalide

    int cityId = this->comboCity->itemData(index).toInt();
    QString cityName = this->comboCity->itemText(index);

    // Vérifier que la ville n'est pas déjà dans la liste pour éviter les doublons
    for (int i = 0; i < this->cityListWidget->count(); ++i) {
        if (this->cityListWidget->item(i)->data(Qt::UserRole).toInt() == cityId) {
            return;
        }
    }

    // Ajouter la ville à la liste (On sauvegarde l'ID dans la "UserRole" data)
    QListWidgetItem* item = new QListWidgetItem(cityName);
    item->setData(Qt::UserRole, cityId);
    this->cityListWidget->addItem(item);
}

void RidesTimeTableTab::removeCity() {
    qDeleteAll(this->cityListWidget->selectedItems());
}

void RidesTimeTableTab::generateTable() {
    int count = this->cityListWidget->count();
    if (count < 2) return; // Il faut au moins 2 villes pour faire un tableau

    this->tableWidget->setRowCount(count);
    this->tableWidget->setColumnCount(count);

    QStringList headers;
    std::vector<const City*> selectedCities;

    // Récupérer toutes les villes sélectionnées et construire les en-têtes
    for (int i = 0; i < count; ++i) {
        QListWidgetItem* item = this->cityListWidget->item(i);
        headers << item->text();
        selectedCities.push_back(NavyraWindow::getMap().getCityById(item->data(Qt::UserRole).toInt()));
    }

    this->tableWidget->setHorizontalHeaderLabels(headers);
    this->tableWidget->setVerticalHeaderLabels(headers);

    // Remplir la matrice
    for (int row = 0; row < count; ++row) {
        for (int col = 0; col < count; ++col) {
            const City* cityA = selectedCities[row];
            const City* cityB = selectedCities[col];

            QTableWidgetItem* cell = new QTableWidgetItem();
            cell->setTextAlignment(Qt::AlignCenter);

            if (row == col) {
                // Trajet de la ville vers elle-même
                cell->setText("-");
                cell->setBackground(QBrush(QColor("#444444"))); // Gris
            } else {
                // Demander l'itinéraire à la Map (utilise les matrices de Floyd-Warshall)
                const Route* route = NavyraWindow::getMap().getShortestRoute(cityA, cityB);

                if (route->getSegments().empty()) {
                    cell->setText("N/A"); // Aucun chemin possible
                    cell->setForeground(QBrush(Qt::red));
                } else {
                    cell->setText(QString::number(route->getTotalWeight()));
                }

                // IMPORTANT : Map::getShortestRoute fait un `new Route`.
                // Il faut le supprimer après en avoir extrait le poids pour éviter une fuite de mémoire !
                delete route;
            }

            this->tableWidget->setItem(row, col, cell);
        }
    }
}
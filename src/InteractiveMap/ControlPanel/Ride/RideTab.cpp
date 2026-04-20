#include "RideTab.hpp"

#include "../../../NavyraWindow.hpp"
#include "../../../Map/Administration/Administration.hpp"
#include "../../../Map/Country/Country.hpp"
#include "../../MapScene.hpp"

#include <QCompleter>

const int RideTab::RIDE_PREVIEW_CITY_RADIUS = 50;
const int RideTab::RIDE_PREVIEW_CITIES_SPACING = 250;
const double RideTab::RIDE_PREVIEW_ARROW_SIZE = 25;

RideTab::RideTab(QWidget* const parent) :
    QWidget{parent},

    layout(new QVBoxLayout(this)),
    label(new QLabel("Sélectionnez une ville de départ et d'arrivée sur la carte.")),

    rideSelectionLayout(new QHBoxLayout),
    comboRideStart(new QComboBox),
    comboRideEnd(new QComboBox),

    rideBtnLayout(new QHBoxLayout),
    calculateRideBtn(new QPushButton("Calculer le Trajet")),
    clearRideBtn(new QPushButton("Retirer le Trajet")),

    ridePreviewLayout(new QVBoxLayout),
    ridePreviewScene(new QGraphicsScene(this)),
    ridePreviewView(new QGraphicsView(this->ridePreviewScene))
{
    this->layout->addWidget(this->label);

    for (const auto& [countryName, countryPtr] : NavyraWindow::getMap().getCountries()) {
        for (const auto& [adminName, adminPtr] : countryPtr->getAdministrations()) {
            for (const auto& [cityName, cityPtr] : adminPtr->getCities()) {
                this->comboRideStart->addItem(cityPtr->getName(), cityPtr->getId());
                this->comboRideEnd->addItem(cityPtr->getName(), cityPtr->getId());
            }
        }
    }

    // Trier les menus par ordre alphabétique
    this->comboRideStart->model()->sort(0);
    this->comboRideEnd->model()->sort(0);

    // Barre de Recherche dans la QComboBox
    this->comboRideStart->setEditable(true);
    this->comboRideEnd->setEditable(true);

    // On empêche l'ajout de nouvelles villes si l'utilisateur appuie sur Entrée
    this->comboRideStart->setInsertPolicy(QComboBox::NoInsert);
    this->comboRideEnd->setInsertPolicy(QComboBox::NoInsert);

    // On configure le filtre pour qu'il cherche partout dans le mot
    // (ex: taper "Paris" trouvera "Paris" mais aussi "Le Touquet-Paris-Plage")
    this->comboRideStart->completer()->setFilterMode(Qt::MatchContains);
    this->comboRideEnd->completer()->setFilterMode(Qt::MatchContains);

    this->rideSelectionLayout->addStretch();
    this->rideSelectionLayout->addWidget(this->comboRideStart, -1);
    this->rideSelectionLayout->addWidget(new QLabel(" 🡪 "));
    this->rideSelectionLayout->addWidget(this->comboRideEnd, -1);
    this->rideSelectionLayout->addStretch();
    this->layout->addLayout(this->rideSelectionLayout);

    this->rideBtnLayout->addWidget(this->calculateRideBtn);
    this->rideBtnLayout->addWidget(this->clearRideBtn);
    this->layout->addLayout(this->rideBtnLayout);

    this->layout->addStretch(); // Pousse le contenu vers le haut

    // Preview du trajet
    this->ridePreviewView->setRenderHint(QPainter::Antialiasing); // Pour lisser les traits
    this->ridePreviewView->setDragMode(QGraphicsView::ScrollHandDrag); // Permet de naviguer
    this->ridePreviewLayout->addWidget(this->ridePreviewView);

    this->layout->addLayout(this->ridePreviewLayout);

    QObject::connect(this->calculateRideBtn, &QPushButton::clicked, this, &RideTab::calculateRide);
    QObject::connect(this->clearRideBtn, &QPushButton::clicked, this, &RideTab::clearRide);
}

void RideTab::calculateRide() {
    // Sécurité. Comme le champ est éditable, l'utilisateur a pu taper n'importe quoi.
    // On cherche l'index réel qui correspond au texte actuellement tapé.
    int startIndex = this->comboRideStart->findText(this->comboRideStart->currentText());
    int endIndex = this->comboRideEnd->findText(this->comboRideEnd->currentText());

    // Si une des deux villes n'existe pas dans la liste, ou si ce sont les mêmes, on annule
    if (startIndex == -1 || endIndex == -1 || startIndex == endIndex) return;

    int startId = this->comboRideStart->itemData(startIndex).toInt();
    int endId = this->comboRideEnd->itemData(endIndex).toInt();

    const City* from = NavyraWindow::getMap().getCityById(startId);
    const City* to = NavyraWindow::getMap().getCityById(endId);

    const Route* ride = NavyraWindow::getMap().getShortestRoute(from, to);
    NavyraWindow::getMap().setCurrentRide(ride);
    NavyraWindow::getInstance()->getMapScene()->buildRide(true);
    this->drawRide(ride);
}

void RideTab::clearRide() {
    NavyraWindow::getMap().clearCurrentRide();
    NavyraWindow::getInstance()->getMapScene()->buildRide(true);
    this->drawRide(nullptr);
}

void RideTab::drawRide(const Route* const ride) {
    this->ridePreviewScene->clear();

    if (ride == nullptr) return;

    if (ride->getSegments().empty()) return;

    this->drawNode(0, 0, ride->getSegments()[0].getFrom()->getName());

    for (size_t i = 0; i < ride->getSegments().size(); i++) {
        const Path& path = ride->getSegments()[i];
        this->drawNode((i+1)*RideTab::RIDE_PREVIEW_CITIES_SPACING, 0, path.getTo()->getName());
        this->drawDirectedEdge(QPointF(i*RideTab::RIDE_PREVIEW_CITIES_SPACING, 0), QPointF((i+1)*RideTab::RIDE_PREVIEW_CITIES_SPACING, 0), path.getWeight());
    }
}

void RideTab::drawNode(qreal x, qreal y, const QString& text) {
    // Le cercle (centré sur x, y)
    QGraphicsEllipseItem* node = new QGraphicsEllipseItem(x - RideTab::RIDE_PREVIEW_CITY_RADIUS, y - RideTab::RIDE_PREVIEW_CITY_RADIUS, RideTab::RIDE_PREVIEW_CITY_RADIUS * 2, RideTab::RIDE_PREVIEW_CITY_RADIUS * 2);
    node->setBrush(QBrush(QColor("#3498DB")));
    node->setPen(QPen(Qt::black, 2));
    node->setZValue(2); // Au-dessus des lignes
    this->ridePreviewScene->addItem(node);

    // Le texte
    QGraphicsSimpleTextItem* label = new QGraphicsSimpleTextItem(text, node);
    QFont font = label->font();
    font.setBold(true);
    label->setFont(font);
    label->setBrush(Qt::white);

    // Centrer le texte dans le cercle
    QRectF textRect = label->boundingRect();
    label->setPos(x - textRect.width() / 2.0, y - textRect.height() / 2.0);
}

#include <cmath>

void RideTab::drawDirectedEdge(QPointF start, QPointF end, int weight) {
    // ligne principale
    QGraphicsLineItem* line = new QGraphicsLineItem(start.x(), start.y(), end.x(), end.y());
    QPen pen(Qt::black, 2);
    line->setPen(pen);
    line->setZValue(1); // Sous les nœuds
    this->ridePreviewScene->addItem(line);

    // La flèche
    double angle = std::atan2(start.y() - end.y(), start.x() - end.x());

    // On s'arrête un peu avant le centre du nœud d'arrivée (selon le rayon du nœud, ex: 20)
    QPointF edgeEnd = end + QPointF(std::cos(angle) * RideTab::RIDE_PREVIEW_CITY_RADIUS, std::sin(angle) * RideTab::RIDE_PREVIEW_CITY_RADIUS);

    QPointF arrowP1 = edgeEnd + QPointF(std::sin(angle + M_PI / 3) * RideTab::RIDE_PREVIEW_ARROW_SIZE, std::cos(angle + M_PI / 3) * RideTab::RIDE_PREVIEW_ARROW_SIZE);
    QPointF arrowP2 = edgeEnd + QPointF(std::sin(angle + M_PI - M_PI / 3) * RideTab::RIDE_PREVIEW_ARROW_SIZE, std::cos(angle + M_PI - M_PI / 3) * RideTab::RIDE_PREVIEW_ARROW_SIZE);

    QPolygonF arrowHead;
    arrowHead << edgeEnd << arrowP1 << arrowP2;
    QGraphicsPolygonItem* arrowItem = new QGraphicsPolygonItem(arrowHead);
    arrowItem->setBrush(Qt::black);
    arrowItem->setZValue(1);
    this->ridePreviewScene->addItem(arrowItem);

    // Le Poids (avec ancre)
    QPointF midPoint((start.x() + end.x()) / 2.0, (start.y() + end.y()) / 2.0);

    QGraphicsRectItem* anchor = new QGraphicsRectItem(0, 0, 0, 0);
    anchor->setPos(midPoint);
    anchor->setZValue(3);
    this->ridePreviewScene->addItem(anchor);

    QGraphicsSimpleTextItem* weightText = new QGraphicsSimpleTextItem(QString::number(weight) + "min", anchor);
    QFont font = weightText->font();
    font.setBold(true);
    font.setPointSize(10);
    weightText->setFont(font);
    weightText->setBrush(QColor("#E74C3C")); // Poids en rouge pour la lisibilité

    QRectF textRect = weightText->boundingRect();
    // Décalage pour ne pas être superposé à la ligne
    weightText->setPos(-textRect.width() / 2.0, -textRect.height() - 5);
}
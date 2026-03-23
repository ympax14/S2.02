#include "NavyraWindow.hpp"

#include "utils/Deserialize.hpp"
#include "Options/OptionsWindow.hpp"

#include <QStatusBar>
#include <QMenuBar>

NavyraWindow* NavyraWindow::INSTANCE = nullptr;
const QString NavyraWindow::WINDOW_TITLE = "Navyra | Carte Interactive";
const int NavyraWindow::WINDOW_WIDTH = 1280, NavyraWindow::WINDOW_HEIGHT = 720;
Map NavyraWindow::map;

NavyraWindow::NavyraWindow(QWidget * const parent) :
    QMainWindow(parent),

    optionsWindow(nullptr),

    splitter(new QSplitter(Qt::Horizontal, this)),

    mapScene(new MapScene(&NavyraWindow::map, this)),
    mapView(new InteractiveView(this->mapScene)),

    optionsAction(new QAction("&Options", this->menuBar())),

    zoomStatus(new QLabel(this))

{
    NavyraWindow::INSTANCE = this;

    this->setupWindow();
    this->setupInteractiveMap();

    // Côté droit : Le panneau de controle. A charger après avoir setup la carte intéractive sinon données de villes nulles
    this->setupControlPanel();
    this->setupMenuBar();
    this->setupStatusBar();
    this->setupZoom();
}

void NavyraWindow::setupWindow() {
    this->setWindowTitle(NavyraWindow::WINDOW_TITLE);
    this->setMinimumSize(NavyraWindow::WINDOW_WIDTH, NavyraWindow::WINDOW_HEIGHT);

    // Un bleu clair naturel pour l'océan
    this->mapScene->setBackgroundBrush(QBrush(QColor(0x86CADC)));

    // Côté gauche : La carte
    this->splitter->addWidget(this->mapView);

    this->setCentralWidget(this->splitter);
}

void NavyraWindow::setupInteractiveMap() {
    Deserialize::loadCitiesFromCsv(":/assets/datas/villes.csv", this->map);
    Deserialize::loadConnectionsFromCsv(":/assets/datas/temps.csv", this->map);

    // On calcule les plus courts chemins au démarrage
    this->map.computeShortestPaths();

    this->mapScene->build();
}

void NavyraWindow::setupControlPanel() {
    this->controlPanel = new ControlPanelTabWidget(this->splitter);
    this->splitter->addWidget(controlPanel);
}

void NavyraWindow::openOptions() {
    if (this->optionsWindow != nullptr) return;

    qDebug() << "[Options] Ouverture de la fenêtre des options";

    this->optionsWindow = new OptionsWindow;

    this->optionsWindow->show();

    QObject::connect(this->optionsWindow, &OptionsWindow::onClose, this, [this]() {
        qDebug() << "[Options] Fermeture de la fenêtre des options";

        delete this->optionsWindow;
        this->optionsWindow = nullptr;
    });
}

void NavyraWindow::setupMenuBar() {
    this->menuBar()->addAction(this->optionsAction);
    QObject::connect(this->optionsAction, &QAction::triggered, this, &NavyraWindow::openOptions);
}

void NavyraWindow::setupStatusBar() {
    this->zoomStatus->setText(QString("Zoom: %1x").arg(InteractiveView::defaultScale,  0, 'f', 2));
    this->statusBar()->addPermanentWidget(zoomStatus);

    QObject::connect(this->mapView, &InteractiveView::zoomChanged, this, [this](qreal scale) {
        this->zoomStatus->setText(QString("Zoom: %1x").arg(scale, 0, 'f', 2));
    });
}

void NavyraWindow::setupZoom() {
    QObject::connect(this->mapView, &InteractiveView::zoomChanged, mapScene, &MapScene::onZoomChanged);
}

NavyraWindow::~NavyraWindow() = default;

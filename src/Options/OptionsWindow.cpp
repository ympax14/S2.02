#include "OptionsWindow.hpp"

#include "../NavyraWindow.hpp"

OptionsWindow::OptionsWindow() :
    QWidget{nullptr},

    mainLayout(new QVBoxLayout(this)),

    pathsBox(new QGroupBox("Routes", this)),
    pathsLayout(new QVBoxLayout(this->pathsBox)),

    showAllPathsLayout(new QHBoxLayout),
    labelShowAllPaths(new QLabel("Afficher toutes les routes", this)),
    checkShowAllPaths(new QCheckBox(this)),

    pathsSizeLayout(new QHBoxLayout),
    labelPathsSize(new QLabel("Taille des Routes", this)),
    spinPathsSize(new QDoubleSpinBox(this)),

    showAllCitiesBox(new QGroupBox("Villes", this)),
    showAllCitiesLayout(new QHBoxLayout),
    labelShowAllCities(new QLabel("Afficher toutes les villes", this)),
    checkShowAllCities(new QCheckBox(this)),

    footerLayout(new QHBoxLayout),
    closeBtn(new QPushButton("&Fermer", this))
{
    this->setWindowTitle("Navyra | Options");
    this->setFixedSize(400, 600);

    this->showAllPathsLayout->addWidget(this->labelShowAllPaths);
    this->checkShowAllPaths->setChecked(!NavyraWindow::getInstance()->getMapScene()->HIDE_PATHS);
    this->showAllPathsLayout->addWidget(this->checkShowAllPaths);
    this->showAllPathsLayout->addStretch();

    this->pathsLayout->addLayout(this->showAllPathsLayout);

    this->pathsSizeLayout->addWidget(this->labelPathsSize);

    this->spinPathsSize->setValue(MapScene::PATH_SIZE);
    this->spinPathsSize->setMinimum(0.3);
    this->spinPathsSize->setMaximum(5.0);
    this->pathsSizeLayout->addWidget(this->spinPathsSize);

    this->pathsSizeLayout->addStretch();

    this->pathsLayout->addLayout(this->pathsSizeLayout);

    this->mainLayout->addWidget(this->pathsBox);

    this->showAllCitiesLayout->addWidget(this->labelShowAllCities);
    this->checkShowAllCities->setChecked(!NavyraWindow::getInstance()->getMapScene()->HIDE_CITIES);
    this->showAllCitiesLayout->addWidget(this->checkShowAllCities);
    this->showAllCitiesLayout->addStretch();

    this->showAllCitiesBox->setLayout(this->showAllCitiesLayout);

    this->mainLayout->addWidget(this->showAllCitiesBox);

    this->mainLayout->addStretch();

    this->footerLayout->addStretch();
    this->footerLayout->addWidget(this->closeBtn);
    this->mainLayout->addLayout(this->footerLayout);

    QObject::connect(this->closeBtn, &QPushButton::clicked, this, &QWidget::close);

    QObject::connect(this->checkShowAllPaths, &QCheckBox::clicked, this, [this]() {
        MapScene::HIDE_PATHS = !this->checkShowAllPaths->isChecked();
        NavyraWindow::getInstance()->getMapScene()->buildRoutes(true);
    });

    QObject::connect(this->spinPathsSize, &QDoubleSpinBox::valueChanged, this, [this]() {
        MapScene::PATH_SIZE = this->spinPathsSize->value();
        NavyraWindow::getInstance()->getMapScene()->buildRoutes(true);
    });

    QObject::connect(this->checkShowAllCities, &QCheckBox::clicked, this, [this]() {
        MapScene::HIDE_CITIES = !this->checkShowAllCities->isChecked();
        NavyraWindow::getInstance()->getMapScene()->buildCities(true);
        NavyraWindow::getInstance()->getMapScene()->onZoomChanged(InteractiveView::scaleFactor);
    });
}

void OptionsWindow::closeEvent(QCloseEvent *event) {
    QWidget::closeEvent(event);

    emit onClose();
}
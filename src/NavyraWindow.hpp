#ifndef NAVYRAWINDOW_H
#define NAVYRAWINDOW_H

#include "Map/Map.hpp"
#include "InteractiveMap/MapScene.hpp"
#include "src/InteractiveMap/InteractiveView.hpp"
#include "InteractiveMap/ControlPanel/ControlPanelTabWidget.hpp"

#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QMainWindow>
#include <QString>
#include <QLabel>

class NavyraWindow : public QMainWindow
{
    Q_OBJECT

private:
    static NavyraWindow* INSTANCE;
    static const QString WINDOW_TITLE;
    static const int WINDOW_WIDTH, WINDOW_HEIGHT;
    static Map map;

    QSplitter *splitter;

    MapScene * const mapScene;
    InteractiveView * const mapView;

    ControlPanelTabWidget *controlPanel;

    QAction* const optionsAction;

    QLabel * const zoomStatus;

    void setupWindow();
    void setupInteractiveMap();
    void setupControlPanel();
    void setupMenuBar();
    void setupStatusBar();
    void setupZoom();
public:
    explicit NavyraWindow(QWidget *parent = nullptr);
    ~NavyraWindow() override;

    inline static NavyraWindow* getInstance() { return NavyraWindow::INSTANCE; };
    inline static Map& getMap() { return NavyraWindow::map; };

    inline MapScene* getMapScene() const { return this->mapScene; };
    inline InteractiveView* getMapView() const { return this->mapView; };
    inline ControlPanelTabWidget* getControlPanel() const { return this->controlPanel; };
};
#endif // NAVYRAWINDOW_H

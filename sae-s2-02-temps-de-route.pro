QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/InteractiveMap/CityItem.cpp \
    src/InteractiveMap/ControlPanel/CityInformations/CityInformationsTab.cpp \
    src/InteractiveMap/ControlPanel/ControlPanelTabWidget.cpp \
    src/InteractiveMap/ControlPanel/Ride/RideTab.cpp \
    src/InteractiveMap/ControlPanel/RidesTimeTable/RidesTimeTableTab.cpp \
    src/InteractiveMap/InteractiveView.cpp \
    src/InteractiveMap/MapScene.cpp \
    src/Map/Administration/Administration.cpp \
    src/Map/City/City.cpp \
    src/Map/Country/Country.cpp \
    src/Map/Map.cpp \
    src/Map/Path/Path.cpp \
    src/Map/Route/Route.cpp \
    src/Options/OptionsWindow.cpp \
    src/main.cpp \
    src/NavyraWindow.cpp \
    src/utils/Deserialize.cpp

HEADERS += \
    src/InteractiveMap/CityItem.hpp \
    src/InteractiveMap/ControlPanel/CityInformations/CityInformationsTab.hpp \
    src/InteractiveMap/ControlPanel/ControlPanelTabWidget.hpp \
    src/InteractiveMap/ControlPanel/Ride/RideTab.hpp \
    src/InteractiveMap/ControlPanel/RidesTimeTable/RidesTimeTableTab.hpp \
    src/InteractiveMap/InteractiveView.hpp \
    src/InteractiveMap/MapScene.hpp \
    src/Map/Administration/Administration.hpp \
    src/Map/City/City.hpp \
    src/Map/Country/Country.hpp \
    src/Map/Map.hpp \
    src/Map/Path/Path.hpp \
    src/Map/Route/Route.hpp \
    src/NavyraWindow.hpp \
    src/Options/OptionsWindow.hpp \
    src/utils/Deserialize.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ressources.qrc

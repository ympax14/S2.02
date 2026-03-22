#ifndef RIDESTIMETABLETAB_HPP
#define RIDESTIMETABLETAB_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QTableWidget>

class RidesTimeTableTab : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout * const layout;

    QHBoxLayout * const selectionLayout;
    QComboBox * const comboCity;
    QPushButton * const addCityBtn;

    QListWidget * const cityListWidget;
    QPushButton * const removeCityBtn;

    QPushButton * const generateBtn;
    QTableWidget * const tableWidget;

public:
    RidesTimeTableTab(QWidget* const parent = nullptr);

public slots:
    void addCity();
    void removeCity();
    void generateTable();
};

#endif // RIDESTIMETABLETAB_HPP
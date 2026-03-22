#ifndef CITYINFORMATIONSTAB_HPP
#define CITYINFORMATIONSTAB_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class CityInformationsTab : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout* const layout;
    QLabel* const label;

public:
    explicit CityInformationsTab(QWidget *parent = nullptr);

    inline QLabel* getLabel() const { return this->label; };
signals:
};

#endif // CITYINFORMATIONSTAB_HPP

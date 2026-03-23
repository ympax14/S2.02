#ifndef OPTIONSWINDOW_HPP
#define OPTIONSWINDOW_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QGroupBox>
#include <QDoubleSpinBox>

class OptionsWindow : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout * const mainLayout;

    QGroupBox * const pathsBox;
    QVBoxLayout * const pathsLayout;

    QHBoxLayout * const showAllPathsLayout;
    QLabel * const labelShowAllPaths;
    QCheckBox * const checkShowAllPaths;

    QHBoxLayout * const pathsSizeLayout;
    QLabel * const labelPathsSize;
    QDoubleSpinBox * const spinPathsSize;

    QGroupBox * const showAllCitiesBox;
    QHBoxLayout * const showAllCitiesLayout;
    QLabel * const labelShowAllCities;
    QCheckBox * const checkShowAllCities;

    QHBoxLayout * const footerLayout;
    QPushButton * const closeBtn;
public:
    explicit OptionsWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void onClose();
};

#endif // OPTIONSWINDOW_HPP

#include "NavyraWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NavyraWindow w;
    w.show();
    return QCoreApplication::exec();
}

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setOrganizationName("L");
    QApplication::setApplicationName("Fitness");

    MainWindow w;
    w.show();
    return a.exec();
}

#include "MainWindow.h"
#include <QApplication>
#include <QGuiApplication>
#include <QString>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    MainWindow w;
    if(a.arguments().size()==3)
    {
        w.setNodCount(a.arguments().at(1).toInt());
        w.setMaxSubNodCount(a.arguments().at(2).toInt());
    }
    w.addSpheres();
    w.show();

    return a.exec();
}

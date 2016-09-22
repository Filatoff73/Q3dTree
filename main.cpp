#include "MainWindow.h"
#include <QApplication>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

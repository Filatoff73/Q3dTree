#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qt3DRenderer/QWindow>

class MainWindow : public Qt3D::QWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *screen = 0);
    ~MainWindow();

};

#endif // MAINWINDOW_H

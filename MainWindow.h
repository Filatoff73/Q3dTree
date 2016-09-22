#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGuiApplication>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>


#include <Qt3DInput/QInputAspect>
#include "qt3dwindow.h"
#include <Qt3DRender/QRenderAspect>
#include <Qt3DExtras//QForwardRenderer>
#include <Qt3DExtras//QPhongMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <QWindow>
#include <QMap>

#include <QGuiApplication>

class MainWindow : public Qt3DExtras::Qt3DWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QScreen *screen = 0);
    ~MainWindow();

    //!Добавление всех сфер
    void addSpheres();

    //! Получение рандомного нецелого числа (от -1 до 1) с accuracy точностью полсе запятой
    double randValue(int accuracy);

    //! Получение координаты Z так, чтобы она лежала в одной плоскости с остальными.
    //! XYZn - нормаль, XYZp - начальная точка, x,y - случайные координаты новой точки
    double findZforXY(const QVector3D& norm, const QVector3D& firstPoint, double x,double y);

    //! Проверить если сфера пересекается с кем то уменьшить ее радиус, или подвинуть  ее
    void checkAndChangeSpherePosition(const QVector3D& norm, const QVector3D& firstPoint, QVector3D& point, double &rad);

    void setNodCount(int value);

    void setMaxSubNodCount(int value);

private:
    Qt3DCore::QEntity *rootEntity;
    int nodCount;
    int maxSubNodCount;
    double radSphere;
    //!максимальные значения генерируемых координат
    int maxX, maxY,maxZ;

    //! Шаг передвижения сферы
    double step;
    //! макисмальное количество итераций для движения сферы
    int maxCountStep;

    //! Cписки позиций чтобы двигать сферы и менять их радиусы
    QList<QVector3D> listPosition;
    QList<double> listRad;

};

#endif // MAINWINDOW_H

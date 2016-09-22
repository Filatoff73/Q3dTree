#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Qt3DRenderer/QWindow>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QCamera>
#include <Qt3DCore/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QLookAtTransform>
#include <Qt3DCore/QScaleTransform>
#include <Qt3DCore/QRotateTransform>
#include <Qt3DCore/QTranslateTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRenderer/QRenderAspect>
#include <Qt3DRenderer/QFrameGraph>
#include <Qt3DRenderer/QForwardRenderer>
#include <Qt3DRenderer/QPhongMaterial>

#include <Qt3DRenderer/QCylinderMesh>
#include <Qt3DRenderer/QSphereMesh>
#include <Qt3DRenderer/QTorusMesh>
#include <Qt3DRenderer/QWindow>
#include <QMap>

class MainWindow : public Qt3D::QWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWindow *screen = 0);
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
    Qt3D::QEntity *rootEntity;
    int nodCount;
    int maxSubNodCount;
    double radSphere;
    //!максимальные значения генерируемых координат
    int maxX, maxY,maxZ;

    //! Шаг передвижения сферы
    double step;
    //! макисмальное количество итераций для движения сферы
    int maxCountStep;

    QList<QVector3D> listPosition;
    QList<double> listRad;

};

#endif // MAINWINDOW_H

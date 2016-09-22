#include "MainWindow.h"

#include <QGuiApplication>
#include "QTime"
#include "qmath.h"


MainWindow::MainWindow(QWindow *screen) :
    Qt3D::QWindow(screen)
{


    nodCount = 3;
    maxSubNodCount = 30;

    radSphere=2.0;
    step = 0.1;
    maxCountStep = 100;

    maxX = 20;
    maxY = 20;
    maxZ = 20;

    Qt3D::QInputAspect *input = new Qt3D::QInputAspect;
    registerAspect(input);

    // Root entity
    rootEntity = new Qt3D::QEntity();

    // Camera
    Qt3D::QCamera *cameraEntity = defaultCamera();

    cameraEntity->lens()->setPerspectiveProjection(80.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, -40.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    input->setCamera(cameraEntity);

    setRootEntity(rootEntity);


}

MainWindow::~MainWindow()
{
    delete rootEntity;
}

void MainWindow::addSpheres()
{
    qDebug()<<"start";
    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));


    for(int i=0;i<nodCount;i++)
    {
        // Material
        Qt3D::QPhongMaterial *material = new Qt3D::QPhongMaterial(rootEntity);
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        material->setAmbient(QColor(r,g,b));

        //случайное количество подузлов
        int maxSubNodCountReal = rand() % (maxSubNodCount+1);
        if(maxSubNodCountReal==0)
            maxSubNodCountReal=1;

        //нормаль
        QVector3D norm;
        //первая точка
        QVector3D firstPoint;

        for(int j=0;j<maxSubNodCountReal;j++)
        {
            // Sphere
            Qt3D::QEntity *sphereEntity = new Qt3D::QEntity(rootEntity);

            //координаты для добавления сферы
            QVector3D point;

            //радиус
            double rad = radSphere;

            //!Если сфера первая - задаем ее координаты и координаты нормали случайным образом
            if(j==0)
            {
                norm.setX(randValue(2)*maxX);
                norm.setY(randValue(2)*maxY);
                norm.setZ(randValue(2)*maxZ);

                firstPoint.setX(randValue(2)*maxX);
                firstPoint.setY(randValue(2)*maxY);
                firstPoint.setZ(randValue(2)*maxZ);

                point = firstPoint;

            }
            else
            {

                point.setX(randValue(2)*maxX);
                point.setY(randValue(2)*maxY);
                point.setZ(findZforXY(norm,firstPoint,point.x(),point.y()));
                while(fabs(point.z())>maxZ)
                {
                    point.setX(randValue(2)*maxX);
                    point.setY(randValue(2)*maxY);
                    point.setZ(findZforXY(norm,firstPoint,point.x(),point.y()));
                }

            }

            qDebug()<<"___";
            qDebug()<<point.x();
            qDebug()<<point.y();
            qDebug()<<point.z();
            qDebug()<<"___";
            checkAndChangeSpherePosition(norm,firstPoint,point,rad);

            // Transform
            Qt3D::QTransform *sphereTransform = new Qt3D::QTransform;
            Qt3D::QTranslateTransform *sphereTranslateTransform = new Qt3D::QTranslateTransform;
            sphereTranslateTransform->setTranslation(point);
            sphereTransform->addTransform(sphereTranslateTransform);

            Qt3D::QSphereMesh *sphereMesh = new Qt3D::QSphereMesh;
            sphereMesh->setRadius(rad);


            sphereEntity->addComponent(sphereMesh);
            sphereEntity->addComponent(material);
            sphereEntity->addComponent(sphereTransform);
            listPosition.append(point);
            listRad.append(rad);


        }



    }
}

double MainWindow::randValue(int accuracy)
{
    double a = 0;
    a = (qrand() % int (2.0*qPow(10, accuracy) + 1) - int (qPow(10, accuracy)))/qPow(10, accuracy);
    return a;
}

double MainWindow::findZforXY(const QVector3D &norm, const QVector3D &firstPoint, double x, double y)
{
    return -(norm.x()*x - norm.x()*firstPoint.x() + norm.y()*y - norm.y()*firstPoint.y() - norm.z()*firstPoint.z()) / norm.z();
}

void MainWindow::checkAndChangeSpherePosition(const QVector3D &norm, const QVector3D &firstPoint, QVector3D &point, double &rad)
{
    int k=0;
    bool mustMove=true;
    //сначала пытаемся уменьшить сферу
    while(rad>step && mustMove && rad>0.5)
    {
        mustMove=false;
        for(int i=0;i<listPosition.size();i++)
        {
            double sphereRad = radSphere;
            if(listRad.size()>i)
                sphereRad = listRad.at(i);

            QVector3D spherePoint = listPosition.at(i);

            if(fabs(point.x()-spherePoint.x())<=(double)(sphereRad+rad)
               && fabs(point.y()-spherePoint.y())<=(double)(sphereRad+rad)
               && fabs(point.z()-spherePoint.z())<=(double)(sphereRad+rad))
            {
                rad-=step;
                mustMove=true;
                qDebug()<<"rad reduced"<<rad;
            }
        }
    }

    //если не получилось - двигаем ее
    while(k<maxCountStep && mustMove)
    {
        mustMove=false;
        for(int i=0;i<listPosition.size();i++)
        {
            double sphereRad = radSphere;
            if(listRad.size()>i)
                sphereRad = listRad.at(i);

            QVector3D spherePoint = listPosition.at(i);

            if(fabs(point.x()-spherePoint.x())<=(double)(sphereRad+rad)
               && fabs(point.y()-spherePoint.y())<=(double)(sphereRad+rad)
               && fabs(point.z()-spherePoint.z())<=(double)(sphereRad+rad))
            {
                point.setX(point.x()+step);
                point.setY(point.y()+step);
                point.setZ(findZforXY(norm,firstPoint,point.x(),point.y()));
                mustMove=true;
                qDebug()<<"moved"<<point;
            }
        }
        k++;
    }
}

void MainWindow::setNodCount(int value)
{
    nodCount = value;
}

void MainWindow::setMaxSubNodCount(int value)
{
    maxSubNodCount = value;
}

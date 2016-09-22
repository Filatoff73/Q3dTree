#include "MainWindow.h"

#include "QTime"
#include "qmath.h"

#include <QOrbitCameraController>
#include <QComponent>
#include <QMesh>
#include <QAttribute>
#include <QBuffer>

MainWindow::MainWindow(QScreen *screen) :
    Qt3DExtras::Qt3DWindow(screen)
{
    nodCount = 3;
    maxSubNodCount = 15;

    radSphere=2.0;
    step = 0.1;
    maxCountStep = 100;

    maxX = 20;
    maxY = 20;
    maxZ = 20;

    // Root entity
    rootEntity = new Qt3DCore::QEntity();

    // Camera
    Qt3DRender::QCamera *camera = this->camera();
    camera->lens()->setPerspectiveProjection(100.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 40.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // For camera controls
    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setLinearSpeed( 50.0f );
    camController->setLookSpeed( 180.0f );
    camController->setCamera(camera);

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

    // Генерируем первый узел дерева
    // Material
    Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);
    material->setAmbient(QColor(Qt::black));
    // Sphere
    Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(rootEntity);

    // Transform
    Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform;
    sphereTransform->setTranslation(QVector3D(0,0,0));
    Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh;
    sphereMesh->setRadius(radSphere);
    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(material);
    sphereEntity->addComponent(sphereTransform);
    listPosition.append(QVector3D(0,0,0));
    listRad.append(radSphere);

    QVector<int> amountParentNodes;
    for(int i=0;i<nodCount+1;i++)
        amountParentNodes.append(0);
    //Изначальное количество родлительских узлов - 1, черный
    amountParentNodes[0]=1;


    for(int i=0;i<nodCount;i++)
    {
        // Material
        Qt3DExtras::QPhongMaterial *material = new Qt3DExtras::QPhongMaterial(rootEntity);
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        material->setAmbient(QColor(r,g,b));

        int amountParenNodesCurrent = amountParentNodes.at(i);

        //нормаль
        QVector3D norm;
        //первая точка
        QVector3D firstPoint;

        for(int parentNodes = 0;parentNodes<amountParenNodesCurrent;parentNodes++)
        {

         //случайное количество подузлов
            int maxSubNodCountReal = rand() % (maxSubNodCount+1);
            if(maxSubNodCountReal==0)
                maxSubNodCountReal=1;

            //!Если сфера первая - задаем ее координаты и координаты нормали случайным образом
            //! для получения уравнения плоскости
            if(parentNodes==0)
            {
                norm.setX(randValue(2)*maxX);
                norm.setY(randValue(2)*maxY);
                norm.setZ(randValue(2)*maxZ);

                firstPoint.setX(randValue(2)*maxX);
                firstPoint.setY(randValue(2)*maxY);
                firstPoint.setZ(randValue(2)*maxZ);
            }

            for(int j=0;j<maxSubNodCountReal;j++)
            {
                //Считаем сколько будет узлов на следующей итерации
                amountParentNodes[i+1]++;
                // Sphere
                Qt3DCore::QEntity *sphereEntity = new Qt3DCore::QEntity(rootEntity);

                //координаты для добавления сферы
                QVector3D point;

                //радиус
                double rad = radSphere;

                point.setX(randValue(2)*maxX);
                point.setY(randValue(2)*maxY);
                point.setZ(findZforXY(norm,firstPoint,point.x(),point.y()));
                int k=0;
                while(fabs(point.z())>maxZ && k<100)
                {
                    point.setX(randValue(2)*maxX);
                    point.setY(randValue(2)*maxY);
                    point.setZ(findZforXY(norm,firstPoint,point.x(),point.y()));
                    k++;
                }

//                qDebug()<<"___";
//                qDebug()<<point.x();
//                qDebug()<<point.y();
//                qDebug()<<point.z();
//                qDebug()<<"___";
                checkAndChangeSpherePosition(norm,firstPoint,point,rad);

                // Transform
                Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform;
                sphereTransform->setTranslation(point);

                Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh;
                sphereMesh->setRadius(rad);

                sphereEntity->addComponent(sphereMesh);
                sphereEntity->addComponent(material);
                sphereEntity->addComponent(sphereTransform);
                listPosition.append(point);
                listRad.append(rad);

            }
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
                //qDebug()<<"rad reduced"<<rad;
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
                //qDebug()<<"moved"<<point;
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

#include "MainWindow.h"

#include <QGuiApplication>

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



MainWindow::MainWindow(QWindow *screen) :
    Qt3D::QWindow(screen)
{

    Qt3D::QInputAspect *input = new Qt3D::QInputAspect;
    registerAspect(input);

    // Root entity
    Qt3D::QEntity *rootEntity = new Qt3D::QEntity();

    // Camera
    Qt3D::QCamera *cameraEntity = defaultCamera();

    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, -40.0f));
    cameraEntity->setUpVector(QVector3D(0, 1, 0));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));
    input->setCamera(cameraEntity);

    // Material
    Qt3D::QPhongMaterial *material = new Qt3D::QPhongMaterial(rootEntity);
    material->setAmbient(QColor(QRgb(0x35423)));
    // Sphere
    Qt3D::QEntity *sphereEntity = new Qt3D::QEntity(rootEntity);
    Qt3D::QSphereMesh *sphereMesh = new Qt3D::QSphereMesh;
    sphereMesh->setRadius(1);


    Qt3D::QTransform *sphereTransform = new Qt3D::QTransform;
    Qt3D::QTranslateTransform *sphereTranslateTransform = new Qt3D::QTranslateTransform;
    sphereTranslateTransform->setTranslation(QVector3D(5, 0, 10));
    sphereTransform->addTransform(sphereTranslateTransform);


    sphereEntity->addComponent(sphereMesh);
    sphereEntity->addComponent(sphereTransform);
    sphereEntity->addComponent(material);

    Qt3D::QEntity *sphereEntity2 = new Qt3D::QEntity();
    Qt3D::QTransform *sphereTransform2 = new Qt3D::QTransform;
    Qt3D::QTranslateTransform *sphereTranslateTransform2 = new Qt3D::QTranslateTransform;
    sphereTranslateTransform2->setTranslation(QVector3D(0, 0, 0));
    sphereTransform2->addTransform(sphereTranslateTransform2);

    sphereEntity2->addComponent(sphereMesh);
    sphereEntity2->addComponent(sphereTransform2);
    sphereEntity2->addComponent(material);
    qDebug()<<sphereTranslateTransform2->dx();
    qDebug()<<sphereTranslateTransform2->dy();
    qDebug()<<sphereTranslateTransform2->dz();




    Qt3D::QEntity *sphereEntity3 = new Qt3D::QEntity(rootEntity);
    Qt3D::QTransform *sphereTransform3 = new Qt3D::QTransform;
    Qt3D::QTranslateTransform *sphereTranslateTransform3 = new Qt3D::QTranslateTransform;
    sphereTranslateTransform3->setTranslation(QVector3D(0, 5, 20));
    sphereTransform3->addTransform(sphereTranslateTransform3);

    sphereEntity3->addComponent(sphereMesh);
    sphereEntity3->addComponent(sphereTransform3);
    sphereEntity3->addComponent(material);


    setRootEntity(rootEntity);

    sphereEntity2->setParent(rootEntity);



}

MainWindow::~MainWindow()
{

}

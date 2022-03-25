#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"renderWindow.h"
#include"functions.h"

#define MY_PI 3.14159265358979323846

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    cubeObject = new CubeObject;

    std::string path ("../renderOBJ/object_file/sphere.obj");

    importFile(path, &cubeObject->vertices,  &cubeObject->facesIdx);
    qDebug() << "Imported";

//    cubeObject->vertices.insert(cubeObject->vertices.end(),{{0,0,0,1},
//                                  {0,10,0,1},
//                                  {20,10,0,1},
//                                  {20,0,0,1},
//                                  {20,0,15,1},
//                                  {20,10,15,1},
//                                  {0,10,15,1},
//                                  {0,0,15,1}
//                                 });

//    cubeObject->facesIdx.insert(cubeObject->facesIdx.end(),{
//                                    {0,1,2,3},
//                                    {2,3,4,5},
//                                    {1,2,5,6},
//                                    {0,3,4,7},
//                                    {0,1,6,7},
//                                    {4,5,6,7}
//                                });

//        cubeObject->vertices.insert(cubeObject->vertices.end(),{
//                                        {15.0, -15.0, 15.0, 1},
//                                        {15.0, 15.0, 15.0,  1},
//                                        {-15.0, 15.0, 15.0,  1},
//                                        {-15.0, -15.0, 15.0, 1},
//                                        {15.0, -15.0, -15.0, 1},
//                                        {15.0, 15.0, -15.0, 1},
//                                        {-15.0, 15.0, -15.0, 1},
//                                        {-15.0, -15.0, -15.0, 1}
//                                     });
//            cubeObject->facesIdx.insert(cubeObject->facesIdx.end(),{
//                                            {0,1,2,3},
//                                            {1,0,4,5},
//                                            {5,4,7,6},
//                                            {6,7,3,2},
//                                            {1,5,6,2},
//                                            {0,3,7,4}

//                                        });

//
    cubeObject->curr_mat=0;

    cubeObject->calcVerticesNormal();


    ////////////GUI
    setFixedSize(400,550);
    ui->setupUi(this);
    renderwindow = new renderWindow;  
    raster1 = new raster;
    lightWhite = new lights;
    lightRed = new lights;
    lightSpec = new lights;

//    colorBtn = new QPushButton(this);
//    colorBtn->setText("Change Color");
//    connect(colorBtn, SIGNAL(clicked()),this, SLOT(pickColor()));

    orthoProyBtn = new QPushButton(this);
    orthoProyBtn->setText("Proyeccion Ortográfica");
    connect(orthoProyBtn, SIGNAL(clicked()),this, SLOT(setOrtho()));

    persProyBtn = new QPushButton(this);
    persProyBtn->setText("Proyeccion Perspectiva");
    connect(persProyBtn, SIGNAL(clicked()),this, SLOT(setPers()));

    setRotationBtn = new QPushButton(this);
    setRotationBtn->setText("Rotate Cube");
    connect(setRotationBtn, SIGNAL(clicked()),this, SLOT(setRotation()));

    switchCamera= new QPushButton(this);
    switchCamera->setText("Switch camera");
    connect(switchCamera, SIGNAL(clicked()),this, SLOT(setSwitchCamera()));

    light1On= new QPushButton(this);
    light1On->setText("Luz blanca");
    connect(light1On, SIGNAL(clicked()),this, SLOT(setLight1On()));

    light2On= new QPushButton(this);
    light2On->setText("Luz 2");
    connect(light2On, SIGNAL(clicked()),this, SLOT(setLight2On()));

    light3On= new QPushButton(this);
    light3On->setText("Especular");
    connect(light3On, SIGNAL(clicked()),this, SLOT(setLight3On()));

    gouraud= new QPushButton(this);
    gouraud->setText("Gouraud Shading");
    connect(gouraud, SIGNAL(clicked()),this, SLOT(setGouShade()));

    phong= new QPushButton(this);
    phong->setText("Phong Shading");
    connect(phong, SIGNAL(clicked()),this, SLOT(setPhongShade()));

    material1= new QPushButton(this);
    material1->setText("Material 1");
    connect(material1, SIGNAL(clicked()),this, SLOT(setMaterial1()));


    material2= new QPushButton(this);
    material2->setText("Material 2");
    connect(material2, SIGNAL(clicked()),this, SLOT(setMaterial2()));


    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(4);

    grid->addWidget(renderwindow, 0, 0,1,2);
    grid->addWidget(persProyBtn, 1, 0);
    grid->addWidget(orthoProyBtn, 1, 1);
    grid->addWidget(setRotationBtn, 6, 0);
    grid->addWidget(switchCamera, 2, 0,1,2);
    grid->addWidget(light1On,3,0,1,2);
    grid->addWidget(light2On, 4, 0);
    grid->addWidget(light3On, 4, 1);
    grid->addWidget(gouraud, 5, 0);
    grid->addWidget(phong, 5, 1);
    grid->addWidget(material1, 7, 0);
    grid->addWidget(material2, 7, 1);

    centralWidget()->setLayout(grid);

    ////////RENDER PROJECTIVE POINTS

    ///////////LIGHTS
    ///

    ambLight = new ambientLight;

    ambLight->intensity = 100;


    //lightWhite = new lights;
    lightWhite->intensity=10;
    lightWhite->lightPos.insert(lightWhite->lightPos.end(),{ 10,10,10});
    lightWhite->color[0]=0.8; lightWhite->color[1]=0.8; lightWhite->color[2]=0.8;

    //lightRed = new lights;
    lightRed->intensity=80;
    lightRed->lightPos.insert(lightRed->lightPos.end(),{ 10,10,10});
    lightRed->color[0]=0.0; lightRed->color[1]=0; lightRed->color[2]=0.8;

    lightSpec->intensity=0;
    lightSpec->lightPos.insert(lightSpec->lightPos.end(),{10,10,10});
    lightSpec->color[0]=0.8; lightSpec->color[1]=0.8; lightSpec->color[2]=0.8;
    lightSpec->p=1;


    timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawObject()));
    timer->start(30);


    setWindowTitle(tr("Lighting&Shading CG"));
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::pickColor()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    renderwindow->setPenColor(color);
}

void MainWindow::setOrtho()
{
    orthoProy=true;
    drawObject();
}

void MainWindow::setPers()
{
    orthoProy=false;
    drawObject();
}

void MainWindow::setRotation()
{

    rotateBool= !rotateBool;
    drawObject();
}

void MainWindow::drawObject(){

    lightScene.push_back(lightWhite);
    lightScene.push_back(lightRed);
    lightScene.push_back(lightSpec);


    raster1->pipeline(*cubeObject, lightScene, orthoProy, phongBool, camSelect);

        //RASTER A CANVAS
        renderwindow->pointsList.append(raster1->rasterPoint);
        raster1->rasterPoint.clear();
        renderwindow->pointsZ.append(raster1->rasterZ);
        raster1->rasterZ.clear();
        for(int i=0;i<=2;++i){
            renderwindow->pixelColor[i].append(raster1->rasterColor[i]);
            raster1->rasterColor[i].clear();

        }

//    //Rotar objeto en +1 grado
    if(rotateBool){
        cubeObject->rotateObject(1);
    }

    lightScene.clear();
    update();

}


void MainWindow::setSwitchCamera()
{
 camSelect +=1;
         if(camSelect>=3){
         camSelect=0;
}
 drawObject();
}

void MainWindow::setLight1On()
{
    if(l1){
        lightWhite->intensity=0;
        l1=false;
    }
    else{
        lightWhite->intensity=10;
        l1=true;
    }
    drawObject();
}

void MainWindow::setLight2On()
{
    if(l2){
        lightRed->intensity=0;
        l2=false;
    }
    else{
        lightRed->intensity=80;
        l2=true;
    }
    drawObject();

}

void MainWindow::setLight3On()
{
    if(l3){
        lightSpec->intensity=0;
        l3=false;
    }
    else{
        lightSpec->intensity=2;
        l3=true;
    }
    drawObject();

}

void MainWindow::setGouShade()
{
    phongBool =false;
}

void MainWindow::setPhongShade()
{
    phongBool =true;
}

void MainWindow::setMaterial1()
{
    cubeObject->curr_mat=0;
}

void MainWindow::setMaterial2()
{
    cubeObject->curr_mat=1;
}




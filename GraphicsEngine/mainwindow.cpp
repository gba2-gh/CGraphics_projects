#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"renderWindow.h"
#include"functions.h"
#include<qaction.h>
#include<QAction>
#include<QKeyEvent>
#include<fstream>
#include <sstream>
#include<qdebug.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stdint.h>
#include<stb_image.h>

#include"lodepng/lodepng.h"



#define MY_PI 3.14159265358979323846

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


//    std::ifstream fin("check.png", std::ios::in | std::ios::binary);
//    std::ostringstream oss;
//    oss << fin.rdbuf();
//    std::string data(oss.str());

//    //qDebug() << data;


//    int wi, he, bpp;

//    unsigned char* rgb_image = stbi_load("../GraphicsEngine/check.png", &wi, &he, &bpp, 3);

//    if(rgb_image == NULL) {
//        qDebug() << "Error in loading the image\n";
//    }

//    qDebug() << rgb_image;

//    stbi_image_free(rgb_image);



//    std::vector<unsigned char> image; //the raw pixels
//     unsigned width, height;

//      const char* filename = "../GraphicsEngine/check.png";

//     //decode
//     unsigned  error = lodepng::decode(image, width, height, filename);

//     //if there's an error, display it
//     if(error) {
//         qDebug() << error;
//        //qDebug() << lodepng_error_text(error);
//     }



     //QString url = R"(../GraphicsEngine/check.png)";
     //QImage texture_img(url);

     //QList<double> texture_img;

//     for(int i=0; i<img.height();i++){
//         for(int j=0; j<img.width();j++){
//             texture_img
//            qDebug() << img.pixelColor(1,1);
//    }

    shaderSel=1;

    cubeObject = new CubeObject;

    std::string path ("../GraphicsEngine/object_file/Cube"
                      ".obj");

    importFile(path, &cubeObject->vertices,  &cubeObject->facesIdx);
    qDebug() << "Imported";




    cubeObject->curr_mat=1;

    if(shaderSel !=0){
      cubeObject->calcVerticesNormal();
    }

    std::vector<std::vector<double> > uvCoord = {{1,0}, {1,1}, {0,1}, {0,0}};//, {0,0},
                       //                       {0,0}, {0,0}, {0,0}, {0,0}};

//     std::vector<std::vector<double> > uvCoord = {{0.5,0}, {0.5,0.5}, {0,0.5}, {0,0},
//                                              {1,0}, {1,0.5}, {0,0}, {0,0}};


    cubeObject->vertex_uvCoord = uvCoord;

    raster1 = new raster;

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

//    setRotationBtn = new QPushButton(this);
//    setRotationBtn->setText("Rotate Cube");
//    connect(setRotationBtn, SIGNAL(clicked()),this, SLOT(setRotation()));

//    switchCamera= new QPushButton(this);
//    switchCamera->setText("Switch camera");
//    connect(switchCamera, SIGNAL(clicked()),this, SLOT(setSwitchCamera()));

//    light1On= new QPushButton(this);
//    light1On->setText("Luz blanca");
//    connect(light1On, SIGNAL(clicked()),this, SLOT(setLight1On()));

    light2On= new QPushButton(this);
    light2On->setText("Luz Azul");
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



    QAction *action = new QAction(this);
    action->setShortcut(Qt::Key_R);
    connect(action,  SIGNAL(triggered()), this, SLOT(setRotation()));
    this->addAction(action);

//    switchCamera->addAction(action);




    QGridLayout *grid = new QGridLayout(this);
    grid->setSpacing(4);

    grid->addWidget(renderwindow, 0, 0,1,2);
    grid->addWidget(persProyBtn, 1, 0);
    grid->addWidget(orthoProyBtn, 1, 1);
    //grid->addWidget(setRotationBtn, 6, 0);
    //grid->addWidget(switchCamera, 2, 0,1,2);
    //grid->addWidget(light1On,3,0);
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


    lightWhite = new lights;
    lightWhite->intensity=10;
    lightWhite->lightPos.insert(lightWhite->lightPos.end(),{ -20,20,20});
    lightWhite->color[0]=0.8; lightWhite->color[1]=0.8; lightWhite->color[2]=0.8;

    lightRed = new lights;
    lightRed->intensity=80;
    lightRed->lightPos.insert(lightRed->lightPos.end(),{ 20,-20,20});
    lightRed->color[0]=0.01; lightRed->color[1]=0.01; lightRed->color[2]=0.8;

    lightSpec = new lights;
    lightSpec->intensity=0.1;
    lightSpec->lightPos.insert(lightSpec->lightPos.end(),{20,20,-20});
    lightSpec->color[0]=0.8; lightSpec->color[1]=0.8; lightSpec->color[2]=0.8;
    lightSpec->p=1;


//    timer= new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(drawObject()));
//    timer->start(30);
    drawObject();


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

    //rotateBool= !rotateBool;
    cubeObject->rotateObject(45);

    drawObject();
}

void MainWindow::drawObject(){

    QString url = R"(../GraphicsEngine/texture.png)";
    QImage texture_img(url);

    lightScene.push_back(lightWhite);
    lightScene.push_back(lightRed);
    lightScene.push_back(lightSpec);


    raster1->pipeline(*cubeObject, lightScene, orthoProy, phongBool, camSelect, shaderSel, texture_img);

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
        lightSpec->intensity=0.1;
        l3=true;
    }
    drawObject();

}

void MainWindow::setGouShade()
{
    phongBool =false;
    shaderSel=1;
    drawObject();
}

void MainWindow::setPhongShade()
{
    phongBool =true;
    shaderSel=2;
    drawObject();
}

void MainWindow::setMaterial1()
{
    cubeObject->curr_mat=0;
    drawObject();
}

void MainWindow::setMaterial2()
{
    cubeObject->curr_mat=1;
    drawObject();
}

void MainWindow::setCamera1()
{
    qDebug()<< "hi";
    update();

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{


    if(event->key()==Qt::Key_1){

        camSelect=0;
    }



    if(event->key()==Qt::Key_2){

        camSelect=1;
    }


    if(event->key()==Qt::Key_3){

        camSelect=2;
    }

    drawObject();

}




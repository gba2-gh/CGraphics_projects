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

#include <stdint.h>



#define MY_PI 3.14159265358979323846

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    raster1 = new raster;

    shaderSel=3;
    cubeObject = new CubeObject;

    cubeObject->curr_mat=1;

  //IMPORT
    std::string path ("../GraphicsEngine/object_file/sphere"
                      ".obj");

    importFile(path, &cubeObject->vertices,  &cubeObject->facesIdx, &cubeObject->vertexNormals);
    qDebug() << "Imported";

   // cubeObject->vertexNormals.clear();
   // cubeObject->calcVerticesNormal();
  //UV COORDINATES
     std::vector<std::vector<double> > uvCoord(cubeObject->vertices.size(), std::vector<double>(2) );


     //COORDINATES CUBE
//     uvCoord[0][0]= 0.5;
//     uvCoord[1][0]=0.5; uvCoord[1][1]=0.5;
//     uvCoord[2][1]=0.5;

//     std::vector<double> textureFaces_sel= {0} ;



//     uvCoord[27359][0]=0.25;
//     uvCoord[16715][0]=0.5;
//     uvCoord[27774][0]=0.75;
//     uvCoord[15254][0]=1;

//     uvCoord[22882][1]=0.25;
//     uvCoord[16714][0]=0.25; uvCoord[16714][1]=0.25;
//     uvCoord[16713][0]=0.5; uvCoord[16713][1]=0.25;
//     uvCoord[27772][0]=0.75; uvCoord[27772][1]=0.25;
//     uvCoord[15256][0]=1; uvCoord[15256][1]=0.25;

//     uvCoord[16716][0]=0; uvCoord[16716][1]=0.5;
//     uvCoord[15615][0]=0.25; uvCoord[15615][1]=0.5;
//     uvCoord[33029][0]=0.5; uvCoord[33029][1]=0.5;
//     uvCoord[27773][0]=0.75; uvCoord[27773][1]=0.5;
//     uvCoord[23997][0]=1.0; uvCoord[23997][1]=0.5;

//     uvCoord[15613][0]=0; uvCoord[15613][1]=0.75;
//     uvCoord[15614][0]=0.25; uvCoord[15614][1]=0.75;
//     uvCoord[15616][0]=0.5; uvCoord[15616][1]=0.75;
//     uvCoord[4635][0]=0.75; uvCoord[4635][1]=0.75;
//     uvCoord[31127][0]=1.0; uvCoord[31127][1]=0.75;

//     uvCoord[10168][0]=0; uvCoord[10168][1]=1.0;
//     uvCoord[5481][0]=0.25; uvCoord[5481][1]=1.0;
//     uvCoord[4637][0]=0.5; uvCoord[4637][1]=1.0;
//     uvCoord[4636][0]=0.75; uvCoord[4636][1]=1.0;



     uvCoord[6557][0]=0; uvCoord[6557][1]=0.0;
     uvCoord[6556][0]=0.125; uvCoord[6556][1]=0.0;
     uvCoord[30964][0]=0.250; uvCoord[30964][1]=0.0;
     uvCoord[3305][0]=0.375; uvCoord[3305][1]=0.0;
     uvCoord[3304][0]=0.5; uvCoord[3304][1]=0.0;
     uvCoord[30767][0]=0.625; uvCoord[30767][1]=0.0;
     uvCoord[20898][0]=0.75; uvCoord[20898][1]=0.0;

     uvCoord[34625][0]=0; uvCoord[34625][1]=0.125;
     uvCoord[8189][0]=0.125; uvCoord[8189][1]=0.125;
     uvCoord[27358][0]=0.250; uvCoord[27358][1]=0.125;
     uvCoord[32563][0]=0.375; uvCoord[32563][1]=0.125;
     uvCoord[15257][0]=0.5; uvCoord[15257][1]=0.125;
     uvCoord[15258][0]=0.625; uvCoord[15258][1]=0.125;
     uvCoord[15259][0]=0.75; uvCoord[15259][1]=0.125;

     uvCoord[31615][0]=0; uvCoord[31615][1]=0.25;
     uvCoord[27359][0]=0.125; uvCoord[27359][1]=0.25;
     uvCoord[16715][0]=0.250; uvCoord[16715][1]=0.25;
     uvCoord[27774][0]=0.375; uvCoord[27774][1]=0.25;
     uvCoord[15254][0]=0.5; uvCoord[15254][1]=0.25;
     uvCoord[15255][0]=0.625; uvCoord[15255][1]=0.25;
     uvCoord[27775][0]=0.75; uvCoord[27775][1]=0.25;

     uvCoord[22882][0]=0; uvCoord[22882][1]=0.375;
     uvCoord[16714][0]=0.125; uvCoord[16714][1]=0.375;
     uvCoord[16713][0]=0.250; uvCoord[16713][1]=0.375;
     uvCoord[27772][0]=0.375; uvCoord[27772][1]=0.375;
     uvCoord[15256][0]=0.5; uvCoord[15256][1]=0.375;
     uvCoord[8166][0]=0.625; uvCoord[8166][1]=0.375;
     uvCoord[8167][0]=0.75; uvCoord[8167][1]=0.375;

     uvCoord[16716][0]=0; uvCoord[16716][1]=0.0;
     uvCoord[15615][0]=0.125; uvCoord[15615][1]=0.5;
     uvCoord[33029][0]=0.250; uvCoord[33029][1]=0.5;
     uvCoord[27773][0]=0.375; uvCoord[27773][1]=0.5;
     uvCoord[23997][0]=0.5; uvCoord[23997][1]=0.5;
     uvCoord[8168][0]=0.625; uvCoord[8168][1]=0.5;
     uvCoord[23998][0]=0.75; uvCoord[23998][1]=0.5;

     uvCoord[15613][0]=0; uvCoord[15613][1]=0.625;
     uvCoord[15614][0]=0.125; uvCoord[15614][1]=0.625;
     uvCoord[15616][0]=0.250; uvCoord[15616][1]=0.625;
     uvCoord[4635][0]=0.375; uvCoord[4635][1]=0.625;
     uvCoord[31127][0]=0.5; uvCoord[31127][1]=0.625;
     uvCoord[11991][0]=0.625; uvCoord[11991][1]=0.625;
     uvCoord[11989][0]=0.75; uvCoord[11989][1]=0.625;

     uvCoord[10168][0]=0; uvCoord[10168][1]=0.75;
     uvCoord[5481][0]=0.125; uvCoord[5481][1]=0.75;
     uvCoord[4637][0]=0.250; uvCoord[4637][1]=0.75;
     uvCoord[4636][0]=0.375; uvCoord[4636][1]=0.75;
     uvCoord[25690][0]=0.5; uvCoord[25690][1]=0.75;
     uvCoord[11990][0]=0.625; uvCoord[11990][1]=0.75;
     uvCoord[34062][0]=0.75; uvCoord[34062][1]=0.75;







        //FACES TO RENDER TEXTURE
    std::vector<double> textureFaces_sel= {41146, 68765, 68764, 34347, 68732, 52246, 41147,
                                          23473, 23472, 12081, 12080, 7608, 67289,38831,
                                          28813, 28812, 7609, 6947, 6946, 67069, 67068,
                                          52247, 48721, 1689, 6900, 68733, 17335, 17334,
                                           68730, 42908,
                                           48725, 48724, 48723,48722,48720,
                                           13063,
                                           67101, 67100, 24136,13062,3154,
                                           66999, 66998, 31266, 23525, 3155,
                                           68731,62647, 59075, 59074, 42909, 40012, 31264,
                                           66912, 43473, 43472, 25862, 6726, 6725,
                                           66820,14838, 4955,
                                           67070,34346,24137,
                                           49075,49074, 40013, 16782,16783, 6724

                                          } ;




    cubeObject->textureFaces = textureFaces_sel;
    cubeObject->vertex_uvCoord = uvCoord;

//    qDebug() << cubeObject->facesIdx[6724];
//    qDebug() << cubeObject->vertices[31615];
//    qDebug() << cubeObject->vertices[31613];
//    qDebug() << cubeObject->vertices[4195];
    //cubeObject->rotateY(30);

    ////////////GUI

            setFixedSize(400,450);
    ui->setupUi(this);
    renderwindow = new renderWindow;
    raster1 = new raster;
    lightWhite = new lights;
    light2 = new lights;
    lightSpec = new lights;

//    colorBtn = new QPushButton(this);
//    colorBtn->setText("Change Color");
//    connect(colorBtn, SIGNAL(clicked()),this, SLOT(pickColor()));

//    orthoProyBtn = new QPushButton(this);
//    orthoProyBtn->setText("Proyeccion Ortográfica");
//    connect(orthoProyBtn, SIGNAL(clicked()),this, SLOT(setOrtho()));

//    persProyBtn = new QPushButton(this);
//    persProyBtn->setText("Proyeccion Perspectiva");
//    connect(persProyBtn, SIGNAL(clicked()),this, SLOT(setPers()));

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
    light2On->setText("Luz Azul (on/off)");
    connect(light2On, SIGNAL(clicked()),this, SLOT(setLight2On()));

//    light3On= new QPushButton(this);
//    light3On->setText("Especular");
//    connect(light3On, SIGNAL(clicked()),this, SLOT(setLight3On()));

//    gouraud= new QPushButton(this);
//    gouraud->setText("Gouraud Shading");
//    connect(gouraud, SIGNAL(clicked()),this, SLOT(setGouShade()));

//    phong= new QPushButton(this);
//    phong->setText("Phong Shading");
//    connect(phong, SIGNAL(clicked()),this, SLOT(setPhongShade()));

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
    //grid->addWidget(persProyBtn, 1, 0);
    //grid->addWidget(orthoProyBtn, 1, 1);
    //grid->addWidget(setRotationBtn, 6, 0);
    //grid->addWidget(switchCamera, 2, 0,1,2);
    //grid->addWidget(light1On,3,0);
    grid->addWidget(light2On, 3,0,0);
    //grid->addWidget(light3On, 4, 1);
    //grid->addWidget(gouraud, 5, 0);
    //grid->addWidget(phong, 5, 1);
    grid->addWidget(material1, 2, 0);
    grid->addWidget(material2, 2, 1);


    centralWidget()->setLayout(grid);

    ////////RENDER PROJECTIVE POINTS

    ///////////LIGHTS
    ///

    lightWhite = new lights;
    lightWhite->intensity=800;
    lightWhite->lightPos.insert(lightWhite->lightPos.end(),{ -2,2,2});
    lightWhite->color[0]=0.8; lightWhite->color[1]=0.8; lightWhite->color[2]=0.8;

    light2 = new lights;
    light2->intensity=800;
    light2->lightPos.insert(light2->lightPos.end(),{ 2,-2,2});
    light2->color[0]=0.1; light2->color[1]=0.1; light2->color[2]=0.9;


//    timer= new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(drawObject()));
//    timer->start(30);
    drawObject();


    setWindowTitle(tr("GraphicsEngine CG"));

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
    cubeObject->rotateY(30);
    qDebug()<< "rotacion";

    drawObject();
}

void MainWindow::drawObject(){

    //QString url = R"(../GraphicsEngine/texture.png)";
    QImage texture_img(cubeObject->all_mat[cubeObject->curr_mat].url);

    lightScene.push_back(lightWhite);
    lightScene.push_back(light2);
    lightScene.push_back(lightSpec);


    raster1->pipeline(*cubeObject, lightScene, orthoProy, phongBool, camSelect, shaderSel, texture_img);

        //RASTER A CANVAS
        renderwindow->pointsList.append(raster1->rasterPoint);
        raster1->rasterPoint.clear();
        renderwindow->pointsZ.append(raster1->rasterZ);
        raster1->rasterZ.clear();
        for(int i=0;i<=2;++i){
            renderwindow->pixelColor[i].append(raster1->rasterColorT[i]);
            raster1->rasterColor[i].clear();
             raster1->rasterColorT[i].clear();
        }

//    //Rotar objeto en +1 grado
    if(rotateBool){
        cubeObject->rotateX(1);
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
        lightWhite->intensity=1000;
        l1=true;
    }
    drawObject();
}

void MainWindow::setLight2On()
{
    if(l2){
        light2->intensity=0;
        l2=false;
    }
    else{
        light2->intensity=10;
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
        qDebug() << "camara 1";
    }



    if(event->key()==Qt::Key_2){

        camSelect=1;
        qDebug() << "camara 2";
    }


    if(event->key()==Qt::Key_3){

        camSelect=2;
        qDebug() << "camara 3";
    }

    drawObject();

}




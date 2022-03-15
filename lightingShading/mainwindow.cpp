#include "mainwindow.h"
#include "ui_mainwindow.h"

#include"renderWindow.h"
#include"camProjection.h"
#include"cubeobject.h"
//#include"transformations.h"
//#include"lights.h"

#define MY_PI 3.14159265358979323846

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ////////////GUI
    setFixedSize(400,500);
    ui->setupUi(this);
    renderwindow = new renderWindow;  

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

    fillPolyBtn= new QPushButton(this);
    fillPolyBtn->setText("Fill poly");
    connect(fillPolyBtn, SIGNAL(clicked()),this, SLOT(fillPoly()));

    drawEdgesBtn= new QPushButton(this);
    drawEdgesBtn->setText("Draw Edges");
    connect(drawEdgesBtn, SIGNAL(clicked()),this, SLOT(drawEdges()));


    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(renderwindow);
    mainLayout->addWidget(persProyBtn);
    mainLayout->addWidget(orthoProyBtn);
    mainLayout->addWidget(setRotationBtn);
    mainLayout->addWidget(fillPolyBtn);
    mainLayout->addWidget(drawEdgesBtn);
    centralWidget()->setLayout(mainLayout);



    ////////RENDER PROJECTIVE POINTS
    camProj = new CamProjection;
    camProj2= new CamProjection;
    cubeObject = new CubeObject;

    std::vector<std::vector<double> > camPos1= {{1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,1,-35},
                                                {0,0,0,1}};



    std::vector<std::vector<double> > camPos2= {{0,0,1,  -10},
                                                {0,1, 0,  20},
                                                {-1,0,0 ,-15},
                                                {0,0,0,1}};

    camProj->camMarco = camPos1;
    camProj2->camMarco=camPos2;


    ///////////LIGHTS
    ambLight = new ambientLight;

    ambLight->intensity = 1;

//    double I[3];

//    I[0]= (ambLight->intensity * cubeObject->ka[0]) *255 ;
//    I[1]= (ambLight->intensity * cubeObject->ka[1]) *255 ;
//    I[2]= (ambLight->intensity * cubeObject->ka[2]) *255 ;

//    for(int i=0; i<3; ++i){

//        I[i] += cubeObject->kd

//    }
    //renderwindow->pixelColor[0].append();

    //renderwindow->pointPen.setColor(QColor(I[0],I[1],I[2],255));






    drawObject();

    timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawObject()));
    timer->start(30);


    setWindowTitle(tr("3d Projection CG"));
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


    //Cámara 1
        qDebug() << fillPolyBool;
        camProj->fillPolyBool=fillPolyBool;
        camProj->projectPoint(*cubeObject, orthoProy, 400, 0, 400, 0);

        renderwindow->drawEdgesBool=drawEdgesBool;
        renderwindow->pointsList.append(camProj->rasterPoint);

        for(int i=0;i<=2;++i){
            renderwindow->pixelColor[i].append(camProj->rasterColor[i]);
            camProj->rasterColor[i].clear();

        }
       // qDebug() << renderwindow->pointsList;
        camProj->rasterPoint.clear();



//    //Cámara 2
//        camProj2->fillPolyBool=fillPolyBool;
//        camProj2->projectPoint(*cubeObject, orthoProy, 400, 0, 400, 0);

//        renderwindow->drawEdgesBool=drawEdgesBool;
//        renderwindow->pointsList.append(camProj2->rasterPoint);

//        for(int i=0;i<=2;++i){
//            renderwindow->pixelColor[i].append(camProj2->rasterColor[i]);
//            camProj2->rasterColor[i].clear();

//        }

//        camProj2->rasterPoint.clear();

//// qDebug()<< renderwindow->pointsList;

//    //Rotar objeto en +1 grado
    if(rotateBool){
        cubeObject->rotateObject(1);
    }

    update();

}

void MainWindow::fillPoly()
{
    fillPolyBool = !fillPolyBool;
    drawObject();
}

void MainWindow::drawEdges()
{
    drawEdgesBool= !drawEdgesBool;
}



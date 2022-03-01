#include "mainwindow.h"
#include "ui_mainwindow.h"

#include"renderWindow.h"
#include"camProjection.h"
#include"cubeobject.h"
//#include"transformations.h"

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


    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(renderwindow);
    mainLayout->addWidget(persProyBtn);
    mainLayout->addWidget(orthoProyBtn);
    mainLayout->addWidget(setRotationBtn);
    centralWidget()->setLayout(mainLayout);



    ////////RENDER PROJECTIVE POINTS
    camProj = new CamProjection;
    camProj2= new CamProjection;

    std::vector<std::vector<double> > camPos1= {{1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,1,-25},
                                                {0,0,0,1}};

    std::vector<std::vector<double> > camPos2= {{0,0,1,  -10},
                                                {0,1, 0,  5},
                                                {-1,0,0 ,-10},
                                                {0,0,0,1}};

    camProj->camMarco = camPos1;
    camProj2->camMarco=camPos2;

    cubeObject = new CubeObject;

    timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawObject()));
    timer->start(500);


    setWindowTitle(tr("3d Projection CG"));
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
}

void MainWindow::setPers()
{
    orthoProy=false;
}

void MainWindow::setRotation()
{
    rotateBool= !rotateBool;
}

void MainWindow::drawObject(){


    //Cámara 1
    //Enviar a cada vértice a la proyección con la cámara 1
        double punto[4];
        for(int k=0; k<8;++k){
            for(int j=0;j<4;++j){
                punto[j]=cubeObject->coord[k][j];
                }

            camProj->projectPoint(punto, orthoProy, 300, 100, 200, 0);
        }

        renderwindow->pointsList.append(camProj->rasterPoint);
        camProj->rasterPoint.clear();

    //Cámara 2
//    double punto2[4];
//    for(int k=0; k<8;++k){
//        for(int j=0;j<4;++j){
//            punto2[j]=cubeObject->coord[k][j];
//            }

//        camProj2->projectPoint(punto2, orthoProy, 300, 100, 400, 200);
//    }

//    renderwindow->pointsList.append(camProj2->rasterPoint);
//   // qDebug()<< renderwindow->pointsList;
//    camProj2->rasterPoint.clear();

//    //Rotar objeto en +1 grado
//    if(rotateBool){
//        currentAngle +=1;
//        cubeObject->rotateObject(currentAngle);
//    }

    update();

}



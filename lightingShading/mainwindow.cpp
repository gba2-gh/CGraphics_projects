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

    double I_R= (ambLight->intensity * cubeObject->ka[0]) *255 ;
    double I_G= (ambLight->intensity * cubeObject->ka[1]) *255 ;
    double I_B= (ambLight->intensity * cubeObject->ka[2]) *255 ;

    renderwindow->pointPen.setColor(QColor(I_R,I_G,I_B,255));


    timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(drawObject()));
    timer->start(30);


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
        camProj->fillPolyBool=fillPolyBool;
        camProj->projectPoint(cubeObject->vertices, orthoProy, 400, 000, 400, 0);

        renderwindow->drawEdgesBool=drawEdgesBool;
        renderwindow->pointsList.append(camProj->rasterPoint);
        camProj->rasterPoint.clear();


    //Cámara 2
//        camProj2->fillPolyBool=fillPolyBool;
//        camProj2->projectPoint(cubeObject->vertices, orthoProy, 300, 100, 400, 200);

//        renderwindow->drawEdgesBool=drawEdgesBool;
//        renderwindow->pointsList.append(camProj2->rasterPoint);
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
}

void MainWindow::drawEdges()
{
    drawEdgesBool= !drawEdgesBool;
}



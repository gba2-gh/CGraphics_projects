#include "mainwindow.h"
#include<QImage>
#include <QtGui/QImage>
#include"object.h"
#include"functions.h"

#include<math.h>
#define PI 3.14159265
MainWindow::MainWindow(QWidget *parent)
{
    setSurfaceType(QWindow::OpenGLSurface); //usar opengl para renderizar la ventana

    QSurfaceFormat format;

    format.setProfile(QSurfaceFormat::CompatibilityProfile); //fixed function pipelne
    format.setVersion(4,5); //version opengl
    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();
    if (!context->create())
       throw std::runtime_error("context creation failed");

    context->makeCurrent(this);


 //IMPORT
    bunnyObject = new Object;
    bunnyObject->curr_mat=0;

    std::string path ("../ArtGallery/object_file/bunny3.obj");
    importFile(path, bunnyObject);

    //bunnyObject->model.translate(3.0f,0.0f,0.0f);

    //COPY

    box_obj = new Object;
    box_obj->curr_mat=0;

    std::string path2 ("../ArtGallery/object_file/box.obj");
    importFile(path2, box_obj);


    wall_obj = new Object;
    wall_obj->curr_mat=0;

    std::string path3 ("../ArtGallery/object_file/wall.obj");
    importFile(path3, wall_obj);



    qDebug() << "Imported";


 ///TRANSLATE OBJECTS
    bunnyObject->model.translate(0.0f, 2.5f, 0.0f);
    box_obj->model.translate(0.0f, 0.0f, 0.0f);
    wall_obj->model.translate(0.0f, 5.0f, -6.0f);



    camPos.setX(0.0f); camPos.setY(3.0f); camPos.setZ(15.0f);
    camFront.setX(0.0f); camFront.setY(4.0f); camFront.setZ(-1.0f);
    camUp.setX(0.0f); camUp.setY(1.0f); camUp.setZ(0.0f);

}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST); //Profundidad
    resizeGL(this->width(), this->height());

    program = new QOpenGLShaderProgram();

    program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/shader.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/shader.frag");
    program->link();


    shader= new QOpenGLShaderProgram();

    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/test.vert");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/test.frag");
    shader->link();

    f = QOpenGLContext::currentContext()->extraFunctions();

}

void MainWindow::resizeGL(int w, int h)
{
      glViewport(0,0,w,h); //coordenadas de ventana, viewport
      qreal aspectRatio = qreal(w)/qreal(h);

 //matriz de proyeccion
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();

      gluPerspective(100, aspectRatio, 0.1, 400000000);

 //MODEL VIEW
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

}

void MainWindow::paintGL()
{
    ////RENDERING CALLBACK
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        //bunnyObject->model.translate(-1.0f,0.0f,0.0f);

        QMatrix4x4 proj;
        proj.perspective(45.0f,1, 0.1f, 100.0f);

        QVector3D direction;
        direction.setX( cos(yaw* PI/180) * cos(pitch* PI/180) );
        direction.setY( sin(pitch* PI/180) );
        direction.setZ( sin(yaw *PI/180) * cos(pitch* PI/180)   );
        direction.normalize();
        //camFront = direction;


        QMatrix4x4 view;
        view.lookAt(camPos, camFront, camUp);


//        QVector3D cameraPos(3.0f, 0.0f,  8.0f);
//        QVector3D cameraFront(-2.0f, 0.0f, -1.0f);
//        QVector3D cameraUp(0.0f, 1.0f,  0.0f);

//        if(camSelect==0){
//            view.translate(0.0f, -1.0f, -8.0f);
//        }else if(camSelect==1){
//            view.rotate(20.0,0,1,0 );
//            view.translate(0.0f, -1.0f, -0.0f);
//        }else{
//            view.rotate(180.0,0,1,0 );
//            view.translate(0.0f, -1.0f, -8.0f);
//        }


        bunnyObject->curr_mat=curr_mat;
        bunnyObject->render(proj,view, camPos, program);

        box_obj->curr_mat = curr_mat;
        box_obj->render(proj, view, camPos, program);

        wall_obj->curr_mat = curr_mat;
        wall_obj->render(proj, view, camPos, program);



}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeGL(this->width(), this->height());

    this->update();

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    paintGL();

}



void MainWindow::keyPressEvent(QKeyEvent *event)
{

    float inc=0.5;

    if(event->key()==Qt::Key_N){

    pitch +=1;

    if(pitch > 89.0f)
         pitch = 89.0f;
    }

    if(event->key()==Qt::Key_M){

    pitch -=1;
    if(pitch < -89.0f)
        pitch = -89.0f;

    }

    if(event->key()==Qt::Key_W){
        camPos += camFront * inc;
        //view.translate(1.0f,0.0f,0.0f);

    }

    if(event->key()==Qt::Key_S){

        camPos -= camFront * inc;

    }

    if(event->key()==Qt::Key_D){
        QVector3D result ;

        result = Q_cross_product(camFront, camUp);
        result.normalize();
        camPos += result * inc;

                qDebug() << "derecha";

    }


    if(event->key()==Qt::Key_A){
        QVector3D result ;

        result = Q_cross_product(camFront, camUp);
        result.normalize();
        camPos -= result * inc;

                qDebug() << "izq";

    }



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

    if(event->key()==Qt::Key_4){
        camSelect=3;
        qDebug() << "camara 4";
    }


    if(event->key()==Qt::Key_L){
        luzOnOff= !luzOnOff;
        qDebug() << "LUZ";
    }

    if(event->key()==Qt::Key_P){
        shaderPhong = !shaderPhong;
        qDebug() << "phong";
    }

    if(event->key()==Qt::Key_8){
        curr_mat = 0;
        qDebug() << "Material 1";
    }

    if(event->key()==Qt::Key_9){
        curr_mat = 1;
        qDebug() << "Material 2";
    }

   this->update();
}

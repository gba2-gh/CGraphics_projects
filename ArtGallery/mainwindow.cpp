#include "mainwindow.h"
#include<QImage>
#include <QtGui/QImage>
#include"object.h"
#include"functions.h"
#include"light.h"

#include<math.h>
#define PI 3.14159265
MainWindow::MainWindow(QWidget *parent)
{
//CONTEXT
    setSurfaceType(QWindow::OpenGLSurface); //usar opengl para renderizar la ventana

    QSurfaceFormat format;

    format.setProfile(QSurfaceFormat::CoreProfile); //fixed function pipelne
    format.setVersion(4,5); //version opengl
    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();
    if (!context->create())
       throw std::runtime_error("context creation failed");

    context->makeCurrent(this);


//IMPORT
    setScene();

    setLights();

    camPos.setX(0.0f); camPos.setY(5.0f); camPos.setZ(20.0f);
    camFront.setX(0.0f); camFront.setY(0.0f); camFront.setZ(-1.0f);
    camUp.setX(0.0f); camUp.setY(1.0f); camUp.setZ(0.0f);

//    timer= new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//    timer->start(300);
    


}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST); //Profundidad

/// compile shaders
    compileShaders();

    f = QOpenGLContext::currentContext()->extraFunctions();

    //crear famebuffer
    //unsigned int frameBuffer ;

    f->glGenFramebuffers(1, &frameBuffer);

    //crear textura de profundidad
    //unsigned int depthMap;
    glGenTextures(1, &depthMap1);
    glBindTexture(GL_TEXTURE_2D, depthMap1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    f->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    //attach a texture image to a framebuffer object
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap1, 0);
    //no color data
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if(f->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                qDebug() << "framebuffer false";


    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //RENDER SHADOW MAP
    genDepthMap();

    setCubeTexture();


}

void MainWindow::resizeGL(int w, int h)
{

}

void MainWindow::paintGL()
{

//regresar buffer original
    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ////RENDERING CALLBACK
    glViewport(0,0, this->width() , this->height());
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


    renderEnviroment();
    renderScene();
    //renderShadowMapDebug();



}




void MainWindow::renderScene()
{    
///CAMERAS
///
    float ar = (float)this->width()/(float)this->height();

    QMatrix4x4 proj;
    QMatrix4x4 view;

    proj.perspective(45.0f, ar  , 0.1f, 100.0f);
    view.lookAt(camPos, camPos+ camFront, camUp);
///
    phong->bind();

    int projLocation = phong->uniformLocation("proj");
    int viewLocation = phong->uniformLocation("view");
    phong->setUniformValue(projLocation, proj);
    phong->setUniformValue(viewLocation, view);

///TEXTURE
    phong->setUniformValue("textImage", 0);

    int shadowMapLoc= phong->uniformLocation("shadowMap");
    phong->setUniformValue( shadowMapLoc, 1);
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, depthMap1);

///RENDER
    setTextures(floorObj->mat1.path);
    setShaderValues(floorObj);
    floorObj->render();

    setTextures(wallObj_1->mat1.path);
    setShaderValues(wallObj_1);
    wallObj_1->render();

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap1);

    setShaderValues(boxObj_1);
    boxObj_1->render();
    setShaderValues(boxObj_2);
    boxObj_2->render();
    setShaderValues(boxObj_3);
    boxObj_3->render();
    setShaderValues(boxObj_4);
    boxObj_4->render();
    setShaderValues(boxObj_5);
    boxObj_5->render();
    setShaderValues(boxObj_6);
    boxObj_6->render();
    setShaderValues(boxObj_7);
    boxObj_7->render();

    setShaderValues(venusObj);
    venusObj->render();

    setTextures(scholarObj->mat1.path);
    setShaderValues(scholarObj);
    scholarObj->render();

    setShaderValues(nude);
    nude->render();

    setTextures(roosterObj->mat1.path);
    setShaderValues(roosterObj);
    roosterObj->render();

    setTextures(estela->mat1.path);
    setShaderValues(estela);
    estela->render();

    QMatrix4x4 normalMat;
    if(reflec){
        reflection_shader->bind();

        normalMat = bunnyObject->model.transposed();
        normalMat = normalMat.inverted();
        reflection_shader->setUniformValue("normalMat", normalMat);
        reflection_shader->setUniformValue("proj", proj);
        reflection_shader->setUniformValue("view", view);
        reflection_shader->setUniformValue("model", bunnyObject->model);
        reflection_shader->setUniformValue("eyePos", camPos);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyCubeTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap1);

        bunnyObject->render();
    }else{
        phong->bind();
        setShaderValues(bunnyObject);
        bunnyObject->render();

    }


    if(movement){
        movement_shader->bind();

        movement_time +=0.05;
        movement_shader->setUniformValue("Time", movement_time);

        normalMat = cuadro->model.transposed();
        normalMat = normalMat.inverted();
        movement_shader->setUniformValue("normalMat", normalMat);

        movement_shader->setUniformValue("proj", proj);
        movement_shader->setUniformValue("view", view);
        movement_shader->setUniformValue("model", cuadro->model);
        movement_shader->setUniformValue("eyePos", camPos);
        setTextures(cuadro->mat1.path);
        setMovementShader(cuadro);
        cuadro->render();
    }else{
        phong->bind();
        setTextures(cuadro->mat1.path);
        setShaderValues(cuadro);
        cuadro->render();
    }

    if(toon){
        toon_shader->bind();
        normalMat = vivi->model.transposed();
        normalMat = normalMat.inverted();
        toon_shader->setUniformValue("normalMat", normalMat);

        toon_shader->setUniformValue("proj", proj);
        toon_shader->setUniformValue("view", view);
        toon_shader->setUniformValue("model", vivi->model);
        toon_shader->setUniformValue("eyePos", camPos);

        setTextures(vivi->mat1.path);
        setToonShader(vivi);
        vivi->render();
    }else{
        phong->bind();
        setTextures(vivi->mat1.path);
        setShaderValues(vivi);
        vivi->render();
    }
}







void MainWindow::keyPressEvent(QKeyEvent *event)
{
////CAMERA MOV
///
///
    float inc=0.5;

    if(event->key()==Qt::Key_J){
        camFront.setX(camFront.x()-0.1);
         camFront.normalize();
         qDebug() << camFront;

    }

    if(event->key()==Qt::Key_L){
        camFront.setX(camFront.x()+0.1);
        camFront.normalize();
        qDebug() << camFront;
    }

    if(event->key()==Qt::Key_I){
        camFront.setY(camFront.y()+0.1);
        camFront.normalize();
        qDebug() << camFront;
    }

    if(event->key()==Qt::Key_K){
        camFront.setY(camFront.y()-0.1);
        camFront.normalize();
        qDebug() << camFront;
    }

    if(event->key()==Qt::Key_W){
        camPos += camFront * inc;
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

////// LIGHTS
///
    if(event->key()==Qt::Key_1){
        if(std::find(all_lights.begin(), all_lights.end(), spotLight_1_2) != all_lights.end()){
        all_lights.erase(all_lights.begin() + 2);
        }else{
            all_lights.insert(all_lights.begin() + 2, spotLight_1_2);
        }
        genDepthMap();
    }
    if(event->key()==Qt::Key_2){
        if(std::find(all_lights.begin(), all_lights.end(), spotLight_2_2) != all_lights.end()){
        all_lights.erase(all_lights.begin() + 4);
        }else{
            all_lights.insert(all_lights.begin() + 4, spotLight_2_2);
        }
        genDepthMap();
    }
    if(event->key()==Qt::Key_3){
        if(std::find(all_lights.begin(), all_lights.end(), spotLight_3_2) != all_lights.end()){
        all_lights.erase(all_lights.begin() + 6);
        }else{
            all_lights.insert(all_lights.begin() + 6, spotLight_3_2);
        }
        genDepthMap();
    }
    if(event->key()==Qt::Key_4){
        if(std::find(all_lights.begin(), all_lights.end(), spotLight_4_2) != all_lights.end()){
        all_lights.erase(all_lights.begin() + 8);
        }else{
            all_lights.insert(all_lights.begin() + 8, spotLight_4_2);
        }
        genDepthMap();
    }

    if(event->key()==Qt::Key_5){
        if(std::find(all_lights.begin(), all_lights.end(), spotLight_5_2) != all_lights.end()){
            all_lights.erase(all_lights.begin() + 10);
            }else{
                all_lights.insert(all_lights.begin() + 10, spotLight_5_2);
        }
        genDepthMap();
    }

    if(event->key()==Qt::Key_6){
        if(std::find(all_lights.begin(), all_lights.end(), spotLight_6_2) != all_lights.end()){
            all_lights.erase(all_lights.begin() + 12);
            }else{
                all_lights.insert(all_lights.begin() + 12, spotLight_6_2);
        }
        genDepthMap();
    }

        if(event->key()==Qt::Key_7){
            if(std::find(all_lights.begin(), all_lights.end(), spotLight_7_2) != all_lights.end()){
                all_lights.pop_back();
                }else{
                    all_lights.insert(all_lights.end(), spotLight_7_2);
            }
            genDepthMap();
        }



        if(event->key()==Qt::Key_B){
            reflec = !reflec;
        }

        if(event->key()==Qt::Key_N){
            toon = !toon;
        }
        if(event->key()==Qt::Key_M){
            movement = !movement;
        }





    this->update();
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













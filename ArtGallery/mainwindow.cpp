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

    camPos.setX(0.0f); camPos.setY(5.0f); camPos.setZ(10.0f);
    camFront.setX(0.0f); camFront.setY(0.0f); camFront.setZ(-1.0f);
    camUp.setX(0.0f); camUp.setY(1.0f); camUp.setZ(0.0f);

    timer= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(30);
    


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
     float ar = (float)this->width()/(float)this->height();
     float near_plane = 1.0f, far_plane = 30.0f;

//regresar buffer original
    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ////RENDERING CALLBACK
        glViewport(0,0, this->width() , this->height());
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        renderEnviroment();


        //CAMERAS

                QMatrix4x4 proj;
                QMatrix4x4 view;
                //proj.frustum(-1.0f*ar, 1.0f*ar, -1.0f, 1.0f, near_plane, far_plane);
                proj.perspective(90.0f, ar  , 0.1f, 100.0f);
                view.lookAt(camPos, camPos+ camFront, camUp);

        reflection_shader->bind();
        QMatrix4x4 model_r;
        QMatrix4x4 normalMat;

        model_r.translate(0.0f, 5.0f, 0.0f);
        normalMat = model_r.transposed();
        normalMat = normalMat.inverted();
        reflection_shader->setUniformValue("normalMat", normalMat);

        reflection_shader->setUniformValue("proj", proj);
        reflection_shader->setUniformValue("view", view);
        reflection_shader->setUniformValue("model", model_r);
        reflection_shader->setUniformValue("eyePos", camPos);

     /// DRAW
//         f->glBindVertexArray(VAO);
//         glActiveTexture(GL_TEXTURE0);
//         glBindTexture(GL_TEXTURE_CUBE_MAP, skyCubeTexture);

//          glDrawArrays(GL_TRIANGLES, 0, 36);
//          f->glBindVertexArray(0);



            program->bind();

            int projLocation = program->uniformLocation("proj");
            int viewLocation = program->uniformLocation("view");
            program->setUniformValue(projLocation, proj);
            program->setUniformValue(viewLocation, view);
//RENDER SCENE

        /// TEXTURE PARAMETERS
               QImage texture_img = bunnyObject->all_mat[curr_mat].texture;

               texture_img=texture_img.convertToFormat(QImage::Format_RGB888);
               uint t1;
               glGenTextures(1, &t1); // generate a unique ID for this texture
               glBindTexture(GL_TEXTURE_2D, t1); // create texture d

               // interpolacion, wrapping
               glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
               glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

               glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_img.height(), texture_img.width(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture_img.bits());
                //target, level, internalFormat, width, height, border, format, type, pointer to texels

               program->setUniformValue("textImage", 0);
               glActiveTexture(GL_TEXTURE0);
               glBindTexture(GL_TEXTURE_2D, t1);

                int shadowMapLoc= program->uniformLocation("shadowMap");
                program->setUniformValue( shadowMapLoc, 1);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, depthMap1);

        setShaderValues(floor_obj, proj, view);
        floor_obj->render(program);

        setShaderValues(wall_obj, proj, view);
        wall_obj->render(program);

        setShaderValues(box_obj, proj, view);
        box_obj->render(program);

        setShaderValues(bunnyObject, proj, view);
        bunnyObject->render(program);


////quad
        renderShadowMapDebug();


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


void MainWindow::renderScene()
{

}



void MainWindow::compileShaders()
{

    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/shader.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/shader.frag");
    program->link();

    depthMap_shader= new QOpenGLShaderProgram();
    depthMap_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/depthMap.vert");
    depthMap_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/depthMap.frag");
    depthMap_shader->link();

    depthMap_shader_quad = new QOpenGLShaderProgram();
    depthMap_shader_quad->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/depthMap_quad.vert");
    depthMap_shader_quad->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/depthMap_quad.frag");
    depthMap_shader_quad->link();

    cubemapEnv_shader = new QOpenGLShaderProgram();
    cubemapEnv_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/cubemapEnv.vert");
    cubemapEnv_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/cubemapEnv.frag");
    cubemapEnv_shader->link();

    reflection_shader = new QOpenGLShaderProgram();
    reflection_shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../ArtGallery/shaders/reflection.vert");
    reflection_shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../ArtGallery/shaders/reflection.frag");
    reflection_shader->link();


}








void MainWindow::keyPressEvent(QKeyEvent *event)
{
////CAMERA MOV
///
///
    float inc=0.5;

    if(event->key()==Qt::Key_N){
        camFront.setX(camFront.x()-0.1);
         camFront.normalize();
         qDebug() << camFront;

    }

    if(event->key()==Qt::Key_M){
        camFront.setX(camFront.x()+0.1);
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


 ////
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















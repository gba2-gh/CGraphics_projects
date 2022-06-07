#include "mainwindow.h"
#include<QImage>
#include <QtGui/QImage>
#include"object.h"
#include"functions.h"
#include"lights.h"

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

    camPos.setX(6.0f); camPos.setY(8.0f); camPos.setZ(6.0f);
    camFront.setX(1.0f); camFront.setY(1.0f); camFront.setZ(1.0f);
    camUp.setX(0.0f); camUp.setY(1.0f); camUp.setZ(0.0f);

}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST); //Profundidad

/// compile shaders
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



}

void MainWindow::resizeGL(int w, int h)
{

}

void MainWindow::paintGL()
{
     float ar = (float)this->width()/(float)this->height();
     float near_plane = 1.0f, far_plane = 30.0f;

     glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    genDepthMap();

    /// SHADOW MAP RENDER
         QMatrix4x4 proj_light;
         QMatrix4x4 view_light;


    //sun light
    proj_light.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    view_light.lookAt(spotLight_1_1->position, spotLight_1_1->direction, camUp);


 //RENDER SHADOW MAP





//regresar buffer original
    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ////RENDERING CALLBACK
        glViewport(0,0, this->width() , this->height());
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


//CAMERAS
        program->bind();

        QMatrix4x4 proj;
        QMatrix4x4 view;
        //proj.frustum(-1.0f*ar, 1.0f*ar, -1.0f, 1.0f, near_plane, far_plane);
        proj.perspective(90.0f, ar  , 0.1f, 100.0f);
        view.lookAt(camPos, camFront, camUp);



        program->setUniformValue("proj_light", proj_light);
        program->setUniformValue("view_light", view_light);


//RENDER


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

//        setShaderValues(box_obj, proj, view);
//        box_obj->render(program);

        setShaderValues(bunnyObject, proj, view);
        bunnyObject->render(program);


////quad
        //glViewport(0,0, 240 , 194);
         glViewport(0,0, 512 , 512);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        depthMap_shader_quad->bind();
        //depthMap_shader_quad->setUniformValue("depthMap", 0);
        depthMap_shader_quad->setUniformValue("near_plane", near_plane);
        depthMap_shader_quad->setUniformValue("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap1);

        depthMap_shader_quad->setUniformValue("depthMap", 0);

        unsigned int quadVAO = 0;
        unsigned int quadVBO;

        float quadVertices[] = {
                    // positions        // texture Coords
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                };
                // setup plane VAO
                f->glGenVertexArrays(1, &quadVAO);
                f->glGenBuffers(1, &quadVBO);
                f->glBindVertexArray(quadVAO);
                f->glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
                f->glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
                f->glEnableVertexAttribArray(0);
                f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                f->glEnableVertexAttribArray(1);
                f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

            f->glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            f->glBindVertexArray(0);

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


void MainWindow::setShaderValues(Object *object, QMatrix4x4 proj, QMatrix4x4 view)
{
    QMatrix4x4 model;
    model= object->model;

    /// CAMERA PARAMETERS
           int projLocation = program->uniformLocation("proj");
           int viewLocation = program->uniformLocation("view");
           int modelLocation = program->uniformLocation("model");


           program->setUniformValue(projLocation, proj);
           program->setUniformValue(viewLocation, view);
           program->setUniformValue(modelLocation, model);

           QMatrix4x4 normalMat;
           normalMat = model.transposed();
           normalMat = normalMat.inverted();
           program->setUniformValue("normalMat", normalMat);


    /// LIGHTS PARAMETERS
    ///    LIGHT1
    ///
           int lightPosLoc = program->uniformLocation("light.pos");
           int lightColorLoc = program->uniformLocation("light.color");

           program->setUniformValue(lightColorLoc, spotLight_1_1->color[0], spotLight_1_1->color[1], spotLight_1_1->color[2] );
           program->setUniformValue(lightPosLoc, spotLight_1_1->position);
           program->setUniformValue("spotlightDir", spotLight_1_1->direction);
           program->setUniformValue("light.intensity", spotLight_1_1->intensity);

           program->setUniformValue("light.attFactor_k", 1.0f);
           program->setUniformValue("light.attFactor_l", 0.22f);
           program->setUniformValue("light.attFactor_q", 0.20f);
           program->setUniformValue("cut_spot", spotLight_1_1->cut_angle);


           /// LIGHT 2
           int lightPos2Loc = program->uniformLocation("sun_light.pos");
           int lightColor2Loc = program->uniformLocation("sun_light.color");

           program->setUniformValue(lightColor2Loc, sun_light->color[0],sun_light->color[1], sun_light->color[2]);
           program->setUniformValue(lightPos2Loc, sun_light->position);

           program->setUniformValue("sun_light.intensity", 1.0f);

           program->setUniformValue("sun_light.attFactor_k", 1.0f);
           program->setUniformValue("sun_light.attFactor_l", 0.35f);
           program->setUniformValue("sun_light.attFactor_q", 0.44f);



           /// MATERIAL PARAMETERS
           program->setUniformValue("ka", object->all_mat[curr_mat].ka[0],
                   object->all_mat[curr_mat].ka[1],
                   object->all_mat[curr_mat].ka[2]);
           program->setUniformValue("kd", object->all_mat[curr_mat].kd[0],
                   object->all_mat[curr_mat].kd[1],
                   object->all_mat[curr_mat].kd[2]);
           program->setUniformValue("ke", object->all_mat[curr_mat].ke[0],
                   object->all_mat[curr_mat].ke[1],
                   object->all_mat[curr_mat].ke[2]);
           program->setUniformValue("shininess", object->all_mat[curr_mat].ro);
           program->setUniformValue("eyePos", camPos);


}


void MainWindow::setScene()
{

///IMPORT
    bunnyObject = new Object;
    bunnyObject->curr_mat=0;

    std::string path = "../ArtGallery/object_file/bunny3.obj";
    importFile(path, bunnyObject);

    box_obj = new Object;
    path =  "../ArtGallery/object_file/box.obj";
    importFile(path, box_obj);

    wall_obj = new Object;
    path = "../ArtGallery/object_file/wall.obj";
    importFile(path, wall_obj);

    floor_obj = new Object;
    path =  "../ArtGallery/object_file/piso.obj";
    importFile(path, floor_obj);



///TRANSLATE OBJECTS
    bunnyObject->model.translate(0.0f, 2.5f, 0.0f);
    //bunnyObject->model.rotate(50.0f, 0.0f, 1.0f, 1.0f);
    box_obj->model.translate(0.0f, 0.0f, 0.0f);
    //box_obj->model.rotate((45.0f), 1.0f, 0.0f, 0.0f);
    wall_obj->model.translate(0.0f, 5.0f, -6.0f);
    //wall_obj->model.rotate((60.0f), 1.0f, 1.0f, 0.0f);


    qDebug() << "Imported";

}

void MainWindow::setLights()
{

    sun_light = new Lights;
    sun_light->type = 0;
    sun_light->direction.setX(0.0f); sun_light->direction.setY(0.0f); sun_light->direction.setZ(0.0f);
    sun_light->position.setX(-3.0f); sun_light->position.setY(5.0f); sun_light->position.setZ(3.0f);
    sun_light->intensity = 2.0;

    all_lights.push_back(sun_light);

    spotLight_1_1 = new Lights;
    spotLight_1_1->type = 1;
    spotLight_1_1->direction.setX(0.0f); spotLight_1_1->direction.setY(0.0f); spotLight_1_1->direction.setZ(-1.0f);
    spotLight_1_1->position.setX(-2.0f); spotLight_1_1->position.setY(5.0f); spotLight_1_1->position.setZ(5.0f);
    spotLight_1_1->color[0] = 0.8f; spotLight_1_1->color[1] = 0.8f; spotLight_1_1->color[2] = 0.8f;
    spotLight_1_1->cut_angle = cos(30.0f * PI/180);

    spotLight_1_2 = new Lights;
    spotLight_1_2->type = 1;
    spotLight_1_2->direction.setX(0.0f); spotLight_1_2->direction.setY(0.0f); spotLight_1_2->direction.setZ(-1.0f);
    spotLight_1_2->position.setX(2.0f); spotLight_1_2->position.setY(5.0f); spotLight_1_2->position.setZ(5.0f);
    spotLight_1_2->color[0] = 0.8f; spotLight_1_2->color[1] = 0.1f; spotLight_1_2->color[2] = 0.1f;
    spotLight_1_2->cut_angle = cos(30.0f * PI/180);

    all_lights.push_back(spotLight_1_1);
    all_lights.push_back(spotLight_1_2);

}

void MainWindow::genDepthMap()
{
    float ar = (float)this->width()/(float)this->height();
    float near_plane = 1.0f, far_plane = 30.0f;

    /// SHADOW MAP RENDER
         QMatrix4x4 proj_light;
         QMatrix4x4 view_light;


    //sun light
    proj_light.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    view_light.lookAt(spotLight_1_1->position, spotLight_1_1->direction, camUp);
    //spot light
    //proj_light.perspective(80.0f, ar  , 0.1f, 100.0f);
    //view_light.lookAt(spotLight_1_1->position, spotLight_1_1->direction, camUp);


    f->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0,0, 1024 , 1024);
    glClear(GL_DEPTH_BUFFER_BIT);

    depthMap_shader->bind();
    int model_loc = depthMap_shader->uniformLocation("model");
    depthMap_shader->setUniformValue("proj", proj_light);
    depthMap_shader->setUniformValue("view", view_light);

    //render
    depthMap_shader->setUniformValue(model_loc, floor_obj->model);
    floor_obj->render(depthMap_shader);

    depthMap_shader->setUniformValue(model_loc, wall_obj->model);
    wall_obj->render(depthMap_shader);

//    depthMap_shader->setUniformValue(model_loc, box_obj->model);
//    box_obj->render(depthMap_shader);

    depthMap_shader->setUniformValue(model_loc, bunnyObject->model);
    bunnyObject->render(depthMap_shader);




    glViewport(1024,0, 1024 , 1024);
    QMatrix4x4 proj_light2;
    QMatrix4x4 view_light2;
    proj_light2.ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    view_light2.lookAt(sun_light->position, sun_light->direction, camUp);

    depthMap_shader->bind();
    depthMap_shader->setUniformValue("proj", proj_light2);
    depthMap_shader->setUniformValue("view", view_light2);

    depthMap_shader->setUniformValue(model_loc, floor_obj->model);
    floor_obj->render(depthMap_shader);

    depthMap_shader->setUniformValue(model_loc, wall_obj->model);
    wall_obj->render(depthMap_shader);

    depthMap_shader->setUniformValue(model_loc, bunnyObject->model);
    bunnyObject->render(depthMap_shader);




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
















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

    box_obj = new Object;
    box_obj->curr_mat=0;

    std::string path2 ("../ArtGallery/object_file/box.obj");
    importFile(path2, box_obj);


    wall_obj = new Object;
    wall_obj->curr_mat=0;

    std::string path3 ("../ArtGallery/object_file/wall.obj");
    importFile(path3, wall_obj);


    floor_obj = new Object;
    floor_obj->curr_mat=0;

    std::string path4 ("../ArtGallery/object_file/piso.obj");
    importFile(path4, floor_obj);


    qDebug() << "Imported";


 ///TRANSLATE OBJECTS
    bunnyObject->model.translate(0.0f, 2.5f, 0.0f);
    //bunnyObject->model.rotate(50.0f, 0.0f, 1.0f, 1.0f);
    box_obj->model.translate(0.0f, 0.0f, 0.0f);
    //box_obj->model.rotate((45.0f), 1.0f, 0.0f, 0.0f);
    wall_obj->model.translate(0.0f, 5.0f, -6.0f);
    //wall_obj->model.rotate((60.0f), 1.0f, 1.0f, 0.0f);




    camPos.setX(5.0f); camPos.setY(10.0f); camPos.setZ(0.0f);
    camFront.setX(1.0f); camFront.setY(1.0f); camFront.setZ(1.0f);
    camUp.setX(0.0f); camUp.setY(1.0f); camUp.setZ(0.0f);

    sun_light = new Lights;
    sun_light->type = 0;
    sun_light->direction.setX(0.0f); sun_light->direction.setY(0.0f); sun_light->direction.setZ(0.0f);
    sun_light->position.setX(-3.0f); sun_light->position.setY(5.0f); sun_light->position.setZ(3.0f);
    sun_light->intensity = 2.0;

    spot_light = new Lights;
    spot_light->type = 1;
    spot_light->direction.setX(0.0f); spot_light->direction.setY(0.0f); spot_light->direction.setZ(-1.0f);
    spot_light->position.setX(0.0f); spot_light->position.setY(1.0f); spot_light->position.setZ(8.0f);
    spot_light->color[0] = 0.5f; spot_light->color[1] = 1.0f; spot_light->color[2] = 0.1f;
    spot_light->cut_angle = cos(30.0f * PI/180);

}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST); //Profundidad
//    resizeGL(this->width(), this->height());

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
    //unsigned int depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    f->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    //attach a texture image to a framebuffer object
    f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    //no color data
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if(f->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                qDebug() << "framebuffer false";

    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    depthMap_shader_quad->bind();
    depthMap_shader_quad->setUniformValue("depthTexture", 0);



}

void MainWindow::resizeGL(int w, int h)
{

//      glViewport(0,0,w,h); //coordenadas de ventana, viewport
//      qreal aspectRatio = qreal(w)/qreal(h);

// //matriz de proyeccion
//      glMatrixMode(GL_PROJECTION);
//      glLoadIdentity();

//      gluPerspective(100, aspectRatio, 0.1, 400000000);

// //MODEL VIEW
//      glMatrixMode(GL_MODELVIEW);
//      glLoadIdentity();

}

void MainWindow::paintGL()
{
    float ar = (float)this->width()/(float)this->height();
     float near_plane = 1.0f, far_plane = 30.0f;


     glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     QMatrix4x4 proj_light;
     //proj.perspective(45.0f,1, 0.1f, 100.0f);
     QMatrix4x4 view_light;
     QMatrix4x4 lightSpaceMatrix;

     proj_light.ortho(-10.0f*ar, 10.0f*ar, -10.0f, 10.0f, near_plane, far_plane);
     //proj_light.perspective(90.0f, ar  , 0.1f, 100.0f);
     //view_light.lookAt(spot_light->position, spot_light->direction, camUp);

     view_light.lookAt(sun_light->position, sun_light->direction, camUp);


     lightSpaceMatrix = proj_light * view_light;
    //depthMap_shader->bind();
    //depthMap_shader->setUniformValue("lightSpaceMatrix", lightSpaceMatrix );

 //RENDER SHADOW MAP
    glViewport(0,0, 1024 , 1024);
    f->glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, woodTexture);
    
    depthMap_shader->bind();
    int model_loc = depthMap_shader->uniformLocation("model");
    depthMap_shader->setUniformValue("proj", proj_light);
    depthMap_shader->setUniformValue("view", view_light);

    depthMap_shader->setUniformValue(model_loc, bunnyObject->model);
    bunnyObject->render(depthMap_shader);


    depthMap_shader->setUniformValue(model_loc, box_obj->model);
    box_obj->render(depthMap_shader);

    depthMap_shader->setUniformValue(model_loc, wall_obj->model);
    wall_obj->render(depthMap_shader);


    depthMap_shader->setUniformValue(model_loc, wall_obj->model);
    wall_obj->render(depthMap_shader);

    depthMap_shader->setUniformValue(model_loc, floor_obj->model);
    floor_obj->render(depthMap_shader);



//regresar buffer original
    f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ////RENDERING CALLBACK
        glViewport(0,0, this->width() , this->height());
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//CAMERAS

        //QMatrix4x4 proj;
        //proj.perspective(45.0f,1, 0.1f, 100.0f);

        QVector3D direction;
        direction.setX( cos(yaw* PI/180) * cos(pitch* PI/180) );
        direction.setY( sin(pitch* PI/180) );
        direction.setZ( sin(yaw *PI/180) * cos(pitch* PI/180)   );
        direction.normalize();

        QMatrix4x4 proj;
        QMatrix4x4 view;

        proj.ortho(-10.0f*ar, 10.0f*ar, -10.0f, 10.0f, near_plane, far_plane);
        //proj.frustum(-1.0f*ar, 1.0f*ar, -1.0f, 1.0f, near_plane, far_plane);
        //proj.perspective(90.0f, ar  , 0.1f, 100.0f);
        //view.lookAt(sun_light->position, sun_light->direction, camUp);
        view.lookAt(camPos, camFront, camUp);

        program->setUniformValue("proj_light", proj_light);
        program->setUniformValue("view_light", view_light);


//RENDER

        bunnyObject->curr_mat=curr_mat;

        program->bind();
//        program->setUniformValue("shadowMap", 1);
        int shadowMapLoc= program->uniformLocation("shadowMap");
         program->setUniformValue( shadowMapLoc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTexture);

        setShaderValues(bunnyObject, proj, view);
        bunnyObject->render(program);

        box_obj->curr_mat = curr_mat;
        setShaderValues(box_obj, proj, view);
        box_obj->render(program);

        wall_obj->curr_mat = curr_mat;
        setShaderValues(wall_obj, proj, view);
        wall_obj->render(program);


        setShaderValues(floor_obj, proj, view);
        floor_obj->render(program);


////quad
        glViewport(0,0, 240 , 194);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        depthMap_shader_quad->bind();
        depthMap_shader_quad->setUniformValue("near_plane", near_plane);
        depthMap_shader_quad->setUniformValue("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthTexture);

        depthMap_shader_quad->setUniformValue("depthTexture", 0);

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

    /// TEXTURE PARAMETERS
           QImage texture_img = object->all_mat[curr_mat].texture;

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

          // program->setUniformValue("textImage", 0);

//           int shadowMapLoc= program->uniformLocation("shadowMap");
//            program->setUniformValue( shadowMapLoc, 0);


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

           program->setUniformValue(lightColorLoc, spot_light->color[0], spot_light->color[1], spot_light->color[2] );
           program->setUniformValue(lightPosLoc, spot_light->position);
           program->setUniformValue("spotlightDir", spot_light->direction);
           program->setUniformValue("light.intensity", spot_light->intensity);

           program->setUniformValue("light.attFactor_k", 1.0f);
           program->setUniformValue("light.attFactor_l", 0.22f);
           program->setUniformValue("light.attFactor_q", 0.20f);
           program->setUniformValue("cut_spot", spot_light->cut_angle);


           /// LIGHT 2
           int lightPos2Loc = program->uniformLocation("light2.pos");
           int lightColor2Loc = program->uniformLocation("light2.color");

           program->setUniformValue(lightColor2Loc, 0.9f, 0.9f, 0.8f);
           program->setUniformValue(lightPos2Loc, 0.0f, 2.0f, 2.0f);

           program->setUniformValue("light2.intensity", 0.0f);

           program->setUniformValue("light2.attFactor_k", 1.0f);
           program->setUniformValue("light2.attFactor_l", 0.35f);
           program->setUniformValue("light2.attFactor_q", 0.44f);



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



//           glActiveTexture(GL_TEXTURE0);
//           glBindTexture(GL_TEXTURE_2D, t1);
//           glActiveTexture(GL_TEXTURE0);
//           glBindTexture(GL_TEXTURE_2D, depthTexture);

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
















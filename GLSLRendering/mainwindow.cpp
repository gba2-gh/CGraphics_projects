#include "mainwindow.h"
#include<QImage>
#include <QtGui/QImage>
#include"object.h"
#include"functions.h"



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
    bunnyObject->curr_mat=1;

    std::string path ("../GLSLRendering/object_file/bunny.obj");
    importFile(path, &bunnyObject->vertices,  &bunnyObject->facesIdx,
                            &bunnyObject->vertexNormals, &bunnyObject->uvCoord);

    bunnyObject->model.translate(3.0f,0.0f,0.0f);

    //COPY
    bunnyObjectCopy = new Object;
    bunnyObjectCopy->curr_mat=1;

    std::string path2 ("../GLSLRendering/object_file/bunny.obj");
    importFile(path2, &bunnyObjectCopy->vertices,  &bunnyObjectCopy->facesIdx,
                            &bunnyObjectCopy->vertexNormals, &bunnyObjectCopy->uvCoord);





    qDebug() << "Imported";

    //bunnyObject->calcVerticesNormal();







}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST); //Profundidad
    resizeGL(this->width(), this->height());

    program = new QOpenGLShaderProgram();

    program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../GLSLRendering/shaders/shader.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../GLSLRendering/shaders/shader.frag");

   // program->link();



    shader= new QOpenGLShaderProgram();

    shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../GLSLRendering/shaders/test.vert");
    shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../GLSLRendering/shaders/test.frag");

   // shader->link();



//    float vertices[this->bunnyObject->vertices.size() *
//                            this->bunnyObject->vertices[0].size()*2];

//    GLuint indices[this->bunnyObject->facesIdx.size() *
//                            this->bunnyObject->facesIdx[0].size()];

//    int totalV= bunnyObject->vertices.size();
//    int sizeV = bunnyObject->vertices[0].size();
//    for(int i=0; i<totalV; ++i ){
//        for(int j=0; j<sizeV; ++j){
//            vertices[i*(sizeV*2)+j] = bunnyObject->vertices[i][j];
//            vertices[i*(sizeV*2)+j+3] = bunnyObject->vertexNormals[i][j];
//        }
//    }

//    for(int i=0; i<bunnyObject->facesIdx.size(); ++i ){
//        for(int j=0; j<bunnyObject->facesIdx[0].size(); ++j){
//            indices[i*(bunnyObject->facesIdx[0].size())+j] = bunnyObject->facesIdx[i][j];
//        }
//    }


        f = QOpenGLContext::currentContext()->extraFunctions();

//        f->glGenVertexArrays(1, &VAO);
//        f->glGenBuffers(1, &VBO);
//        f->glGenBuffers(1, &EBO);

//        //bind
//        f->glBindVertexArray(VAO);

//        f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//         f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//         f->glEnableVertexAttribArray(0);

//        f->glBindBuffer(GL_ARRAY_BUFFER, 0);
//         f->glBindVertexArray(0);



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
        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bunnyObject->render( context, program);
        bunnyObjectCopy->render(context, shader);








//        QMatrix4x4 proj;
//        //proj.ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
//        proj.perspective(45.0f,1, 0.1f, 100.0f);

//        QMatrix4x4 view;
//        if(camSelect==0){
//            view.translate(0.0f, -1.0f, -8.0f);
//        }else if(camSelect==1){
//            view.translate(0.0f, 0.0f, -8.0f);
//            view.rotate(180, 0,1,0 );
//        }else {
//            view.translate(0.0f, 0.0f, -8.0f);
//            view.rotate(45, 1,1,0);
//        }


//        QMatrix4x4 model;
//        //model.translate(0.0f, 0.0f, 0.0f);
//        //model.rotate(45, 0,1, 0);

//        QMatrix4x4 normalMat;
//        normalMat = model.transposed();
//        normalMat = normalMat.inverted();

//    program->release();
//    shader->release();


 //   if(shaderPhong){
        //shader->release();

//        int projLocation = program->uniformLocation("proj");
//        int viewLocation = program->uniformLocation("view");
//        int modelLocation = program->uniformLocation("model");

//        int lightPosLoc = program->uniformLocation("lightPos");
//        int lightColorLoc = program->uniformLocation("lightColor");
//        int lightPos2Loc = program->uniformLocation("lightPos2");
//        int lightColor2Loc = program->uniformLocation("lightColor2");

//        program->setUniformValue(projLocation, proj);
//        program->setUniformValue(viewLocation, view);
//        program->setUniformValue(modelLocation, model);
//        program->setUniformValue("normalMat", normalMat);


//        program->setUniformValue(lightColorLoc, 1.0f, 1.0f, 1.0f);
//        program->setUniformValue(lightPosLoc, 2.0f, 2.0f, 2.0f);
//        program->setUniformValue("lightIntensity", 1.0f);

//        program->setUniformValue(lightColor2Loc, 0.0f, 0.1f, 0.8f);
//        program->setUniformValue(lightPos2Loc, 2.0f, -2.0f, 2.0f);


//        if(luzOnOff){
//        program->setUniformValue("lightIntensity2", 1.0f);}
//        else{
//            program->setUniformValue("lightIntensity2", 0.0f);
//        }



//        program->setUniformValue("ka", bunnyObject->all_mat[curr_mat].ka[0],
//                                        bunnyObject->all_mat[curr_mat].ka[1],
//                                            bunnyObject->all_mat[curr_mat].ka[2]);
//        program->setUniformValue("kd", bunnyObject->all_mat[curr_mat].kd[0],
//                                        bunnyObject->all_mat[curr_mat].kd[1],
//                                            bunnyObject->all_mat[curr_mat].kd[2]);
//        program->setUniformValue("ke", bunnyObject->all_mat[curr_mat].ke[0],
//                                            bunnyObject->all_mat[curr_mat].ke[1],
//                                                 bunnyObject->all_mat[curr_mat].ke[2]);
//        program->setUniformValue("shininess", bunnyObject->all_mat[curr_mat].ro);

//        program->setUniformValue("eyePos", 1.0f, 0.0f, 1.0f);
//        program->setUniformValue("shaderPhong", shaderPhong);

//        program->bind();

//     f->glBindVertexArray(VAO);
//     //glDrawElements(GL_TRIANGLES, bunnyObject->facesIdx.size()*bunnyObject->facesIdx[0].size(), GL_UNSIGNED_INT, 0);

//     program->release();


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

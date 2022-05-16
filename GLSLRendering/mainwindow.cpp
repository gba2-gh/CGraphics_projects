#include "mainwindow.h"
#include<QImage>
#include <QtGui/QImage>
#include"cubeobject.h"
#include"functions.h"



MainWindow::MainWindow(QWidget *parent)
{
    setSurfaceType(QWindow::OpenGLSurface); //usar opengl para renderizar la ventana

    QSurfaceFormat format;

    format.setProfile(QSurfaceFormat::CompatibilityProfile); //fixed function pipelne
    format.setVersion(4,1); //version opengl
    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();
    context->makeCurrent(this);

    //openGLFunctions = context->functions();


    cubeObject = new CubeObject;
    cubeObject->curr_mat=1;

//IMPORT
    std::string path ("../OpenGLRendering/object_file/bunny3.obj");

    importFile(path, &cubeObject->vertices,  &cubeObject->facesIdx,
                            &cubeObject->vertexNormals, &cubeObject->uvCoord);
    qDebug() << "Imported";

    //cubeObject->calcVerticesNormal();

    rotation=0;

}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST); //Profundidad
    resizeGL(this->width(), this->height());

    program = new QOpenGLShaderProgram();
    program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                     "#version 450\n"
                                     "in vec3 position;\n"
                                     "out vec4 fragColor;\n"
                                     "void main() {\n"
                                     "    fragColor = vec4(0.3, 0.2, 0.75, 1.0);\n"
                                     "    gl_Position = vec4(position, 1.0);\n"
                                     "}");
    program->addSHaderFromSourceCode(QOpenGLShader::Fragment,
                                     "#version 450\n"
                                     "in vec4 fragColor;\n"
                                     "out vec4 finalColor;\n"
                                     "void main(){\n"
                                     "    finalColor = fragColor;\n"
                                     "}");

    program->link();

    float vertices[] = {
         0.0f,  0.8f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    vao = new QOpengl
    vbo = new QOpenGLBuffer;

    vao.create();
    vao.bind();


}

void MainWindow::resizeGL(int w, int h)
{
    //Proyecccion ortográfica
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glViewport(0,0,w,h);
//    qreal aspectRatio = qreal(w)/qreal(h);
//    glOrtho(-1 * aspectRatio, 1* aspectRatio, -1, 1, 1, -1);


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
    //glClearColor(1.0, 0.0f, 0.0f, 0.0f);

//RENDERING CALLBACK
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//reset model view matrix
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();  //identidad matriz actual


// CÁMARAS
    double es=0.8;
    if(camSelect==0){
        gluLookAt(-1,1 , 5*es, 0, 0, -1, 0,1,0);
    }else if(camSelect==1){
        gluLookAt(4*es,2*es , 4*es, 0, 0, -1, 0,1,0);
    }else if(camSelect==2){
        gluLookAt(-4*es,2*es , -4*es, 0, 0, -1, 0,1,0);
    }else{
         gluLookAt(0,2, -5*es, 0, 0, 1, 0,1,0);
    }



// MATERIAL

    GLfloat kd[3];
    GLfloat ka[3];
    GLfloat ke[3];
    GLfloat shininess = 0;

    for (int k =0; k< 3; ++k){
        kd[k] = cubeObject->all_mat[curr_mat].kd[k];
        ka[k] = cubeObject->all_mat[curr_mat].ka[k];
        ke[k] = cubeObject->all_mat[curr_mat].ke[k];
        shininess = cubeObject->all_mat[curr_mat].ro;
    }

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ka);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ke);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

// LUZ
    //GLfloat al[] = {0.8, 0.8, 0.8, 1.0};
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, al);


//LUZ 1
    GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat light_position[] = {2.0, 2.0, 2.0, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    GLfloat light_ambient[] = {0.8, 0.8, 0.8, 1.0};
    //glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    if(luzOnOffB){
        glEnable(GL_LIGHT0);
    }else{
        glDisable(GL_LIGHT0);
    }

//LUZ 2
    GLfloat light_diffuse2[] = {0.1, 0.1, 0.9, 1.0};
    GLfloat light_position2[] = {-2.0, -2.0, 2.0, 1.0};
    GLfloat light_specular2[] = {0.0, 0.0, 1.0, 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);

    //glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);


    if(luzOnOff){
        glEnable(GL_LIGHT1);
    }else{
        glDisable(GL_LIGHT1);
    }

 //Transformacion
    glTranslatef(0.0,0.0,0);
    glRotatef(rotation, 1.0, 1.0, 1.0);


//IMAGEN TEXTURA

//    QImage image(400, 400, QImage::Format_RGB32);
//    QRgb value = qRgb(189, 149, 39); ;

//    for(int i=0; i<400; ++i){
//        for(int j=0; j<400; ++j){
//            image.setPixel(i,j, value);
//        }
//    }
    //texture_img = image;

    texture_img = cubeObject->all_mat[curr_mat].texture;

    texture_img=texture_img.convertToFormat(QImage::Format_RGB888);
    uint d;
    glGenTextures(1, &d); // generate a unique ID for this texture
    glBindTexture(GL_TEXTURE_2D, d); // create texture d

    // interpolacion, wrapping
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_img.height(), texture_img.width(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture_img.bits());
     //target, level, internalFormat, width, height, border, format, type, pointer to texels

    glEnable(GL_TEXTURE_2D);

    // Método 2
        GLfloat vert[3];
        GLfloat n[3];
        GLfloat uv[2];
        glBegin(GL_TRIANGLES);
            for(int i=0; i< cubeObject->facesIdx.size(); ++i){ // Each new triangle starts here
                for(int k = 0; k < 3; k++){
                    for(int j = 0; j < 2; j++){ // Each vertex specified here
                         uv[j] = cubeObject->uvCoord[cubeObject->facesIdx[i][k]][j];
                     }
                    glTexCoord2fv(uv);

                    for(int j = 0; j < 3; j++){ // Each vertex specified here
                        n[j] = cubeObject->vertexNormals[cubeObject->facesIdx[i][k]][j];
                        vert[j] = cubeObject->vertices[cubeObject->facesIdx[i][k]][j];
                    }
                     glNormal3fv(n);
                     glVertex3fv(vert);

                }
            }
        glEnd();




//Metodo1

//    int v_size = cubeObject->vertices.size() * cubeObject->vertices[0].size();
//    int n_size = cubeObject->vertexNormals.size() * cubeObject->vertexNormals[0].size();
//    int uv_size = cubeObject->uvCoord.size() * cubeObject->uvCoord[0].size();
//    int i_size = cubeObject->facesIdx.size() * cubeObject->facesIdx[0].size();

//    GLfloat *vertices = (GLfloat*) malloc(v_size * sizeof(GLfloat));
//    GLfloat *normals = (GLfloat*) malloc(n_size * sizeof(GLfloat));
//    GLfloat *uvCoord = (GLfloat*) malloc(uv_size * sizeof(GLfloat));
//    GLuint *indexes = (GLuint*) malloc(i_size * sizeof(GLuint));

//    GLfloat vertices[v_size];
//    GLuint indexes[i_size];
//    GLfloat uvCoord[uv_size];
//    GLfloat normals[n_size];

//    int id=0;

//    for(int i=0; i<cubeObject->vertices.size(); ++i){
//        for(int j=0; j<cubeObject->vertices[0].size(); ++j){
//            vertices[id] = cubeObject->vertices[i][j];
//            id+=1;
//        }
//    }
//    id=0;
//    for(int i=0; i<cubeObject->vertexNormals.size(); ++i){
//        for(int j=0; j<cubeObject->vertexNormals[0].size(); ++j){
//            normals[id] = cubeObject->vertexNormals[i][j];
//            id+=1;
//        }
//    }
//    id=0;
//    for(int i=0; i<cubeObject->facesIdx.size(); ++i){
//        for(int j=0; j<cubeObject->facesIdx[0].size(); ++j){
//            indexes[id] = cubeObject->facesIdx[i][j];
//            id+=1;
//        }
//    }

//    id=0;
//    for(int i=0; i<cubeObject->uvCoord.size(); ++i){
//        for(int j=0; j<cubeObject->uvCoord[0].size(); ++j){
//            uvCoord[id] = cubeObject->uvCoord[i][j];
//            id+=1;
//        }
//    }

//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_FLOAT, 0, vertices);
//    glNormalPointer(GL_FLOAT, 0, normals);
//    glTexCoordPointer(2, GL_FLOAT, 0, uvCoord);
//    glDrawElements(GL_TRIANGLES, i_size, GL_UNSIGNED_INT, indexes);

//    free(vertices);
//    free(indexes);
//    free(normals);
//    free(uvCoord);





//    unsigned int EBO;
//    glGenBuffers(1, &EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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


    if(event->key()==Qt::Key_6){
        luzOnOff= !luzOnOff;
        qDebug() << "LUZ";
    }

//    if(event->key()==Qt::Key_7){
//        luzOnOffB= !luzOnOffB;
//        qDebug() << "LUZB";
//    }

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


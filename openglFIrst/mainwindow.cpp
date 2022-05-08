#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
{
    setSurfaceType(QWindow::OpenGLSurface); //usar opengl para renderizar la ventana

    QSurfaceFormat format;

    format.setProfile(QSurfaceFormat::CompatibilityProfile); //fixed function pipelne
    format.setVersion(2,1); //version opengl
    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();
    context->makeCurrent(this);

    openGLFunctions = context->functions();

    QTimer *timer = new QTimer(this);

//    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateAnimation()) );
//    timer->start(100);

    rotation=0;

}

MainWindow::~MainWindow()
{
}

void MainWindow::initializeGL()
{
    glEnable(GL_DEPTH_TEST); //Profundidad
    resizeGL(this->width(), this->height());

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
   glLoadIdentity();           //identidad matriz actual


    if(camSelect==0){
        gluLookAt(0,0 , -50, 0, 0, -1, 0,1,0);
    }else if(camSelect==1){
        //gluLookAt(0,0 , 50, 0, 0, 1, 0,1,0);
        gluLookAt(40,20 , 40, 0, 0, -1, 0,1,0);
    }else{
        gluLookAt(-40,20 , -40, 0, 0, -1, 0,1,0);
    }

    GLfloat kd[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat ka[] = {0.8, 0.8, 0.8, 1.0};
    GLfloat al[] = {0.8, 0.8, 0.8, 1.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ka);

    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, al);

//    GLfloat light_ambient[] = {0.0, 0.1, 0.0, 1.0};
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    //LUZ 1
    GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_position[] = {30, 30, 30, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);

    //LUZ 2

    GLfloat light_diffuse2[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat light_position2[] = {-30.0, 30.0, 30.0, 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

    if(luzOnOff){
        glEnable(GL_LIGHT1);
    }else{
        glDisable(GL_LIGHT1);
    }


    //Transformacion cubo

    glTranslatef(0.0,0.0,0);
    glRotatef(rotation, 1.0, 1.0, 1.0);


    ///CUBE
    //FRENTE
        glBegin(GL_POLYGON);
            glColor3f(1.0, 1.0, 1.0);

                glNormal3f(0.7071067811, -0.7071067811, 0);
            glVertex3f(15, -15, 15); //1
                glNormal3f(0.7071067811, -0.7071067811, 0);
            glVertex3f(15, 15, 15); //2
                glNormal3f(-0.7071067811, -0.7071067811, 0);
            glVertex3f(-15, 15, 15); //3
                glNormal3f(-0.7071067811, -0.7071067811, 0);
            glVertex3f(-15, -15, 15); //4
        glEnd();
    //BACK
        glBegin(GL_POLYGON);

            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(15, -15, -15); //5
            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(15, 15, -15); //6
            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, 15, -15); //7
            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, -15, -15); //8
        glEnd();

     //DERECHA
        glBegin(GL_POLYGON);
            glColor3f(1.0, 1.0, 1.0);

            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(15, -15, 15); //1
            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(15, 15, 15); //2
            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(15, 15, -15); //6
            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(15, -15, -15); //5
        glEnd();

     //IZQUIERDA
        glBegin(GL_POLYGON);
            glColor3f(1.0, 1.0, 1.0);
            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, 15, 15); //3
            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, -15, 15); //4
            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, -15, -15); //8
            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, 15, -15); //7


        glEnd();

     //ARRIBA
        glBegin(GL_POLYGON);
            glColor3f(1.0, 1.0, 1.0);

            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(15, 15, 15); //2
            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, 15, 15); //3
            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, 15, -15); //7
            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(15, 15, -15); //6

        glEnd();

      //ABAJO
        glBegin(GL_POLYGON);
            glColor3f(1.0, 1.0, 1.0);

            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(15, -15, 15); //1
            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, -15, 15); //4
            glNormal3f(0.7071067811, -0.7071067811, 0);
        glVertex3f(-15, -15, -15); //8
            glNormal3f(-0.7071067811, -0.7071067811, 0);
        glVertex3f(15, -15, -15); //5

        glEnd();


     glFlush();



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

void MainWindow::UpdateAnimation()
{
 rotation += 10 ;
 this->update();

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

    if(event->key()==Qt::Key_6){

        luzOnOff= !luzOnOff;
        qDebug() << "LUZ";
    }

   this->update();
}






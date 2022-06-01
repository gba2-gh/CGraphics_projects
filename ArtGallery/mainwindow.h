#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>>
#include"object.h"
#include "QOpenGLExtraFunctions"

#include <QOpenGLFunctions_4_5_Core>

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //QMatrix4x4 view;
    QVector3D camPos;
    QVector3D camFront;
    QVector3D camUp;
    float yaw =0;
    float pitch=0;

protected:
    //FUNCIONES OPENGLWINDOW interaccion de ventanas
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent * event);


public slots:


private:
    QOpenGLContext *context;
    QOpenGLExtraFunctions *f;

    //QOpenGLFunctions *f;
    //QOpenGLFunctions_4_5_Core *f;

    QOpenGLBuffer *ibo;
    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;
    QOpenGLShaderProgram *program;
    QOpenGLShaderProgram *shader;
    unsigned int VBO, VAO, EBO;

    float rotation =0;
    float camSelect = 0;
    bool luzOnOff=true;
    bool luzOnOffB=true;
    int curr_mat= 0;

    bool shaderPhong = true;

    Object *bunnyObject;
    Object *box_obj;
    Object *wall_obj;
    QImage texture_img;




};
#endif // MAINWINDOW_H

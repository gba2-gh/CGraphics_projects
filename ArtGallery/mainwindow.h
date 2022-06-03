#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>>
#include"object.h"
#include"lights.h"

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
    Lights *sun_light;
    Lights *spot_light;

    unsigned int depthTexture;
    unsigned int frameBuffer ;


protected:
    //FUNCIONES OPENGLWINDOW interaccion de ventanas
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent * event);

    void setShaderValues(Object *object, QMatrix4x4 proj, QMatrix4x4 view);

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
    QOpenGLShaderProgram *depthMap_shader;
    QOpenGLShaderProgram *depthMap_shader_quad;

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
    Object *floor_obj;


    QImage texture_img;




};
#endif // MAINWINDOW_H

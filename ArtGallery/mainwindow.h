#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>>
#include"object.h"
#include"light.h"

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

    std::vector<Light *> all_lights;
    Light *sun_light;
    Light *spotLight_1_1;
    Light *spotLight_1_2;
    Light *spotLight_2_1;
    Light *spotLight_2_2;

    unsigned int depthMap1;
    unsigned int depthMap2;
    unsigned int frameBuffer ;

    unsigned int skyCubeTexture;


protected:
    //FUNCIONES OPENGLWINDOW interaccion de ventanas
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent * event);

    void renderScene();
    void renderShadowMapDebug();

    void setShaderValues(Object *object, QMatrix4x4 proj, QMatrix4x4 view);
    void setScene();
    void setLights();
    void genDepthMap();
    void renderEnviroment();
    void setCubeTexture();
    void compileShaders();

public slots:


private:
    QTimer *timer;
    QOpenGLContext *context;
    QOpenGLExtraFunctions *f;

    //QOpenGLFunctions *f;
    //QOpenGLFunctions_4_5_Core *f;

    QOpenGLBuffer *ibo;
    QOpenGLBuffer *vbo;
    QOpenGLVertexArrayObject *vao;
    QOpenGLShaderProgram *program;
    QOpenGLShaderProgram *cubemapEnv_shader;
    QOpenGLShaderProgram *depthMap_shader;
    QOpenGLShaderProgram *depthMap_shader_quad;
    QOpenGLShaderProgram *reflection_shader;


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

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
///OBJECTS
///
    std::vector<Object *> all_objects;
    Object *bunnyObject;
    Object *venusObj;
    Object *scholarObj;
    Object *nude;
    Object *roosterObj;
    Object *cuadro;
    Object *vivi;
    Object *estela;


    Object *boxObj_1;
    Object *boxObj_2;
    Object *boxObj_3;
    Object *boxObj_4;
    Object *boxObj_5;
    Object *boxObj_6;
    Object *boxObj_7;

    Object *wallObj_1;
    Object *wallObj_2;

    Object *floorObj;
    Object *ceilObj;

///LIGHTS
    std::vector<Light *> all_lights;
    Light *sun_light;
    Light *spotLight_1_1;
    Light *spotLight_1_2;
    Light *spotLight_2_1;
    Light *spotLight_2_2;
    Light *spotLight_3_1;
    Light *spotLight_3_2;
    Light *spotLight_4_1;
    Light *spotLight_4_2;
    Light *spotLight_5_1;
    Light *spotLight_5_2;
    Light *spotLight_6_1;
    Light *spotLight_6_2;
    Light *spotLight_7_1;
    Light *spotLight_7_2;
    Light *spotLight_8_1;
    Light *spotLight_8_2;

    unsigned int woodTexture;
    unsigned int fabricTexture;
    unsigned int roosterTexture;
    unsigned int scholarTexture;

    unsigned int depthMap1;
    unsigned int depthMap2;
    unsigned int frameBuffer ;

    unsigned int skyCubeTexture;
    unsigned int t1;



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

    void setShaderValues(Object *object);
    void setScene();
    void setLights();
    void genDepthMap();
    void renderEnviroment();
    void setCubeTexture();
    void compileShaders();
    void setMovementShader(Object *object);
    void setToonShader(Object *object);

    void setTextures(QString path);

    float movement_time=0.0;

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
    QOpenGLShaderProgram *phong;
    QOpenGLShaderProgram *phong_texture;
    QOpenGLShaderProgram *cubemapEnv_shader;
    QOpenGLShaderProgram *depthMap_shader;
    QOpenGLShaderProgram *depthMap_shader_quad;
    QOpenGLShaderProgram *reflection_shader;
    QOpenGLShaderProgram *movement_shader;
    QOpenGLShaderProgram *toon_shader;


    unsigned int VBO, VAO, EBO;

    float rotation =0;
    float camSelect = 0;
    bool luzOnOff=true;
    bool luzOnOffB=true;
    int curr_mat= 0;

    bool shaderPhong = true;
    bool toon =true;
    bool movement=true;
    bool reflec=true;

    QImage texture_img;




};
#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>>
#include"cubeobject.h"

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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
    QOpenGLFunctions *openGLFunctions;
    float rotation =0;
    float camSelect = 0;
    bool luzOnOff=true;
    bool luzOnOffB=true;
    int curr_mat= 0;

    CubeObject *cubeObject;
    QImage texture_img;


};
#endif // MAINWINDOW_H

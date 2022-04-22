#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<QPainter>
#include<QApplication>
#include<QtWidgets>
#include"renderWindow.h"
#include"camProjection.h"
#include"cubeobject.h"
#include"lights.h"
#include"raster.h"
#include<qaction.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class renderWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    std::vector<lights*> lightScene;
    CubeObject *cubeObject;
    QAction    *action;
public slots:

    void pickColor();
    void setOrtho();
    void setPers();
    void setRotation();
    void drawObject();
    void setSwitchCamera();
    void setLight1On();
    void setLight2On();
    void setLight3On();
    void setGouShade();
    void setPhongShade();
    void setMaterial1();
    void setMaterial2();
    void setCamera1();


private:
    Ui::MainWindow *ui;
    QPushButton *colorBtn;
    QPushButton *orthoProyBtn;
    QPushButton *persProyBtn;
    QPushButton *setRotationBtn;
    QPushButton *fillPolyBtn;
    QPushButton *drawEdgesBtn;
    QPushButton *switchCamera;
    QPushButton *light1On;
    QPushButton *light2On;
    QPushButton *light3On;
    QPushButton *gouraud;
    QPushButton *phong;
    QPushButton *material1;
     QPushButton *material2;

     void keyPressEvent(QKeyEvent * event);



    renderWindow *renderwindow;

    ambientLight *ambLight;
    lights *lightWhite;
    lights *light2;
    lights   *lightSpec;
    raster *raster1;

    QTimer *timer ;
    bool orthoProy= false;
    bool rotateBool=false;
    bool l1=true;
    bool l2=true;
    bool l3=true;
    bool phongBool=false;

    int camSelect=0;
    int shaderSel;

};
#endif // MAINWINDOW_H

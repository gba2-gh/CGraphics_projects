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

    std::vector<lights> lightScene;

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


    renderWindow *renderwindow;
    CamProjection *camProj;
    CamProjection *camProj2;
    CubeObject *cubeObject;
    ambientLight *ambLight;
    lights lightWhite;
    lights lightRed;
    lights   lightSpec;

    QTimer *timer ;
    bool orthoProy= false;
    bool rotateBool=false;
    bool cam1Bool=true;
    bool l1=true;
    bool l2=true;
    bool l3=true;
    bool phongBool=false;

};
#endif // MAINWINDOW_H

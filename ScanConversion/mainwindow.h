#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<QPainter>
#include<QApplication>
#include<QtWidgets>
#include"renderWindow.h"
#include"camProjection.h"
#include"cubeobject.h"


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

public slots:
    void pickColor();
    void setOrtho();
    void setPers();
    void setRotation();
    void drawObject();


private:
    Ui::MainWindow *ui;
    QPushButton *colorBtn;
    QPushButton *orthoProyBtn;
    QPushButton *persProyBtn;
    QPushButton *setRotationBtn;
    renderWindow *renderwindow;
    CamProjection *camProj;
    CamProjection *camProj2;
    CubeObject *cubeObject;
    QTimer *timer ;
    bool orthoProy= false;
    bool rotateBool=false;
};
#endif // MAINWINDOW_H

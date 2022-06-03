#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QSurfaceFormat>
#include <QCommandLineParser>
#include <QCommandLineOption>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(960, 496);
    //w.resize(800, 600);

    w.show();
    return a.exec();

}

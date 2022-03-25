#include "renderWindow.h"
#include <vector>
#include <QDebug>
#include <cmath>

renderWindow::renderWindow(QWidget *parent):
    QWidget(parent){

//    for(int i=0; i<400; i++){
//        for(int j=0; j<400; j++){
//            depthBuffer[i][j] = 0;
//        }
//    }
    //pointPen.setColor(Qt::white);
    //pointsList.append(200);
    //pointsList.append(200);
}

void renderWindow::setPenColor(QColor color)
{
    this->pointPen.setColor(color);
    update();
}


void renderWindow::paintEvent(QPaintEvent *event)
{
    //Fondo
    QPainter painter(this);
    QBrush bgColor(Qt::black);
    QRect bg(0,0,400,400);
    painter.fillRect(bg, bgColor);

    QPainter points(this);


    for(int k=0; k<pointsList.size();k=k+1) {//fork

        pointPen.setColor(QColor(pixelColor[0][k], pixelColor[1][k], pixelColor[2][k],255));

        points.setPen(pointPen);

        if (pointsZ[k] > depthBuffer[pointsList[k][0]][pointsList[k][1]]){

            depthBuffer[pointsList[k][0]][pointsList[k][1]] = pointsZ[k];
            points.drawPoint(pointsList[k][0],pointsList[k][1]);

        }

//        qDebug() << k;
//        qDebug() << "coord"<< pointsList[k][0]<<pointsList[k][1];
//        qDebug()<< "color"<< pixelColor[0][k] <<pixelColor[1][k]<<pixelColor[2][k];

}

    pointsList.clear();
    pointsZ.clear();

    pixelColor[0].clear();
    pixelColor[1].clear();
    pixelColor[2].clear();

    for(int i=0; i<400; i++){
        for(int j=0; j<400; j++){
            depthBuffer[i][j] = 0;
        }
    }


}



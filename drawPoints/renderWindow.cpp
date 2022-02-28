#include "renderWindow.h"

renderWindow::renderWindow(QWidget *parent):
    QWidget(parent){

    pointPen.setColor(Qt::white);
    pointsList.append(100);
    pointsList.append(100);
}

void renderWindow::setPenColor(QColor color)
{
    //this->pointPen = pen;
    this->pointPen.setColor(color);
    update();
}


void renderWindow::paintEvent(QPaintEvent *event)
{
    //Fondo
    QPainter painter(this);
    QBrush bgColor(Qt::black);
    QRect bg(0,0,200,200);
    painter.fillRect(bg, bgColor);

    QPainter points(this);
    //QPolygon polygon;

    points.setPen(pointPen);

    for(int k=0; k<pointsList.size()-1;k=k+2) {
        points.drawPoint(pointsList[k],pointsList[k+1]);
    }



}





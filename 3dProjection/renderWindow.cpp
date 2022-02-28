#include "renderWindow.h"

renderWindow::renderWindow(QWidget *parent):
    QWidget(parent){

    pointPen.setColor(Qt::white);
    //pointsList.append(200);
    //pointsList.append(200);
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
    QRect bg(0,0,400,400);
    painter.fillRect(bg, bgColor);

    QPainter points(this);
    //QPolygon polygon;

    points.setPen(pointPen);

    for(int k=0; k<pointsList.size()-1;k=k+2) {
        points.drawPoint(pointsList[k],pointsList[k+1]);
    }
    pointsList.clear();


}





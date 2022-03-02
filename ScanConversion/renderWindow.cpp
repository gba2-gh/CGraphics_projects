#include "renderWindow.h"
#include <vector>
#include <QDebug>

renderWindow::renderWindow(QWidget *parent):
    QWidget(parent){

    pointPen.setColor(Qt::white);
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
    //QPolygon polygon;

    points.setPen(pointPen);

    for(int k=0; k<pointsList.size()-1;k=k+2) {//fork
        points.drawPoint(pointsList[k],pointsList[k+1]);
}

    drawCubeEdges(points, pointsList);
    pointsList.clear();


}





void renderWindow::drawCubeEdges(QPainter &points, QList<int> pointsList)
{

    /////////////////////////////////// CUBO 1 ///////////////////////


        lineDDA(points,pointsList.value(0), pointsList.value(1),
                 pointsList.value(14), pointsList.value(15));

        lineDDA(points,pointsList.value(2), pointsList.value(3),
                 pointsList.value(12), pointsList.value(13));

        lineDDA(points,pointsList.value(4), pointsList.value(5),
                 pointsList.value(10), pointsList.value(11));

       lineDDA(points,pointsList.value(6), pointsList.value(7),
                pointsList.value(8), pointsList.value(9));


       lineDDA(points,pointsList.value(6), pointsList.value(7),
                pointsList.value(0), pointsList.value(1));


    //    lineDDA(points,pointsList.value(0), pointsList.value(1),
    //             pointsList.value(2), pointsList.value(3));
    //    lineDDA(points,pointsList.value(2), pointsList.value(3),
    //             pointsList.value(4), pointsList.value(5));
    //    lineDDA(points,pointsList.value(4), pointsList.value(5),
    //             pointsList.value(6), pointsList.value(7));
           for(int k=0; k<6;k=k+2) {
               lineDDA(points,pointsList.value(k), pointsList.value(k+1),
                       pointsList.value(k+2), pointsList.value(k+3));
               //points.drawPoint(pp[0],pp[1]);
           }


       // qDebug()<<"pointlist"<<pointsList;
       lineDDA(points,pointsList.value(14), pointsList.value(15),
                 pointsList.value(8), pointsList.value(9));
       for(int k=8; k<14;k=k+2) {
           lineDDA(points,pointsList.value(k), pointsList.value(k+1),
                   pointsList.value(k+2), pointsList.value(k+3));
           //points.drawPoint(pp[0],pp[1]);
       }
    //   lineDDA(points,pointsList.value(8), pointsList.value(9),
    //            pointsList.value(10), pointsList.value(11));
    //   lineDDA(points,pointsList.value(10), pointsList.value(11),
    //            pointsList.value(12), pointsList.value(13));
    //   lineDDA(points,pointsList.value(12), pointsList.value(13),
    //            pointsList.value(14), pointsList.value(15));




    ///////////////////////////////// CUBO 2 ////////////////////////////77
    ///

       lineDDA(points,pointsList.value(0+16), pointsList.value(1+16),
                pointsList.value(14+16), pointsList.value(15+16));

       lineDDA(points,pointsList.value(2+16), pointsList.value(3+16),
                pointsList.value(12+16), pointsList.value(13+16));

       lineDDA(points,pointsList.value(4+16), pointsList.value(5+16),
                pointsList.value(10+16), pointsList.value(11+16));

      lineDDA(points,pointsList.value(6+16), pointsList.value(7+16),
               pointsList.value(8+16), pointsList.value(9+16));


      lineDDA(points,pointsList.value(6+16), pointsList.value(7+16),
               pointsList.value(0+16), pointsList.value(1+16));


          for(int k=0+16; k<6+16;k=k+2) {
              lineDDA(points,pointsList.value(k), pointsList.value(k+1),
                      pointsList.value(k+2), pointsList.value(k+3));
              //points.drawPoint(pp[0],pp[1]);
          }


      // qDebug()<<"pointlist"<<pointsList;
      lineDDA(points,pointsList.value(14+16), pointsList.value(15+16),
                pointsList.value(8+16), pointsList.value(9+16));
      for(int k=8+16; k<14+16;k=k+2) {
          lineDDA(points,pointsList.value(k), pointsList.value(k+1),
                  pointsList.value(k+2), pointsList.value(k+3));
          //points.drawPoint(pp[0],pp[1]);
      }



}

void renderWindow::lineDDA(QPainter & points, double p1x, double p1y, double p2x, double p2y)
{
    //qDebug()<<"p1 "<<p1x<<p1y<<"p2 "<<p2x<<p2y;
    double mx=p2x-p1x;
    double my=p2y-p1y;

    double s=0;
    if (abs(mx)>abs(my)){
        s = abs(mx);
    }else{
            s=abs(my);
    }

    double dx= mx/s, dy=my/s;
    int x=p1x, y=p1y;


    for(int i=0; i<s; i++){
        x=int(p1x+dx*i);
        y=int(p1y+dy*i);
        //qDebug()<<"linepoint:"<<x<<", "<<y;
        points.drawPoint(x,y);
    }
}





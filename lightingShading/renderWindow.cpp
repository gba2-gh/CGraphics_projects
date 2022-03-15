#include "renderWindow.h"
#include <vector>
#include <QDebug>

renderWindow::renderWindow(QWidget *parent):
    QWidget(parent){

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
    //QPolygon polygon;

//    double Ia= 0.5;
//    double R=Ia*

//    pointPen.setColor(QColor(Ia*255,Ia*255,Ia*255,255));

    points.setPen(pointPen);

    int temp;
    for(int i=0;i<pixelColor[0].size(); ++i){
        for(int j=0; j<=02; j++){
            temp = (int)pixelColor[j][i];
            if (temp >255){
                temp=255;}
            if(temp<=0){
                temp=0;
            }
            pixelColor[j][i] = temp   ;
        }
    }



    for(int k=0; k<pointsList.size();k=k+1) {//fork

        pointPen.setColor(QColor(pixelColor[0][k], pixelColor[1][k], pixelColor[2][k],255));

        //pointPen.setColor(QColor(250, 200, 200,255));


        points.drawPoint(pointsList[k][0],pointsList[k][1]);


        qDebug() << k;
        qDebug() << "coord"<< pointsList[k][0]<<pointsList[k][1];
        qDebug()<< "color"<< pixelColor[0][k] <<pixelColor[1][k]<<pixelColor[2][k];

}



//    if(drawEdgesBool){
//        drawCubeEdges(points, pointsList);
//    }



    pointsList.clear();

//    pixelColor[0].clear();
//    pixelColor[1].clear();
//    pixelColor[2].clear();


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

           for(int k=0; k<6;k=k+2) {
               lineDDA(points,pointsList.value(k), pointsList.value(k+1),
                       pointsList.value(k+2), pointsList.value(k+3));
           }


       lineDDA(points,pointsList.value(14), pointsList.value(15),
                 pointsList.value(8), pointsList.value(9));
       for(int k=8; k<14;k=k+2) {
           lineDDA(points,pointsList.value(k), pointsList.value(k+1),
                   pointsList.value(k+2), pointsList.value(k+3));

       }


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
          }


      lineDDA(points,pointsList.value(14+16), pointsList.value(15+16),
                pointsList.value(8+16), pointsList.value(9+16));
      for(int k=8+16; k<14+16;k=k+2) {
          lineDDA(points,pointsList.value(k), pointsList.value(k+1),
                  pointsList.value(k+2), pointsList.value(k+3));
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





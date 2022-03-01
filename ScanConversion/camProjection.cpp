#include "camProjection.h"
#include<QDebug>

void   CamProjection::projectPoint(double point[], bool ortho,  double xdmax ,double xdmin,
                                                                 double ydmax, double ydmin)
{

    double p[4];

    double suma;

    for(int i=0; i<4; ++i){
        for(int j=0;j<4;++j){
                suma += camMarco[i][j]*point[j];
        }
        p[i]=suma;
        suma=0;
    }


    //Proyectar
    double  pProy[2] ;
    int f=1;
    int canvSc=0;

    if(ortho){//Ortogonal
        pProy[0] = p[0];
        pProy[1] = p[1];

        canvSc=20;
    }
    else{//Perspectiva
        pProy[0] = p[0]*f/-p[2];
        pProy[1] = p[1]*f/-p[2];

        canvSc=1;
    }
    //qDebug()<<pProy[0];
    int xd, yd;
    double xpmax=2*canvSc, xpmin=-2*canvSc;
    double ypmax=2*canvSc, ypmin=-2*canvSc;
    double sx= ((xdmax-xdmin)/(xpmax-xpmin)), sy= ((ydmin-ydmax)/(ypmax-ypmin));
    double ox = sx*(-xpmin)+ xdmin, oy = sx*(-ypmin)+ ydmin;

    xd=sx*pProy[0] + ox;
    yd=sy*pProy[1] + oy;

    rasterPoint.append(xd);
    rasterPoint.append(yd);
}



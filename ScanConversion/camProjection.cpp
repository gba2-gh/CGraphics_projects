#include "camProjection.h"
#include<QDebug>
#include<vector>
void   CamProjection::projectPoint(std::vector<std::vector<double> > vertices, bool ortho,  double xdmax ,double xdmin,
                                                                 double ydmax, double ydmin)
{
    std::vector<double> vProy;
 for(int k=0; k<8;++k){

    double point[4];
    for(int j=0;j<4;++j){
          point[j]=vertices[k][j];
                   }

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
    int f=1;

    if(ortho){//Ortogonal
        vProy.push_back(p[0]);
        vProy.push_back(p[1]);
    }
    else{//Perspectiva
        vProy.push_back(p[0]*f/-p[2]);
        vProy.push_back(p[1]*f/-p[2]);

    }
 }

    qDebug()<<vProy;


//todo CALCULAR CARAS



//RASTERIZAR
    int canvSc=1;
    if(ortho){canvSc=20;}
    int xd, yd;
    double xpmax=2*canvSc, xpmin=-2*canvSc;
    double ypmax=2*canvSc, ypmin=-2*canvSc;
    double sx= ((xdmax-xdmin)/(xpmax-xpmin)), sy= ((ydmin-ydmax)/(ypmax-ypmin));
    double ox = sx*(-xpmin)+ xdmin, oy = sx*(-ypmin)+ ydmin;

    for(int k=0; k<vProy.size();k=k+2){
        xd=sx*vProy[k] + ox;
        yd=sy*vProy[k+1] + oy;

        rasterPoint.append(xd);
        rasterPoint.append(yd);

       }
    qDebug()<<rasterPoint;
    vProy.clear();

}



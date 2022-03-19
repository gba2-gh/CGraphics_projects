#include "camProjection.h"
#include<QDebug>
#include<cmath>




void   CamProjection::projectPoint(CubeObject cubeobject, bool ortho,  double xdmax ,double xdmin,
                                                                 double ydmax, double ydmin)
{
    std::vector<std::vector<double> > vProy;

    for(int k=0; k<cubeobject.vertices.size();++k){//Proyectar cada vértice

        //MULTIPLICAR PUNTO POR EL MARCO DE LA CÁMARA
        double p[4];
        double suma;

        for(int i=0; i<4; ++i){
            for(int j=0;j<4;++j){
                    suma += camMarco[i][j]*cubeobject.vertices[k][j];
            }
            p[i]=suma;
            suma=0;
        }

        //PROYECTAR PUNTO
        int f=1;
        std::vector<double > vp;
        if(ortho){//Ortogonal
            vp.push_back(p[0]);
            vp.push_back(p[1]);
        }
        else{//Perspectiva
            vp.push_back(p[0]*f/-p[2]);
            vp.push_back(p[1]*f/-p[2]);
        }
        vProy.push_back(vp);
        vp.clear();

     }//end for proyeccion


    //RASTERIZAR
        //CALCULOS DE LA VENTANA
        int canvSc=1;
        if(ortho){canvSc=20;}
        QList<int> p_dev;
        double xpmax=2*canvSc, xpmin=-2*canvSc;
        double ypmax=2*canvSc, ypmin=-2*canvSc;
        double sx= ((xdmax-xdmin)/(xpmax-xpmin)), sy= ((ydmin-ydmax)/(ypmax-ypmin));
        double ox = sx*(-xpmin)+ xdmin, oy = sx*(-ypmin)+ ydmin;

        for(int k=0; k<vProy.size(); ++k){

            p_dev.append(sx*vProy[k][0] + ox);
            p_dev.append(sy*vProy[k][1] + oy);

            rasterPoint.append(p_dev);
            p_dev.clear();
       }

     vProy.clear();

///////////////////////////////////

}






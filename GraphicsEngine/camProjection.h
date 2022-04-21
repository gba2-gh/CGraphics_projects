#ifndef CAMPROJECTION_H
#define CAMPROJECTION_H
#include <QApplication>
#include<vector>
#include"cubeobject.h"


class CamProjection{

private:

public:
    void projectPoint(CubeObject cubeobject, bool ortho=false, double xdmax=400 ,double xdmin=0,
                                                    double ydmax=-400, double ydmin=0
            );
    //void persProjection(double p[4], int f=1);
    int imageWidth = 400;
    int imageHeight = 400;
    int yMax=0;
    int yMin=400;
    bool fillPolyBool=true;

    std::vector<std::vector <double> >  vertices;
    QList<QList<int> > rasterPoint;
    QList<double> rasterZ;
    QList<double> proy[3];
    QList<double> depthZ;


    std::vector<std::vector<double> > camMarco= {{1,0,0,0},
                                        {0,1,0,0},
                                        {0,0,1,0},
                                        {0,0,0,1}};



};


#endif // CAMPROJECTION_H

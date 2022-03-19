#ifndef RASTER_H
#define RASTER_H
#include <QApplication>
#include<vector>
#include"cubeobject.h"
#include"lights.h"

//#include"shading.h"


class raster
{
public:
    raster();

    void pipeline(CubeObject cubeobject, std::vector<lights*> lightScene, bool ortho,bool phongBool,bool cam1Bool);
    void drawFaces(int cam);
    void fillCubeFace(int v0, int v1, int v2, int v3);
    void scanLine(int v1, int v2);


    QList<double>   rasterObs[3];
    QList<double>   rasterLightPos[3];
    QList<double>   rasterNormal[3];
    QList<double>  rasterColor[3];
    QList<QList<int> > rasterPoint;


    int yBuffer[400][2]={0};
    double colorBuffer[3][400][2];
    double normalBuffer[3][400][2];
    double lightPosBuffer[3][400][2];
    double obsBuffer[3][400][2];

    bool phong;

};

#endif // RASTER_H

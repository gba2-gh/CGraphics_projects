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

    void pipeline(CubeObject cubeobject, std::vector<lights*> lightScene, bool ortho,bool phongBool,int camSelect, int shaderS);
    void drawFaces(CubeObject cubeobject);
    void fillCubeFace(CubeObject cubeobject);
    void scanLine(int v1, int v2);


    QList<double>   rasterObs[3];
    QList<double>   rasterLightPos[3];
    QList<double>   rasterNormal[3];
    QList<double>  rasterColor[3];
    QList<QList<int> > rasterPoint;
    QList<double>  rasterZ;


    int yBuffer[400][2]={0};
    double colorBuffer[3][400][2]={0};
    double normalBuffer[3][400][2]={0};
    double lightPosBuffer[3][400][2]={0};
    double obsBuffer[3][400][2]={0};
    double zBuffer[400][2]={0};
    double depthBuffer[400][400];

    bool phong;
    int shaderSel;

};

#endif // RASTER_H

#ifndef RASTER_H
#define RASTER_H
#include <QApplication>
#include<vector>
#include"cubeobject.h"
#include"lights.h"
#include<QImage>

//#include"shading.h"


class raster
{
public:
    raster();

    void pipeline(CubeObject cubeobject, std::vector<lights*> lightScene, bool ortho,bool phongBool,int camSelect, int shaderS, QImage texture_img);
    void drawFaces(CubeObject cubeobject);
    void fillCubeFace(CubeObject cubeobject, std::vector<lights*> lightScene);
    void scanLine(int v1, int v2, bool facein);


    QList<double>   rasterObs[3];
    QList<double>   rasterLightPos[3];
    QList<double>   rasterLightPos2[3];
    QList<double>   rasterNormal[3];
    QList<double>  rasterColor[3];
    QList<QList<int> > rasterPoint;
    QList<double>  rasterZ;
    QList<double>  depthZ;
    QList<double> proy[3];
    QList<double>  rasterUV[2];

    QList<double>  rasterColorT[3];

    QImage texture_img;



    int yBuffer[400][2]={0};
    double colorBuffer[3][400][2]={0};
    double normalBuffer[3][400][2]={0};
    double lightPosBuffer[3][400][2]={0};
    double lightPos2Buffer[3][400][2]={0};
    double obsBuffer[3][400][2]={0};
    double zBuffer[400][2]={0};
    double uvBuffer[2][400][2]={0};
    double depthBuffer[400][2]={0};


    bool phong;
    int shaderSel;

};

#endif // RASTER_H

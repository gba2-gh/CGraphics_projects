#ifndef CAMPROJECTION_H
#define CAMPROJECTION_H
#include <QApplication>
#include<vector>
#include"cubeobject.h"
#include"lights.h"


class CamProjection{

private:

public:
    void projectPoint(CubeObject cubeobject, std::vector<lights> lightScene, bool ortho=false, double xdmax=400 ,double xdmin=0,
                                                    double ydmax=-400, double ydmin=0, int cam=0, bool phongBool=false
            );
    //void persProjection(double p[4], int f=1);
    int imageWidth = 400;
    int imageHeight = 400;
    int yBuffer[400][2]={ 0 };
    int yMax=0;
    int yMin=400;
    bool fillPolyBool=true;
    double colorBuffer[3][400][2];
    double normalBuffer[3][400][2];
    double lightPosBuffer[3][400][2];
    double obsBuffer[3][400][2];

    std::vector<std::vector <double> >  vertices;

    QList<double>   rasterObs[3];
    QList<double>   rasterLightPos[3];
    QList<double>   rasterNormal[3];
    QList<double>  rasterColor[3];
    QList<QList<int> > rasterPoint;


    std::vector<std::vector<double> > camMarco= {{1,0,0,0},
                                        {0,1,0,0},
                                        {0,0,1,0},
                                        {0,0,0,1}};

    void scanLine(int v1, int v2  );
    void scanDDA();
    void drawFaces(int cam);
    //void fillCubeFace(std::vector<QList<int> > face);
    void fillCubeFace(int v0, int v1, int v2, int v3);


    //double dot_product(std::vector<double> v1, std::vector<double> v2);

};


#endif // CAMPROJECTION_H

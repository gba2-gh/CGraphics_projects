#ifndef CAMPROJECTION_H
#define CAMPROJECTION_H
#include <QApplication>
#include<vector>

class CamProjection{

private:

public:
    void projectPoint(std::vector<std::vector<double> > vertices, bool ortho=false, double xdmax=400 ,double xdmin=0,
                                                    double ydmax=-400, double ydmin=0
            );
    //void persProjection(double p[4], int f=1);
    int imageWidth = 400;
    int imageHeight = 400;
    int yBuffer[400][400]={ 0 };
    int yMax=0;
    int yMin=400;
    bool fillPolyBool=false;


    QList<int> rasterPoint;

    std::vector<std::vector<double> > camMarco= {{1,0,0,0},
                                        {0,1,0,0},
                                        {0,0,1,0},
                                        {0,0,0,1}};

    void scanFillPoly(double x1, double y1, double x2, double y2);
    void scanDDA();
    void drawFaces();
    void fillCubeFace(std::vector<int> face);
};


#endif // CAMPROJECTION_H

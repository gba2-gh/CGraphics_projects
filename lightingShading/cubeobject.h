#ifndef CUBEOBJECT_H
#define CUBEOBJECT_H

#include<vector>
class CubeObject
{
public:
    CubeObject();
    std::vector<std::vector<double> > vertices={{0,0,0,1},
                              {0,10,0,1},
                              {20,10,0,1},
                              {20,0,0,1},
                              {20,0,15,1},
                              {20,10,15,1},
                              {0,10,15,1},
                              {0,0,15,1}
                             };
    double ka[3]={0.3, 0.3,0.3};
    double kd[3]={0.07568, 0.61424,0.07568};
    double ke[3]={0.633, 0.72811,0.66};


   //caras
   std::vector<std::vector<std::vector <double> > > faces;

   std::vector<std::vector <double> > normals;
//    std::vector<std::vector<double> > vertices={{20,15,0,1},
//                              {0,10,0,1},
//                              {20,0,0,1},
//                             };


    void rotateObject(double angleD);
    std::vector<std::vector <double> > createFace(int v0, int v1, int v2, int v3);
    std::vector<double> calcNormal(std::vector<std::vector<double> > face);

};

#endif // CUBEOBJECT_H

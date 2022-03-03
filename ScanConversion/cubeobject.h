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

//    std::vector<std::vector<double> > vertices={{20,15,0,1},
//                              {0,10,0,1},
//                              {20,0,0,1},
//                             };


    void rotateObject(double angleD);
    void createFaces();
};

#endif // CUBEOBJECT_H

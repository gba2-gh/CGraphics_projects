#ifndef CUBEOBJECT_H
#define CUBEOBJECT_H

#include<vector>
class CubeObject
{
public:
    CubeObject();
    std::vector<std::vector<double> > coord={{0,0,0,1},
                              {0,10,0,1},
                              {20,10,0,1},
                              {20,0,0,1},
                              {20,0,15,1},
                              {20,10,15,1},
                              {0,10,15,1},
                              {0,0,15,1}
                             };
    void rotateObject(double angleD);

};

#endif // CUBEOBJECT_H

#include "cubeobject.h"
#include<cmath>
#include"transformations.h"

#define MY_PI 3.14159265358979323846
CubeObject::CubeObject()
{

}

void CubeObject::rotateObject(double angleD)
{
    double angleR=angleD*MY_PI/180;

    double rotMatX[4][4]= {1,0,0,0,
                          0, cos(angleR), sin(angleR),0,
                          0, -sin(angleR), cos(angleR),0,
                          0,0,0,1};


    this->vertices= rotateEuler(this->vertices,rotMatX);

}

void CubeObject::createFaces()
{
    }

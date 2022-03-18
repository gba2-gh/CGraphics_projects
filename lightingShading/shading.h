#ifndef SHADING_H
#define SHADING_H
#include"cubeobject.h"
#include"lights.h"
#include<cmath>
#include<qdebug.h>
#include"functions.h"

double calcLightVertex(CubeObject cubeobject, std::vector<lights> lightScene, int k, int i );


#endif // SHADING_H

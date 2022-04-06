#ifndef SHADING_H
#define SHADING_H
#include<cmath>
#include<qdebug.h>


double calcLightVertex(CubeObject cubeobject, std::vector<lights*> lightScene, int k, int i );

std::vector<double> lightVertex(CubeObject cubeobject, std::vector<double> L, std::vector<double> N, std::vector<double> O, std::vector<lights*> lightScene);



#endif // SHADING_H


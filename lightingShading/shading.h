#ifndef SHADING_H
#define SHADING_H
#include"cubeobject.h"
#include"lights.h"
#include<cmath>
#include<qdebug.h>
#include"functions.h"

double calcLightVertex(CubeObject cubeobject, std::vector<lights> lightScene, int k, int i );

std::vector<double> lightVertex(CubeObject cubeobject, std::vector<double> L, std::vector<double> N, std::vector<double> O, std::vector<lights> lightScene);
#endif // SHADING_H

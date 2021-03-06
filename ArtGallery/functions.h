#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>
#include<vector>
#include<QDebug>
#include<object.h>


bool importFile( const std::string& pFile, Object *object);


std::vector<std::vector<double>> rotateEuler( std::vector<std::vector<double> > coord, double rotMat[4][4]);

std::vector<double> sum_vectors(std::vector<double> v1, std::vector<double> v2);

std::vector<double> norm_vector(std::vector<double> v);

double dot_product(std::vector<double> v1, std::vector<double> v2);

QList<double> horInterpolation(int yBuffer[400][2], double colorBuffer[400][2]);

QList<QList<int>> scanConversion(int yBuffer[400][2]);

QVector3D Q_cross_product(QVector3D v1, QVector3D v2);

#endif // FUNCTIONS_H

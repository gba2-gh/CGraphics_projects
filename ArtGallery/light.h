#ifndef LIGHT_H
#define LIGHT_H

#include<qvector3d.h>
#include<QMatrix4x4>

class Light
{
public:
    Light();

    int type;

    QMatrix4x4 projection;
    QMatrix4x4 view;
    QVector3D position;
    QVector3D direction;
    float  intensity;
    float color[3];


    float attFactor_k;
    float attFactor_l;
    float attFactor_q;
    float cut_angle;

    float bias=0.005;

    void setViewMatrix();

};

#endif // LIGHT_H

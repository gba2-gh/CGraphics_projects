#ifndef LIGHTS_H
#define LIGHTS_H

#include<qvector3d.h>
#include<QMatrix4x4>

class Lights
{
public:
    Lights();

    int type;

    QMatrix4x4 projection;
    QVector3D position;
    QVector3D direction;
    float  intensity;
    float color[3];


    float attFactor_k;
    float attFactor_l;
    float attFactor_q;
    float cut_angle;

    float bias=0.005;


};

#endif // LIGHTS_H

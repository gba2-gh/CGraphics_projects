#ifndef LIGHTS_H
#define LIGHTS_H

#include<qvector3d.h>


class Lights
{
public:
    Lights();

    int type;

    QVector3D position;
    QVector3D direction;
    float  intensity;
    float color[3];


    float attFactor_k;
    float attFactor_l;
    float attFactor_q;
    float cut_angle;



};

#endif // LIGHTS_H

#include "light.h"

Light::Light()
{

    position.setX(0.0f); position.setY(0.0f); position.setZ(0.0f);
    direction.setX(0.0f); direction.setY(0.0f); direction.setZ(0.0f);
    intensity= 1.0;
    color[0]=1.0; color[1]=1.0; color[2]=1.0;


    attFactor_k = 1.0;
    attFactor_l = 0.22;
    attFactor_q = 0.20;

}

void Light::setViewMatrix()
{
    QMatrix4x4 aux;

    aux.lookAt(position, direction, QVector3D(0.0f, 1.0f, 0.0f));

    view = aux;
}

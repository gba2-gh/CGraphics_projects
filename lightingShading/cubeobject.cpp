#include "cubeobject.h"
#include<cmath>
#include"transformations.h"
#include<qdebug.h>


#define MY_PI 3.14159265358979323846
CubeObject::CubeObject()
{
    ///CREATE FACES
    faces.insert(faces.end(), createFace(0,1,2,3));
    faces.insert(faces.end(), createFace(4,5,6,7));

    faces.insert(faces.end(), createFace(0,1,6,7));
    faces.insert(faces.end(), createFace(2,3,4,5));

    faces.insert(faces.end(), createFace(1,2,5,6));
    faces.insert(faces.end(), createFace(0,3,4,7));
    //qDebug()<<"dos"<<faces[5];


    ///CALCULAR NORMALES
    ///
    for(int i=0; i<=5; ++i){
        normals.insert(normals.end(), calcNormal(faces[i]));
    }

    qDebug()<<normals;
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

std::vector<std::vector <double> > CubeObject::createFace(int v0, int v1, int v2, int v3)
{
    std::vector<std::vector <double> >  face;
    face.insert(face.end(), vertices[v0]);
    face.insert(face.end(), vertices[v1]);
    face.insert(face.end(), vertices[v2]);
    face.insert(face.end(), vertices[v3]);
    return face;
    }

std::vector<double> CubeObject::calcNormal(std::vector<std::vector<double> > face){
    std::vector<double> v0v1;
    std::vector<double> v0v3;

    for(int i=0; i<face[1].size()-1; ++i){
        v0v1.push_back(face[1][i]-face[0][i]);
    }
    for(int i=0; i<face[1].size()-1; ++i){
        v0v3.push_back(face[3][i]-face[0][i]);
    }

      //Producto cruz
    std::vector<double> pc;
    pc.push_back(v0v1[1]*v0v3[2] - v0v1[2]*v0v3[1]);
    pc.push_back(-(v0v1[0]*v0v3[2] - v0v1[2]*v0v3[0]));
    pc.push_back(v0v1[0]*v0v3[1] - v0v1[1]*v0v3[0]);

    return pc;

}

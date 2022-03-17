#include "cubeobject.h"
#include<cmath>
#include"transformations.h"
#include<qdebug.h>


#define MY_PI 3.14159265358979323846
CubeObject::CubeObject()
{

    facesV.insert(facesV.end(), {face0, face1, face2, face3, face4, face5});
    ///CREATE FACES
    facesV[0].vertices= createFace(0,1,2,3);

    ///formar caras como referencia a los vertices de la cara
    std::vector<std::reference_wrapper<std::vector<double>> > faceP;

    for(int i=0; i<vertices.size(); ++i){
        faceP.push_back( vertices[i]);
    }

    qDebug() << faceP[0].get();

    createFaces();

    ///CALCULAR NORMALES
    ///
    ///
    for(int i=0; i<=5; ++i){
        faceNormals.insert(faceNormals.end(), calcFaceNormal(faces[i]));
        facesV[i].normal = calcFaceNormal(faces[i]) ;
    }


    ////VERTEX NORMAL
    ///
    calcVerticesNormal();

    //qDebug()<<faceNormals;



}

void CubeObject::rotateObject(double angleD)
{
    double angleR=angleD*MY_PI/180;

    double rotMatX[4][4]= {1,0,0,0,
                          0, cos(angleR), sin(angleR),0,
                          0, -sin(angleR), cos(angleR),0,
                          0,0,0,1};


    this->vertices= rotateEuler(this->vertices,rotMatX);

    createFaces();
       faceNormals.clear();
    for(int i=0; i<=5; ++i){
        faceNormals.insert(faceNormals.end(), calcFaceNormal(faces[i]));
    }

    calcVerticesNormal();



}


void CubeObject::createFaces()
{
    faces.clear();
    faces.insert(faces.end(), createFace(0,1,2,3)); //f0
    faces.insert(faces.end(), createFace(4,5,6,7)); //f1

    faces.insert(faces.end(), createFace(7,6,1,0)); //f2
    faces.insert(faces.end(), createFace(5,4,3,2)); //f3

    faces.insert(faces.end(), createFace(6,5,2,1)); //f4
    faces.insert(faces.end(), createFace(0,3,4,7)); //f5
    //qDebug()<<"dos"<<faces[5];
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

std::vector<double> CubeObject::calcFaceNormal(std::vector<std::vector<double> > face){

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



void CubeObject::calcVertexNormal(std::vector<double> fn1,
                                                 std::vector<double> fn2,
                                                 std::vector<double> fn3)
{

    std::vector<double> v0n;

    v0n=sum_vectors(fn1, fn2);
    v0n=sum_vectors(v0n, fn3);
    v0n=norm_vector(v0n);
    vertexNormals.push_back(v0n);

}

void CubeObject::calcVerticesNormal()
{
    vertexNormals.clear();
    calcVertexNormal(faceNormals[0],faceNormals[2],faceNormals[5]); //v0
    calcVertexNormal(faceNormals[0],faceNormals[2],faceNormals[4]); //v1
    calcVertexNormal(faceNormals[0],faceNormals[3],faceNormals[4]); //v2
    calcVertexNormal(faceNormals[0],faceNormals[3],faceNormals[5]); //v3
    calcVertexNormal(faceNormals[1],faceNormals[3],faceNormals[5]); //v4
    calcVertexNormal(faceNormals[1],faceNormals[3],faceNormals[4]); //v5
    calcVertexNormal(faceNormals[1],faceNormals[2],faceNormals[4]); //v6
    calcVertexNormal(faceNormals[1],faceNormals[2],faceNormals[5]); //v7
   // qDebug() << vertexNormals;

}




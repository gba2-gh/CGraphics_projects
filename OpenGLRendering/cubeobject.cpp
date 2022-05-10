#include "cubeobject.h"
#include<cmath>
#include"functions.h"
#include<qdebug.h>


#define MY_PI 3.14159265358979323846
CubeObject::CubeObject()
{

        for(int i=0; i<=2; ++i){
            mat1.ka[i]=0.0;
            mat1.kd[i]=0.5;
            mat1.ke[i]=0.7;

            mat2.ka[i]=0.23125;
            mat2.kd[i]=0.2775;
            mat2.ke[i]=0.773911;
        }
        mat1.ro=32;
        mat2.ro=89.6;

        mat1.O.insert(mat1.O.end(), {40, 0 ,40 });
        mat2.O.insert(mat2.O.end(), {0, 0 ,40 });

        mat1.url = R"(../GraphicsEngine/check.png)";
        mat2.url = R"(../GraphicsEngine/texture.jpg)";



        all_mat.push_back(mat1);
        all_mat.push_back(mat2);



}

void CubeObject::faceVertices(int i, int v0, int v1, int v2, int v3)
{

    facesV[i].vertices.push_back(vertices[v0]);
    facesV[i].vertices.push_back(vertices[v1]);
    facesV[i].vertices.push_back(vertices[v2]);
    facesV[i].vertices.push_back(vertices[v3]);


}


void CubeObject::rotateX(double angleD)
{
    double angleR=angleD*MY_PI/180;

    double rotMatX[4][4]= {1,0,0,0,
                          0, cos(angleR), sin(angleR),0,
                          0, -sin(angleR), cos(angleR),0,
                          0,0,0,1};

    this->vertices= rotateEuler(this->vertices,rotMatX);

    calcVerticesNormal();

}

void CubeObject::rotateY(double angleD)
{
    double angleR=angleD*MY_PI/180;

    double rotMat[4][4]= {cos(angleR),0,sin(angleR),0,
                          0, 1, 0,0,
                          -sin(angleR), 0, cos(angleR),0,
                          0,0,0,1};

    this->vertices= rotateEuler(this->vertices,rotMat);

    calcVerticesNormal();

}





void CubeObject::calcVerticesNormal()
{
    ///CREATE FACE
    ///formar caras como referencia a los vertices de la cara
    createFaces();

    ///CALCULAR NORMALES
    ///
    ///
    faceNormals.clear();
    for(int i=0; i<faces.size(); ++i){
        faceNormals.insert(faceNormals.end(), calcFaceNormal(faces[i]));
        //facesV[i].normal = calcFaceNormal(facesV[i].get_face()) ;
    }

    std::vector<double> vn={0,0,0};

    vertexNormals.clear();
    for(int v_id=0; v_id< vertices.size(); ++v_id){
        for(int i=0; i<facesIdx.size(); ++i){
            for(int j=0; j<facesIdx[i].size(); ++j){

               if(facesIdx[i][j]==v_id){ //TODO OPTIMIZAR

                   vn =sum_vectors(vn, faceNormals[i]);
               }
            }
          }
        vn=norm_vector(vn);
        vertexNormals.push_back(vn);
        vn={0,0,0};
     }

    //qDebug() <<"vertices: " <<vertices;
    //qDebug() <<"vnormal: " << vertexNormals;

}




void CubeObject::createFaces()
{
    faces.clear();
    std::vector<std::vector<double> > face;

    for(int i=0; i<facesIdx.size(); ++i){
        for(int j=0; j<facesIdx[i].size(); ++j){
            face.push_back(vertices[facesIdx[i][j]]);
        }
        faces.push_back(face);
        face.clear();

    }
    //faces.push_back( createFace(facesIdx[i][0], facesIdx[i][1], facesIdx[i][2], facesIdx[i][3]));


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
        v0v3.push_back(face[2][i]-face[0][i]);
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





std::vector<std::vector<double> > CubeObject::face::get_face()
{

                std::vector<std::vector<double>> vec;
                for(int i=0; i<=3;++i){
                    vec.push_back(this->vertices[i].get());
                }

                return vec;

}

#include <cmath>
#include<vector>
#include<QDebug>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include"object.h"


bool importFile(const std::string &pFile, Object *object)
{
    // Create an instance of the Importer class
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(pFile,
      aiProcess_JoinIdenticalVertices);

     aiMesh* mesh = scene->mMeshes[0];
     aiVector3D* pPos ;
     aiVector3D uv ;
     aiVector3D* normal;
     aiFace* face;
     //aiUVTransform* uv;

     std::vector<double> vertex;
     std::vector<std::vector<double>> vertices;
     std::vector<std::vector<double>> normales;


     ///VERTICE
     for(int i=0; i<mesh->mNumVertices; ++i){
          pPos = &(mesh->mVertices[i]);
          vertex.push_back(pPos->x);
          vertex.push_back(pPos->y);
          vertex.push_back(pPos->z);
          //vertex.push_back(1);
          vertices.push_back(vertex);
          vertex.clear();
     }

     object->vertices= vertices;


    ///NORMALES
    if(&(mesh->mNormals[0]) != NULL){

     for(int i=0; i<mesh->mNumVertices; ++i){
      normal = &(mesh->mNormals[i]);
      vertex.push_back(normal->x);
      vertex.push_back(normal->y);
      vertex.push_back(normal->z);
      normales.push_back(vertex);
      vertex.clear();
     }
    }else{
        vertex.push_back(0);
        normales.push_back(vertex);
    }


    object->vertexNormals=normales;

    ///INDICES
     std::vector<std::vector<int> > facesIdx;
     std::vector<int> idx;
     for(int i=0; i<mesh->mNumFaces; ++i){
         face = &(mesh->mFaces[i]);
          for(int j=0; j<face->mNumIndices; ++j){
            idx.push_back(face->mIndices[j]);
            //qDebug() << face->mIndices[0];
          }
          facesIdx.push_back(idx);
          idx.clear();
     }

      object->facesIdx = facesIdx;


      ///UV
       std::vector<std::vector<double> > uvCoord;
       std::vector<double> temp;

       if(mesh->mTextureCoords[0] != NULL){
           for(int i=0; i< mesh->mNumVertices ; ++i){
               uv =  (mesh->mTextureCoords[0][i]);
               temp.push_back(uv.x);
               temp.push_back(uv.y);
               uvCoord.push_back(temp);
               temp.clear();
           }
       }else{
           temp.push_back(0);
            uvCoord.push_back(temp);
       }
       object->uvCoord = uvCoord;

    ///LIST
    int totalV= object->vertices.size();
    int sizeV = object->vertices[0].size();
    for(int i=0; i<totalV; ++i ){
        for(int j=0; j<sizeV; ++j){
            object->vertList.push_back( object->vertices[i][j]);
        }
        for(int j=0; j<sizeV; ++j){
            object->vertList.push_back( object->vertexNormals[i][j]);
        }
        for(int j=0; j<2; ++j){
            object->vertList.push_back( object->uvCoord[i][j]);
        }
    }


    for(int i=0; i< object->facesIdx.size(); ++i ){
        for(int j=0; j<object->facesIdx[0].size(); ++j){
            object->indexList.push_back( object->facesIdx[i][j]);
        }
    }


    //qDebug() << facesIdx;
    if (nullptr != scene) {
        printf("Error");
        return false;
    }



    return true;
}




std::vector<std::vector<double> > rotateEuler(std::vector<std::vector<double> > coord, double rotMat[4][4]){

    double aux[4];
    double suma=0;

    for(int i=0; i<8;++i){
        for(int j=0; j<4;++j){
            for(int k=0; k<4;++k){

                suma += coord[i][k]*rotMat[j][k];

            }
            aux[j]=suma;
            suma=0;
        }

        for(int w=0; w<4;++w){

            coord[i][w] = aux[w];

        }

    }

    return coord;
}

std::vector<double> sum_vectors(std::vector<double> v1, std::vector<double> v2)
{
    std::vector<double> sum;
    for(int i=0; i<v1.size(); ++i ){
        sum.push_back(v1[i]+v2[i]);
    }

    return sum;

}

std::vector<double> norm_vector(std::vector<double> v)
{
    double norm=0;
    std::vector<double> norm_vec;
    for(int i=0; i<v.size(); ++i ){
        norm += v[i]*v[i];
    }

    norm=sqrt(norm);

    for(int i=0; i<v.size(); ++i ){
        norm_vec.push_back(v[i]/norm);

    }

    //qDebug() << norm;
    return norm_vec;

}

double dot_product(std::vector<double> v1, std::vector<double> v2){

    double sum=0;
    for(int i=0; i<v1.size(); ++i){
        sum += v1[i]*v2[i];
    }
    return sum;
}



QList<double> horInterpolation(int yBuffer[400][2], double colorBuffer[400][2]){

    QList<double>  rasterColor;

    int xmin=0,xmax=0;
    double imin=0;
    double imax=0;

    double deltaI;

    for(int y=0;y<400;++y){

           if(yBuffer[y][0]<yBuffer[y][1]){
                    xmin= yBuffer[y][0];
                    xmax= yBuffer[y][1];

                    imin= colorBuffer[y][0];
                    imax= colorBuffer[y][1];
            }
            else{
                    xmin= yBuffer[y][1];
                    xmax= yBuffer[y][0];
                    imin= colorBuffer[y][1];
                    imax= colorBuffer[y][0];
            }

           deltaI=(imin-imax)/(xmin-xmax);

           for(int x=xmin; x<=xmax; ++x){

               rasterColor.append(imin);
               imin += deltaI;
            }

    }

      return rasterColor;
}


QList<QList<int>> scanConversion(int yBuffer[400][2]){
    QList<QList<int>> output;
    int xmin=0,xmax=0;
    QList<int> p_fill;

    for(int y=0;y<400;++y){

           if(yBuffer[y][0]<yBuffer[y][1]){
                    xmin= yBuffer[y][0];
                    xmax= yBuffer[y][1];
              }
           else{
                    xmin= yBuffer[y][1];
                    xmax= yBuffer[y][0];
               }

           for(int x=xmin; x<=xmax; ++x){
                p_fill.append(x);
                p_fill.append(y);
                output.append(p_fill);
                p_fill.clear();
            }
     }

    return output;
}



QVector3D Q_cross_product(QVector3D v1, QVector3D v2)
{
    QVector3D result;

    result.setX(v1.y()*v2.z() - v1.z()*v2.y());
    result.setY(-(v1.x()*v2.z() - v1.z()*v2.x()));
    result.setZ(v1.x()*v2.y() - v2.x()*v1.y());

    return result;
}

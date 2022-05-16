#ifndef CUBEOBJECT_H
#define CUBEOBJECT_H

#include<vector>
#include<QImage>
#include<qstring.h>
class CubeObject
{
public:
    CubeObject();


    struct material
    {
        double ka[4]={0,0,0,1};
        double kd[4]={0,0,0,1};
        double ke[4]={0,0,0,1};
        double ro=0;
        double Ia=200;

        std::vector<double> O;

        QString url;
        QImage texture;
    };

    material mat1, mat2;
    std::vector<material> all_mat;
    int curr_mat=0;


    std::vector<std::vector<double> > vertices;
    std::vector<std::vector<int> > facesIdx;
    std::vector<std::vector<double> > uvCoord;

    std::vector<double> textureFaces;
   //caras
   std::vector<std::vector<std::vector <double> > > faces;

   std::vector<std::vector <double> > faceNormals;
   std::vector<std::vector <double> >  vertexNormals;


    std::vector<double> calcFaceNormal(std::vector<std::vector<double> > face);

    void calcVerticesNormal();
    void createFaces();
    void rotateX(double angleD);
    void rotateY(double angleD);

};

#endif // CUBEOBJECT_H

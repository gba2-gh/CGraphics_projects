#ifndef CUBEOBJECT_H
#define CUBEOBJECT_H

#include<vector>
class CubeObject
{
public:
    CubeObject();
    std::vector<std::vector<double> > vertices;

    struct material
    {
        double ka[4]={0,0,0,1};
        double kd[4]={0,0,0,1};
        double ke[4]={0,0,0,1};
        double ro=0;
        double Ia=200;
    };

    material mat1, mat2;
    std::vector<material> all_mat;
    int curr_mat=0;

    struct face
    {
        std::vector<std::reference_wrapper<std::vector<double> > >  vertices;
        double position=0;
        std::vector <double> normal;

        std::vector<std::vector<double>> get_face();
    };

    face face0, face1, face2, face3, face4, face5;
    std::vector<face> facesV;


    std::vector<std::vector<int> > facesIdx;


   //caras
   std::vector<std::vector<std::vector <double> > > faces;

   std::vector<std::vector <double> > faceNormals;
   std::vector<std::vector <double> >  vertexNormals;


    void faceVertices(int i, int v0, int v1, int v2, int v3);
    std::vector<std::vector <double> > createFace(int v0, int v1, int v2, int v3);


    std::vector<double> calcFaceNormal(std::vector<std::vector<double> > face);
    void calcVertexNormal(std::vector<double> fn1,
                                                     std::vector<double> fn2,
                                                     std::vector<double> fn3);
    void calcVerticesNormal();
    void createFaces();
    void rotateObject(double angleD);

};

#endif // CUBEOBJECT_H

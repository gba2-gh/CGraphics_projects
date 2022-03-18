#ifndef CUBEOBJECT_H
#define CUBEOBJECT_H

#include<vector>
class CubeObject
{
public:
    CubeObject();
    std::vector<std::vector<double> > vertices={{0,0,0,1},
                              {0,10,0,1},
                              {20,10,0,1},
                              {20,0,0,1},
                              {20,0,15,1},
                              {20,10,15,1},
                              {0,10,15,1},
                              {0,0,15,1}
                             };
    double ka[3]={0.3, 0.3,0.3};
    double kd[3]={0.07568, 0.61424,0.07568};
    double ke[3]={0.633, 0.72811,0.66};

    struct face
    {
        std::vector<std::reference_wrapper<std::vector<double> > >  vertices;
        double position=0;
        std::vector <double> normal;

        std::vector<std::vector<double>> get_face();

    };

    face face0, face1, face2, face3, face4, face5;
    std::vector<face> facesV;



   //caras
   std::vector<std::vector<std::vector <double> > > faces;

   std::vector<std::vector <double> > faceNormals;
   std::vector<std::vector <double> >  vertexNormals;
//    std::vector<std::vector<double> > vertices={{20,15,0,1},
//                              {0,10,0,1},
//                              {20,0,0,1},
//                             };

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

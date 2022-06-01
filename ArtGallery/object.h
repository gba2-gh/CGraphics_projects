#ifndef OBJECT_H
#define OBJECT_H

#include<vector>
#include<QImage>
#include<qstring.h>

#include <QOpenGLFunctions_4_5_Core>




#include <QtOpenGL>


class Object
{
public:
    Object();

    struct material
    {
        float ka[4]={0,0,0,1};
        float kd[4]={0,0,0,1};
        float ke[4]={0,0,0,1};
        float ro=0;
        float Ia=200;

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

     std::vector<float> vertList;
     std::vector<GLuint> indexList;

    unsigned int VBO, VAO, EBO;

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

    void render(QMatrix4x4 proj, QMatrix4x4 view,QVector3D camPos, QOpenGLShaderProgram *program);

    QOpenGLShaderProgram *program;
       QOpenGLExtraFunctions *f;

       QMatrix4x4 model;

};

#endif // OBJECT_H

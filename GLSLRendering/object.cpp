#include "object.h"
#include<cmath>
#include"functions.h"
#include<qdebug.h>


#include <QOpenGLFunctions_4_5_Core>

#define MY_PI 3.14159265358979323846
Object::Object()
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

        mat1.url = R"(../OpenGLRendering/check.png)";
        mat2.url = R"(../OpenGLRendering/texture.jpg)";


        QString url = R"(../OpenGLRendering/card.jpg)";
        QString url2 = R"(../OpenGLRendering/rust.jpg)";
        QImage input(url);
        QImage input2(url2);
        QTransform myTransform;
        myTransform.rotate(90);

        input = input.transformed(myTransform);
        input2 = input2.transformed(myTransform);

        mat1.texture =  input.mirrored(false,true);
        mat2.texture =  input2.mirrored(false,true);


        all_mat.push_back(mat1);
        all_mat.push_back(mat2);





}



void Object::rotateX(double angleD)
{
    double angleR=angleD*MY_PI/180;

    double rotMatX[4][4]= {1,0,0,0,
                          0, cos(angleR), sin(angleR),0,
                          0, -sin(angleR), cos(angleR),0,
                          0,0,0,1};

    this->vertices= rotateEuler(this->vertices,rotMatX);

    calcVerticesNormal();

}

void Object::rotateY(double angleD)
{
    double angleR=angleD*MY_PI/180;

    double rotMat[4][4]= {cos(angleR),0,sin(angleR),0,
                          0, 1, 0,0,
                          -sin(angleR), 0, cos(angleR),0,
                          0,0,0,1};

    this->vertices= rotateEuler(this->vertices,rotMat);

    calcVerticesNormal();

}


void Object::calcVerticesNormal()
{
    ///CREATE FACE
    ///formar caras como referencia a los vertices de la cara
    createFaces();

    ///CALCULAR NORMALES
    faceNormals.clear();
    for(int i=0; i<faces.size(); ++i){
        faceNormals.insert(faceNormals.end(), calcFaceNormal(faces[i]));
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




void Object::createFaces()
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

 }



std::vector<double> Object::calcFaceNormal(std::vector<std::vector<double> > face){

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




void Object::render(QOpenGLContext *context, QOpenGLShaderProgram *program){

    program->bind();

    float vertices[this->vertices.size() *
                            this->vertices[0].size()*2];

    GLuint indices[this->facesIdx.size() *
                            this->facesIdx[0].size()];

    int totalV= this->vertices.size();
    int sizeV = this->vertices[0].size();
    for(int i=0; i<totalV; ++i ){
        for(int j=0; j<sizeV; ++j){
            vertices[i*(sizeV*2)+j] = this->vertices[i][j];
            vertices[i*(sizeV*2)+j+3] = this->vertexNormals[i][j];
        }
    }

    for(int i=0; i<this->facesIdx.size(); ++i ){
        for(int j=0; j<this->facesIdx[0].size(); ++j){
            indices[i*(this->facesIdx[0].size())+j] = this->facesIdx[i][j];
        }
    }


    //context->makeCurrent(this);

        //QOpenGLFunctions_4_5_Core *f = new QOpenGLFunctions_4_5_Core;
        //f->initializeOpenGLFunctions();

        f = QOpenGLContext::currentContext()->extraFunctions();

        f->glGenVertexArrays(1, &VAO);
        f->glGenBuffers(1, &VBO);
        f->glGenBuffers(1, &EBO);

        //bind
        f->glBindVertexArray(VAO);

        f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
         f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
         f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
         f->glEnableVertexAttribArray(0);

        f->glBindBuffer(GL_ARRAY_BUFFER, 0);
        f->glBindVertexArray(0);




        QMatrix4x4 proj;

        proj.perspective(45.0f,1, 0.1f, 100.0f);

        QMatrix4x4 view;
            view.translate(0.0f, -1.0f, -8.0f);


        //QMatrix4x4 model;
        //model.translate(0.0f, 0.0f, 0.0f);
        //model.rotate(45, 0,1, 0);

        QMatrix4x4 normalMat;
        normalMat = model.transposed();
        normalMat = normalMat.inverted();




        int projLocation = program->uniformLocation("proj");
        int viewLocation = program->uniformLocation("view");
        int modelLocation = program->uniformLocation("model");

        int lightPosLoc = program->uniformLocation("lightPos");
        int lightColorLoc = program->uniformLocation("lightColor");
        int lightPos2Loc = program->uniformLocation("lightPos2");
        int lightColor2Loc = program->uniformLocation("lightColor2");

        program->setUniformValue(projLocation, proj);
        program->setUniformValue(viewLocation, view);
        program->setUniformValue(modelLocation, model);
        program->setUniformValue("normalMat", normalMat);


        program->setUniformValue(lightColorLoc, 1.0f, 1.0f, 1.0f);
        program->setUniformValue(lightPosLoc, 2.0f, 2.0f, 2.0f);
        program->setUniformValue("lightIntensity", 1.0f);

        program->setUniformValue(lightColor2Loc, 0.0f, 0.1f, 0.8f);
        program->setUniformValue(lightPos2Loc, 2.0f, -2.0f, 2.0f);


        if(true){
        program->setUniformValue("lightIntensity2", 1.0f);}
        else{
            program->setUniformValue("lightIntensity2", 0.0f);
        }



        program->setUniformValue("ka", this->all_mat[curr_mat].ka[0],
                                        this->all_mat[curr_mat].ka[1],
                                            this->all_mat[curr_mat].ka[2]);
        program->setUniformValue("kd", this->all_mat[curr_mat].kd[0],
                                        this->all_mat[curr_mat].kd[1],
                                            this->all_mat[curr_mat].kd[2]);
        program->setUniformValue("ke", this->all_mat[curr_mat].ke[0],
                                            this->all_mat[curr_mat].ke[1],
                                                 this->all_mat[curr_mat].ke[2]);
        program->setUniformValue("shininess", this->all_mat[curr_mat].ro);

        program->setUniformValue("eyePos", 1.0f, 0.0f, 1.0f);
        program->setUniformValue("shaderPhong", true);

        program->bind();


        f->glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, this->facesIdx.size()*this->facesIdx[0].size(), GL_UNSIGNED_INT, 0);
        f->glBindVertexArray(0);

       // program->release();



}

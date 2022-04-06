#include"cubeobject.h"
#include"lights.h"
#include"functions.h"
#include<cmath>


double calcLightVertex(CubeObject cubeobject, std::vector<lights*> lightScene, int k, int i){

    //LIGHTS////
    double I=0;
    double dp=0;
    double Ia=100;
    double aux;
    std::vector<double> L;
    std::vector<double> op= {15,0,15};
    std::vector<double> o;
    std::vector<double> h;
    double p=1;//p=76.8;

    //color
    //diffuse
    // for(int i=0; i<=2; ++i){

    I= cubeobject.all_mat[cubeobject.curr_mat].Ia*cubeobject.all_mat[cubeobject.curr_mat].ka[i];

    for (int j=0; j<=2; ++j){
        L.push_back(cubeobject.vertices[k][j]-lightScene[0]->lightPos[j]);
    }
    dp=dot_product(L,cubeobject.vertexNormals[k]);
    I += lightScene[0]->color[i]*(lightScene[0]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp) ;

    L.clear();

    //L2
    for (int j=0; j<=2; ++j){
        L.push_back(cubeobject.vertices[k][j]-lightScene[1]->lightPos[j]);
    }
    dp=dot_product(L,cubeobject.vertexNormals[k]);
    double aux2=lightScene[1]->color[i]*(lightScene[1]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp) ;
    I += abs(aux2);

    L.clear();

    //specular
    for (int j=0; j<=2; ++j){
        L.push_back(cubeobject.vertices[k][j]-lightScene[2]->lightPos[j]);
    }
    dp= 2*dot_product(L, cubeobject.vertexNormals[k]);
    h.insert(h.end(), {dp*cubeobject.vertexNormals[k][0],
                       dp*cubeobject.vertexNormals[k][1], dp*cubeobject.vertexNormals[k][2]});

    h[0] -= L[0];h[1] -= L[1]; h[2] -= L[2];


    for (int j=0; j<=2; ++j){
        o.push_back(cubeobject.vertices[k][j]-op[j]);
    }

    dp=dot_product(o, h);
    h.clear();
    o.clear();
    double power= pow(dp,lightScene[2]->p);
    aux = lightScene[2]->intensity*cubeobject.all_mat[cubeobject.curr_mat].ke[i]*power ;
    if(aux<0){aux=0;}

    I +=aux;

    L.clear();
    //qDebug() <<I;

    if(I>255){I=255;}
    if(I<0){I=0;}

    //rasterColor[i].append(I);

    return I;
    // }
}


std::vector<double> lightVertex(CubeObject cubeobject, std::vector<double> L, std::vector<double> N, std::vector<double> O, std::vector<lights*> lightScene){

    std::vector<double> I_v;
    double I;
    double dp=0;
    double Ia=100;
    double aux=0;

    std::vector<double> h;

    for(int i=0; i<=2; ++i){

        //AMBIENT
        I= cubeobject.all_mat[cubeobject.curr_mat].Ia*cubeobject.all_mat[cubeobject.curr_mat].ka[i];

//        //DIFFUSE
//        dp=dot_product(L, N);

//        I += lightScene[0]->color[i]*(lightScene[0]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp) ;
//        I += abs(lightScene[1]->color[i]*(lightScene[1]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp)) ;

//        //SPECULAR
//        h.insert(h.end(), {2*dp*N[0], 2*dp*N[1], 2*dp*N[2]});
//        h[0] -= L[0];h[1] -= L[1]; h[2] -= L[2];


//        double power= pow(dot_product(O, h),lightScene[2]->p);
//        aux = lightScene[2]->intensity*cubeobject.all_mat[cubeobject.curr_mat].ke[i]*power ;
//        if(aux<0){aux=0;}

        I +=aux;
        if(I>255){I=255;}
        if(I<0){I=0;}

        I_v.push_back(I);
        h.clear();

    }

    return I_v;
}






















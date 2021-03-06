#include "raster.h"
#include"camProjection.h"
#include"functions.h"
#include"shading.h"
raster::raster()
{
    //colorBuffer[0][0][0]=0;
}

void raster::pipeline(CubeObject cubeobject, std::vector<lights*> lightScene, bool ortho, bool phongBool, bool cam1bool)
{
    phong = phongBool;
    CamProjection camProj;
    std::vector<std::vector<double> > camPos1= {{1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,1,-35},
                                                {0,0,0,1}};

    std::vector<std::vector<double> > camPos2= {{0,0,1,  -20},
                                                {0,1, 0,  5},
                                                {-1,0,0 ,-25},
                                                {0,0,0,1}};
    if(cam1bool){
        camProj.camMarco = camPos1;
    }else{
        camProj.camMarco = camPos2;
    }

    camProj.projectPoint(cubeobject, ortho, 400, 0, 400, 0);


    rasterPoint.append(camProj.rasterPoint);
    rasterZ.append(camProj.rasterZ);


    camProj.rasterPoint.clear();


    double test;
    test=  calcLightVertex(cubeobject, lightScene, 7, 0);
    ///////////////////////////////////

         ///FIRST VERTEX COLOR
        for(int k=0; k<cubeobject.vertices.size(); ++k){
            for(int i =0; i<=2; ++i){
                rasterColor[i].append(calcLightVertex(cubeobject, lightScene, k, i));
               // rasterColor[i].append(255);
            }
        }

    ///FIRST VERTEX LIGHTPOS
        for(int k=0; k<cubeobject.vertices.size(); ++k){
            for(int j =0; j<=2; ++j){
                rasterLightPos[j].append(cubeobject.vertices[k][j] - lightScene[0]->lightPos[j]);
            }
        }


        ///FIRST VERTEX NORMALS
        for(int i =0; i<=cubeobject.vertexNormals.size()-1; ++i){
            for(int j =0; j<=2; ++j){
                rasterNormal[j].append(cubeobject.vertexNormals[i][j]);
            }
        }

        ///FIRST VERTEX OBS
         std::vector<double> op= {15,0,15};
         for(int k=0; k<cubeobject.vertices.size(); ++k){
             for(int j =0; j<=2; ++j){
                 rasterObs[j].append(cubeobject.vertices[k][j] - op[j]);
             }
         }



   this->drawFaces(cubeobject);

         //PHONG
         if(phong){
             for (int i=0; i<=2; i++){
                 rasterColor[i].clear();
             }

             std::vector<double> I;
             std::vector<double> O;
             std::vector<double> L;
             std::vector<double> N;


             for(int k=0; k<rasterNormal[0].size(); ++k){

                 for(int j=0; j<=2; ++j){
                     L.push_back(rasterLightPos[j][k]);
                     N.push_back(rasterNormal[j][k]);
                     O.push_back(rasterObs[j][k]);
                 }


                 I= lightVertex(cubeobject, L, N, O, lightScene);
                 for(int i=0; i<=2;++i){
                     rasterColor[i].append(I[i]);
                 }

                 I.clear();
                 L.clear();
                 N.clear();
                 O.clear();
             }
         }

         for (int i=0; i<=2; i++){
             rasterNormal[i].clear();
             rasterLightPos[i].clear();
             rasterObs[i].clear();
         }



}



void raster::drawFaces(CubeObject cubeobject){

    for(int i=0; i< cubeobject.facesIdx.size(); ++i){
        fillCubeFace(cubeobject.facesIdx[i][0],cubeobject.facesIdx[i][1],
                    cubeobject.facesIdx[i][2], cubeobject.facesIdx[i][3]);
    }


}

void raster::fillCubeFace(int v0, int v1, int v2, int v3){

    scanLine(v0,v1);
    scanLine(v1,v2);
    scanLine(v2,v3);
    scanLine(v3,v0);

    //SCAN CONVERSION
    rasterPoint.append(scanConversion(yBuffer));
    rasterZ.append(horInterpolation(yBuffer, zBuffer));





    //INTERPOLAR
    for(int i=0; i<=2; ++i){
       if(!phong){
            rasterColor[i].append(horInterpolation(yBuffer, colorBuffer[i]));
       }
       rasterNormal[i].append(horInterpolation(yBuffer, normalBuffer[i]));
       rasterLightPos[i].append(horInterpolation(yBuffer, lightPosBuffer[i]));
       rasterObs[i].append(horInterpolation(yBuffer, obsBuffer[i]));

    }



    //CLEAN BUFFER
    for(int y=0;y<400;++y){
        for(int i=0; i<=1; ++i){
            yBuffer[y][i]=0;

            for(int j=0;j<2;++j){
                colorBuffer[j][y][i]=0;
                normalBuffer[j][y][i]=0;
                lightPosBuffer[j][y][i]=0;
                obsBuffer[j][y][i]=0;
            }
     }
    }



}




void raster::scanLine(int v1, int v2)
{
    int up=0;
    if(rasterPoint[v1][1]>rasterPoint[v2][1])//cambiar coordenadas
    {
        up=1;
        double temp=v1;
        v1=v2; v2=temp;
    }

    //SCANLINE
    double x1=rasterPoint[v1][0], y1=rasterPoint[v1][1], z1=rasterZ[v1];
    double x2=rasterPoint[v2][0], y2=rasterPoint[v2][1], z2=rasterZ[v2];


    double dx= x2-x1;
    double dy= y2-y1;
    double m = dx/dy;

    double yd=y1+1 - y1;
    double x= x1+ m*yd ;

    int y1p=int(y1+1);
    int y2p= int(y2);

    double deltaZ = (z2-z1)/dy;
    double z=z1;

    //FILL BUFFER
    double deltaI[3];
    double deltaN[3];
    double deltaL[3];
    double deltaO[3];

    for(int i=0; i<=2; i++){
        deltaI[i]=(rasterColor[i][v2]-rasterColor[i][v1])/(y2-y1);
        deltaN[i]=(rasterNormal[i][v2]-rasterNormal[i][v1])/(dy);
        deltaL[i]=(rasterLightPos[i][v2]-rasterLightPos[i][v1])/(dy);
        deltaO[i]=(rasterObs[i][v2]-rasterObs[i][v1])/(dy);
    }

    double I[3]={rasterColor[0][v1], rasterColor[1][v1], rasterColor[2][v1]};
    double N[3]= {rasterNormal[0][v1], rasterNormal[1][v1], rasterNormal[2][v1]};
    double L[3]= {rasterLightPos[0][v1], rasterLightPos[1][v1], rasterLightPos[2][v1]};
    double O[3]= {rasterObs[0][v1], rasterObs[1][v1], rasterObs[2][v1]};


    //INTERPOLACION
    for(int y=y1p ;y<=y2p ;y++){
           yBuffer[y][up]=int(x);
           x+=m;
            zBuffer[y][up]=z;
            z+=deltaZ;

            for(int i=0; i<=2; i++){
                colorBuffer[i][y][up]=I[i];
                I[i] +=deltaI[i];

                normalBuffer[i][y][up]=N[i];
                N[i] +=deltaN[i];

                lightPosBuffer[i][y][up]=L[i];
                L[i] +=deltaL[i];

                obsBuffer[i][y][up]=O[i];
                O[i] +=deltaO[i];
            }

    }
}














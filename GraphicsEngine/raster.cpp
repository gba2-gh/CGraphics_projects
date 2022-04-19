#include "raster.h"
#include"camProjection.h"
#include"functions.h"
#include"shading.h"
raster::raster()
{
    //colorBuffer[0][0][0]=0;
}

void raster::pipeline(CubeObject cubeobject, std::vector<lights*> lightScene, bool ortho, bool phongBool, int camSelect, int shaderS, QImage texture_img)
{
    phong = phongBool;
    this->shaderSel= shaderS;
    CamProjection camProj;
    std::vector<std::vector<double> > camPos1= {{1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,1,15},
                                                {0,0,0,1}};

    std::vector<std::vector<double> > camPos2=
                        {{0.5,  0,    -0.87,         0},
                         {0,  1,    0,               0},
                          {
                             0.87,     0,    0.5 ,   -15},
                           {0,0,0,                  1}};

    std::vector<std::vector<double> > camPos3= {{0,0,1,  -10},
                                                {0,1, 0,  0},
                                                {-1,0,0 ,-15},
                                                {0,0,0,1}};




    if(camSelect==0){
        camProj.camMarco = camPos1;
    }else if(camSelect==1){
        camProj.camMarco = camPos2;
    }else if(camSelect==2){
        camProj.camMarco = camPos3;
    }

    camProj.projectPoint(cubeobject, ortho, 400, 0, -400, 0);


    rasterPoint.append(camProj.rasterPoint);
    rasterZ.append(camProj.rasterZ);
    //qDebug() <<"rasterZ : " << rasterZ;

    camProj.rasterPoint.clear();
    camProj.rasterZ.clear();

    ///////////////////////////////////



        if(shaderSel != 0){
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
        }

        double I;
        std::vector<double> O;
        std::vector<double> L;
        std::vector<double> N;


        ///FIRST VERTEX COLOR
       for(int k=0; k<cubeobject.vertices.size(); ++k){
           //for(int i =0; i<=2; ++i){
               if(shaderSel == 0){

                   for(int i=0; i<=2;++i){
                       rasterColor[i].append(255);
                   }

               }else if(shaderSel==3){


                   for(int i=0; i<=2;++i){
                       rasterColor[i].append(255);
                   }

               }
              else{
               //rasterColor[i].append(calcLightVertex(cubeobject, lightScene, k, i));

                   for(int j=0; j<=2; ++j){
                       L.push_back(rasterLightPos[j][k]);
                       N.push_back(rasterNormal[j][k]);
                       O.push_back(rasterObs[j][k]);
                   }
                   //I=lightVertex(cubeobject, L, N, O, lightScene);


                   for(int i=0; i<=2;++i){
                      // rasterColor[i].append(I[i]);
                       double dp=dot_product(L, N);
                       I=cubeobject.all_mat[cubeobject.curr_mat].Ia*cubeobject.all_mat[cubeobject.curr_mat].ka[i];
                       I += lightScene[0]->color[i]*(lightScene[0]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp) ;
                       I += abs(lightScene[1]->color[i]*(lightScene[1]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp)) ;

                       if(I>255){I=255;}
                       if(I<0){I=0;}
                        rasterColor[i].append(I);
                   }

                   //I.clear();
                   I=0;
                   L.clear();
                   N.clear();
                   O.clear();
               }

         //  }
       }

       QList<double> texel;
       for(int k=0; k<cubeobject.vertex_uvCoord.size(); ++k){
           //texel.append(cubeobject.vertex_uvCoord[k][0]);
           //texel.append(cubeobject.vertex_uvCoord[k][1]);
           //rasterUV.append(texel);
           //texel.clear();
           rasterUV[0].append(cubeobject.vertex_uvCoord[k][0]);
           rasterUV[1].append(cubeobject.vertex_uvCoord[k][1]);
       }

    ///INTRPOLACION
    ///

   this->fillCubeFace(cubeobject);

if(shaderSel==3){
       //for (int i=0; i<=2; i++){
        //   rasterColor[i].clear();
       //}
       QColor color_texel;

       for(int i=0; i< rasterUV[0].size(); i++){

           int x,y;

           x= floor((texture_img.width()-1)*(rasterUV[0][i]) );
           y= floor((texture_img.height()-1)*(1 -rasterUV[1][i]) );


           color_texel= texture_img.pixelColor(x,y);


           qDebug() << "x: " << x << "y: " << y;
           qDebug() << rasterUV[0][i];

           if(color_texel.isValid()){
               rasterColor[0][i] = color_texel.red();
               rasterColor[1][i] = color_texel.green();
               rasterColor[2][i] = color_texel.blue();

           }



          // for (int i=0; i<=2; i++){
           // rasterColor[i].append(I[i]);
           //}
       }
   }


         //PHONG
    if(shaderSel==2){
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

                /////i

                 std::vector<double> I_v;
                 double I[3];
                 double dp=0;
                 double Ia=100;
                 double aux=0;

                 std::vector<double> h;

                 for(int i=0; i<=2; ++i){

                     //AMBIENT
                     I[i]= cubeobject.all_mat[cubeobject.curr_mat].Ia*cubeobject.all_mat[cubeobject.curr_mat].ka[i];

                     //DIFFUSE
                     dp=dot_product(L, N);

                     I[i] += lightScene[0]->color[i]*(lightScene[0]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp) ;
                     I[i] += abs(lightScene[1]->color[i]*(lightScene[1]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp)) ;

//                     //SPECULAR
//                     h.insert(h.end(), {2*dp*N[0], 2*dp*N[1], 2*dp*N[2]});
//                     h[0] -= L[0];h[1] -= L[1]; h[2] -= L[2];


//                     double power= pow(dot_product(O, h),lightScene[2]->p);
//                     aux = lightScene[2]->intensity*cubeobject.all_mat[cubeobject.curr_mat].ke[i]*power ;
//                     if(aux<0){aux=0;}

//                     I +=aux;
                     if(I[i]>255){I[i]=255;}
                     if(I[i]<0){I[i]=0;}

                     //I_v.push_back(I[i]);
                     h.clear();


                    }







                 //I= lightVertex(cubeobject, L, N, O, lightScene);

                 for(int i=0; i<=2;++i){
                     rasterColor[i].append(I[i]);
                 }

                 //I.clear();
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
     rasterUV[0].clear();
     rasterUV[1].clear();



}



void raster::fillCubeFace(CubeObject cubeobject){

    for(int i=0; i< cubeobject.facesIdx.size(); ++i){
        for(int j=0; j<cubeobject.facesIdx[i].size()-1; ++j){
          scanLine(cubeobject.facesIdx[i][j],cubeobject.facesIdx[i][j+1])   ;
        }
       scanLine(cubeobject.facesIdx[i][cubeobject.facesIdx[i].size()-1], cubeobject.facesIdx[i][0] );


    //SCAN CONVERSION
    rasterPoint.append(scanConversion(yBuffer));
    rasterZ.append(horInterpolation(yBuffer, zBuffer));


    rasterUV[0].append(horInterpolation(yBuffer, uvBuffer[0]));
    rasterUV[1].append(horInterpolation(yBuffer, uvBuffer[1]));





    //INTERPOLAR
    for(int i=0; i<=2; ++i){
       if(shaderSel==2){
           rasterNormal[i].append(horInterpolation(yBuffer, normalBuffer[i]));
           rasterLightPos[i].append(horInterpolation(yBuffer, lightPosBuffer[i]));
           rasterObs[i].append(horInterpolation(yBuffer, obsBuffer[i]));

       }else{
       rasterColor[i].append(horInterpolation(yBuffer, colorBuffer[i]));
       }

    }



    //CLEAN BUFFER
    for(int y=0;y<400;++y){
        for(int i=0; i<=1; ++i){
            yBuffer[y][i]=0;
            uvBuffer[0][y][i]=0;
            uvBuffer[1][y][i]=0;

            for(int j=0;j<2;++j){
                colorBuffer[j][y][i]=0;
                normalBuffer[j][y][i]=0;
                lightPosBuffer[j][y][i]=0;
                obsBuffer[j][y][i]=0;
            }
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


    ///TEXTTURE
    ///

    double u1= rasterUV[0][v1], u2=rasterUV[0][v2];
    double vC1= rasterUV[1][v1], vC2=rasterUV[1][v2];

    double du= u2-u1;
    double dv= vC2-vC1;
    double duv=du/dv;


    //double vd= vC1+1 - vC1;
    double u= u1+ duv/dy ;

    double v=int(vC1+1);
    int vC2p= int(vC2);


    double deltaU=0;
      if(dx !=0){
            deltaU= du/dy;}
    double deltaV= dv/dy;

    //FILL BUFFER
        double deltaI[3];
        double deltaN[3];
        double deltaL[3];
        double deltaO[3];

        for(int i=0; i<=2; i++){
            deltaI[i]=(rasterColor[i][v2]-rasterColor[i][v1])/(y2-y1);
            if(shaderSel==2){
            deltaN[i]=(rasterNormal[i][v2]-rasterNormal[i][v1])/(dy);
            deltaL[i]=(rasterLightPos[i][v2]-rasterLightPos[i][v1])/(dy);
            deltaO[i]=(rasterObs[i][v2]-rasterObs[i][v1])/(dy);
            }
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

            uvBuffer[0][y][up]=u;
            uvBuffer[1][y][up]=v;
            u=u+deltaU;
            v=v-deltaV;


                for(int i=0; i<=2; i++){
                    colorBuffer[i][y][up]=I[i];
                    I[i] +=deltaI[i];

                    if(shaderSel == 2){
                    normalBuffer[i][y][up]=N[i];
                    N[i] +=deltaN[i];

                    lightPosBuffer[i][y][up]=L[i];
                    L[i] +=deltaL[i];

                    obsBuffer[i][y][up]=O[i];
                    O[i] +=deltaO[i];
                    }
                }


    }
}












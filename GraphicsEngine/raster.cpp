#include "raster.h"
#include"camProjection.h"
#include"functions.h"
#include"shading.h"
raster::raster()
{
    //colorBuffer[0][0][0]=0;
}

void raster::pipeline(CubeObject cubeobject, std::vector<lights*> lightScene, bool ortho, bool phongBool, int camSelect, int shaderS, QImage texture_img_in)
{
    texture_img = texture_img_in;
    phong = phongBool;
    this->shaderSel= shaderS;
    CamProjection camProj;
    std::vector<std::vector<double> > camPos1= {{1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,1,-1.7},
                                                {0,0,0,1}};

    std::vector<std::vector<double> > camPos3= {{-1,0,0,0},
                                                {0,1,0,0},
                                                {0,0,-1,-1.5},
                                                {0,0,0,1}};

    std::vector<std::vector<double> > camPos2=
                        {{1,  0,    0,         0},
                         {0,  0,    -1,               0},
                          {0,     1,    0 ,   -1.2},
                           {0,0,0,                  1}};
//    std::vector<std::vector<double> > camPos2= {{0,0,-1,  0},
//                                                {0,1, 0,  0},
//                                                {1,0,0 ,-15},
//                                                {0,0,0,1}};



//    std::vector<std::vector<double> > camPos3= {{0,0,1,  0},
//                                                {0,1, 0,  0},
//                                                {-1,0,0 ,-30},
//                                                {0,0,0,1}};




    if(camSelect==0){
        camProj.camMarco = camPos1;
    }else if(camSelect==1){
        camProj.camMarco = camPos2;
    }else if(camSelect==2){
        camProj.camMarco = camPos3;
    }

    camProj.projectPoint(cubeobject, ortho, 400, 0, 0, 400);


    rasterPoint.append(camProj.rasterPoint);
    rasterZ.append(camProj.rasterZ);
    depthZ.append(camProj.depthZ);

    proy[0].append(camProj.proy[0]);
    proy[1].append(camProj.proy[1]);
    proy[2].append(camProj.proy[2]);

    //qDebug() <<"rasterZ : " << rasterZ;

    camProj.rasterPoint.clear();
    camProj.rasterZ.clear();

    ///////////////////////////////////



        if(shaderSel != 0){
            ///FIRST VERTEX LIGHTPOS
            for(int k=0; k<cubeobject.vertices.size(); ++k){
                for(int j =0; j<=2; ++j){
                    rasterLightPos[j].append(cubeobject.vertices[k][j] - lightScene[0]->lightPos[j]);
                    rasterLightPos2[j].append(cubeobject.vertices[k][j] - lightScene[1]->lightPos[j]);
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
        std::vector<double> L2;
        std::vector<double> N;


        ///FIRST VERTEX COLOR
       for(int k=0; k<cubeobject.vertices.size(); ++k){
               if(shaderSel == 0){
                   for(int i=0; i<=2;++i){
                       rasterColor[i].append(255);
                   }
               }

              else{
               //rasterColor[i].append(calcLightVertex(cubeobject, lightScene, k, i));

                   for(int j=0; j<=2; ++j){
                       L.push_back(rasterLightPos[j][k]);
                       L2.push_back(rasterLightPos2[j][k]);
                       N.push_back(rasterNormal[j][k]);
                       O.push_back(rasterObs[j][k]);
                   }
                   //I=lightVertex(cubeobject, L, N, O, lightScene);


                   for(int i=0; i<=2;++i){
                      // rasterColor[i].append(I[i]);
                       double dp=dot_product(L, N);
                       I=cubeobject.all_mat[cubeobject.curr_mat].Ia*cubeobject.all_mat[cubeobject.curr_mat].ka[i];
                       I += lightScene[0]->color[i]*(lightScene[0]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp) ;

                       dp=dot_product(L2, N);
                       I += lightScene[1]->color[i]*(lightScene[1]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp) ;

                       if(I>255){I=255;}
                       if(I<0){I=0;}
                        rasterColor[i].append(I);
                   }

                   //I.clear();
                   I=0;
                   L.clear();
                   L2.clear();
                   N.clear();
                   O.clear();
               }

       }


        //TEXTURE
       for(int k=0; k<cubeobject.vertex_uvCoord.size(); ++k){

           rasterUV[0].append(cubeobject.vertex_uvCoord[k][0]);
           rasterUV[1].append(cubeobject.vertex_uvCoord[k][1]);
       }



    ///INTERPOLACION
    ///

   this->fillCubeFace(cubeobject);


         //PHONG
    if(shaderSel==2){
             for (int i=0; i<=2; i++){
                 rasterColorT[i].clear();
             }

             std::vector<double> O;
             std::vector<double> L;
             std::vector<double> L2;
             std::vector<double> N;


             for(int k=0; k<rasterNormal[0].size(); ++k){

                 for(int j=0; j<=2; ++j){
                     L.push_back(rasterLightPos[j][k]);
                     L2.push_back(rasterLightPos2[j][k]);
                     N.push_back(rasterNormal[j][k]);
                     O.push_back(rasterObs[j][k]);
                 }

                /////i

                 double I;
                 double dp=0;
                 double aux=0;

                 std::vector<double> h;

                 for(int i=0; i<=2; ++i){

                     //AMBIENT
                     I= cubeobject.all_mat[cubeobject.curr_mat].Ia*cubeobject.all_mat[cubeobject.curr_mat].ka[i];

                     //DIFFUSE
                     dp=dot_product(L, N);

                     I += lightScene[0]->color[i]*(lightScene[0]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp) ;
//                     if(aux<0){aux=0;}
//                          I +=aux;
                     dp=dot_product(L2, N);
                     I += lightScene[1]->color[i]*(lightScene[1]->intensity*cubeobject.all_mat[cubeobject.curr_mat].kd[i]*dp) ;
//                     if(aux<0){aux=0;}
//                          I +=aux;
                     //SPECULAR
//                     h.insert(h.end(), {2*dp*N[0], 2*dp*N[1], 2*dp*N[2]});
//                     h[0] -= L[0];h[1] -= L[1]; h[2] -= L[2];
//                     double power= pow(dot_product(O, h),lightScene[2]->p);
//                     aux = lightScene[2]->intensity*cubeobject.all_mat[cubeobject.curr_mat].ke[i]*power ;

//                     if(aux<0){aux=0;}
//                     I[i] +=aux;


                     if(I>255){I=255;}
                     if(I<0){I=0;}

                     h.clear();

                    rasterColorT[i].append(I);

                    }

                 L.clear();
                 L2.clear();
                 N.clear();
                 O.clear();

             }
     } //end if shadersel

    //CLEAN
     for (int i=0; i<=2; i++){
         rasterNormal[i].clear();
         rasterLightPos[i].clear();
         rasterLightPos2[i].clear();
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

           scanLine(cubeobject.facesIdx[i][cubeobject.facesIdx[i].size()-1], cubeobject.facesIdx[i][0]);


        //SCAN CONVERSION
        rasterPoint.append(scanConversion(yBuffer));
        rasterZ.append(horInterpolation(yBuffer, zBuffer));


        //rasterUV[0].append(horInterpolation(yBuffer, uvBuffer[0]));
        //rasterUV[1].append(horInterpolation(yBuffer, uvBuffer[1]));

        int xmin=0,xmax=0;
        double imin=0, imin2=0;
        double imax=0, imax2=0;

        double deltaI, deltaI2, z_act;

        for(int y=0;y<400;++y){

               if(yBuffer[y][0]<yBuffer[y][1]){
                        xmin= yBuffer[y][0];
                        xmax= yBuffer[y][1];

                        imin= uvBuffer[0][y][0];
                        imax= uvBuffer[0][y][1];
                        imin2= uvBuffer[1][y][0];
                        imax2= uvBuffer[1][y][1];

                        z_act= zBuffer[y][0];
                }
                else{
                        xmin= yBuffer[y][1];
                        xmax= yBuffer[y][0];
                        imin= uvBuffer[0][y][1];
                        imax= uvBuffer[0][y][0];
                        imin= uvBuffer[1][y][1];
                        imax= uvBuffer[1][y][0];

                        z_act= zBuffer[y][1];
                }

               deltaI=(imin-imax)/(xmin-xmax);
               deltaI2=(imin2-imax2)/(xmin-xmax);

               for(int x=xmin; x<=xmax; ++x){

                   rasterUV[0].append(imin);
                   imin += deltaI;

                   rasterUV[1].append(imin2);
                   imin2 += deltaI2;
                }

        }


        //INTERPOLAR
        for(int c=0; c<=2; ++c){
           if(shaderSel==2){
               rasterNormal[c].append(horInterpolation(yBuffer, normalBuffer[c]));
               rasterLightPos[c].append(horInterpolation(yBuffer, lightPosBuffer[c]));
               rasterLightPos2[c].append(horInterpolation(yBuffer, lightPos2Buffer[c]));
               rasterObs[c].append(horInterpolation(yBuffer, obsBuffer[c]));

           }else{
           rasterColor[c].append(horInterpolation(yBuffer, colorBuffer[c]));
           }

        }



        double color_sizeT= rasterColorT[0].size();
        int uv_size= rasterUV[0].size()/2;

        ////////TEST
        ///
        bool facein=false;
        for(int l=0; l< cubeobject.textureFaces.size(); ++l){
             if(cubeobject.textureFaces[l]==i){

                    facein=true;
        }}

        if(i>0){
            for(int l=0; l<cubeobject.vertex_uvCoord.size(); ++l){
                rasterUV[0].removeFirst();
                rasterUV[1].removeFirst();
            }

        }

        if(shaderSel==3 && facein){

               QColor color_texel;

               for(int k=0; k< rasterUV[0].size(); k++){

                   int x,y;
                   x= floor((texture_img.width()-1)*(rasterUV[0][k] ));
                   y= floor((texture_img.height()-1)*(1 -rasterUV[1][k]) );


                   color_texel= texture_img.pixelColor(x,y);


                   ////reemplazo


                   //           ////STICKER
                   ////           if(color_texel.isValid()){
                   ////               rasterColor[0][i] -= color_texel.red()/10;
                   ////               rasterColor[1][i] -= color_texel.green()/10;
                   ////               rasterColor[2][i] -= color_texel.blue()/10;
                   ////           }

                       rasterColorT[0].append(color_texel.red());
                       rasterColorT[1].append(color_texel.green());
                       rasterColorT[2].append(color_texel.blue());

               }  //end for rasteruv
            }
            else{//else shader==3
                for(int c=color_sizeT; c< rasterColor[0].size(); ++c){
                rasterColorT[0].append(rasterColor[0][c]);
                rasterColorT[1].append(rasterColor[1][c]);
                rasterColorT[2].append(rasterColor[2][c]);
                }
            }
        rasterUV[0].clear();
        rasterUV[1].clear();


        for(int k=0; k<cubeobject.vertex_uvCoord.size(); ++k){

            rasterUV[0].append(cubeobject.vertex_uvCoord[k][0]);
            rasterUV[1].append(cubeobject.vertex_uvCoord[k][1]);
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
                    lightPos2Buffer[j][y][i]=0;
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
        double temporal=v1;
        v1=v2; v2=temporal;
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


    double z1d= depthZ[v1], z2d= depthZ[v2];
    double deltaZd = (z2d-z1d)/dy;
    double zd=z1d;


 ///TEXTTURE
    ///

    double u1= rasterUV[0][v1], u2=rasterUV[0][v2];
    double vC1= rasterUV[1][v1], vC2=rasterUV[1][v2];

    double du= u2-u1;
    double dv= vC2-vC1;
    double duv=du/dv;


    //double vd= vC1+1 - vC1;
    double u= u1 ;

    double v= vC1;
    //int vC2p= int(vC2);


    double deltaU=0;

            deltaU= du/dy;
    double deltaV= dv/dy;

    //FILL BUFFER
        double deltaI[3];
        double deltaN[3];
        double deltaL[3];
        double deltaL2[3];
        double deltaO[3];
    if(shaderSel == 2){
        for(int i=0; i<=2; i++){
            deltaI[i]=(rasterColor[i][v2]-rasterColor[i][v1])/(y2-y1);
            if(shaderSel==2){
            deltaN[i]=(rasterNormal[i][v2]-rasterNormal[i][v1])/(dy);
            deltaL[i]=(rasterLightPos[i][v2]-rasterLightPos[i][v1])/(dy);
            deltaL2[i]=(rasterLightPos2[i][v2]-rasterLightPos2[i][v1])/(dy);
            deltaO[i]=(rasterObs[i][v2]-rasterObs[i][v1])/(dy);
            }
        }

 }

        double I[3]={rasterColor[0][v1], rasterColor[1][v1], rasterColor[2][v1]};
        double N[3]= {rasterNormal[0][v1], rasterNormal[1][v1], rasterNormal[2][v1]};
        double L[3]= {rasterLightPos[0][v1], rasterLightPos[1][v1], rasterLightPos[2][v1]};
        double L2[3]= {rasterLightPos2[0][v1], rasterLightPos2[1][v1], rasterLightPos2[2][v1]};
        double O[3]= {rasterObs[0][v1], rasterObs[1][v1], rasterObs[2][v1]};


    //INTERPOLACION
    for(int y=y1p ;y<=y2p ;y++){
           yBuffer[y][up]=int(x);
           x+=m;
            zBuffer[y][up]=z;
            z+=deltaZ;


            if(shaderSel==3){
                //depthBuffer[y][up] = zd;
                zd += deltaZd;
                //double lambda = (x - proy[v1][0])/(proy[0][v2]-proy[0][v1]);
                double lambda =  (x-x1)/(x2-x1);
                //double pz= 1/ proy[v1][2] * (1-lambda) + 1/proy[v2][3] * (lambda);
                double pz = (1/x1) * (1-lambda) + (1/x2)*(lambda);


                depthBuffer[y][up]= 1/pz;
                uvBuffer[0][y][up]=u;
                uvBuffer[1][y][up]=v;

                u=u+deltaU;
                v=v+deltaV;
            }


                for(int i=0; i<=2; i++){
                    colorBuffer[i][y][up]=I[i];
                    I[i] +=deltaI[i];

                    if(shaderSel == 2){
                    normalBuffer[i][y][up]=N[i];
                   N[i] +=deltaN[i];

                    lightPosBuffer[i][y][up]=L[i];
                    L[i] +=deltaL[i];

                    lightPos2Buffer[i][y][up]=L2[i];
                    L2[i] +=deltaL2[i];


                    obsBuffer[i][y][up]=O[i];
                    O[i] +=deltaO[i];
                    }
                }


    }
}













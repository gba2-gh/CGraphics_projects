#include "camProjection.h"
#include<QDebug>
#include<vector>
#include"cubeobject.h"
#include<cmath>
#include"shading.h"



void   CamProjection::projectPoint(CubeObject cubeobject, std::vector<lights> lightScene, bool ortho,  double xdmax ,double xdmin,
                                                                 double ydmax, double ydmin, int cam, bool phongBool)
{
    for(int i=0; i<cubeobject.vertices.size();++i){
        vertices.insert(vertices.end(), cubeobject.vertices[i]);
    }

    std::vector<std::vector<double> > vProy;
 for(int k=0; k<vertices.size();++k){

    double point[4];
    for(int j=0;j<4;++j){
          point[j]=vertices[k][j];

                   }

    double p[4];

    double suma;

    for(int i=0; i<4; ++i){
        for(int j=0;j<4;++j){
                suma += camMarco[i][j]*point[j];
        }
        p[i]=suma;
        suma=0;
    }


    //Proyectar
    int f=1;
    std::vector<double > vp;
    if(ortho){//Ortogonal
        vp.push_back(p[0]);
        vp.push_back(p[1]);
    }
    else{//Perspectiva
        vp.push_back(p[0]*f/-p[2]);
        vp.push_back(p[1]*f/-p[2]);
    }
    vProy.push_back(vp);
    vp.clear();
 }

    //qDebug()<<vProy;

//RASTERIZAR
    int canvSc=1;
    if(ortho){canvSc=20;}
    QList<int> p_dev;
    double xpmax=2*canvSc, xpmin=-2*canvSc;
    double ypmax=2*canvSc, ypmin=-2*canvSc;
    double sx= ((xdmax-xdmin)/(xpmax-xpmin)), sy= ((ydmin-ydmax)/(ypmax-ypmin));
    double ox = sx*(-xpmin)+ xdmin, oy = sx*(-ypmin)+ ydmin;



    for(int k=0; k<vProy.size(); ++k){

        p_dev.append(sx*vProy[k][0] + ox);
        p_dev.append(sy*vProy[k][1] + oy);

        rasterPoint.append(p_dev);
        p_dev.clear();
        //rasterPoint.append(yd);  
       }

     vProy.clear();


    ///FIRST VERTEX LIGHTPOS
     for(int k=0; k<cubeobject.vertices.size(); ++k){
         for(int j =0; j<=2; ++j){
             rasterLightPos[j].append(cubeobject.vertices[k][j] - lightScene[0].lightPos[j]);
         }

     }

     ///FRIST VERTEX COLOR
    for(int k=0; k<rasterPoint.size(); ++k){
        for(int i =0; i<=2; ++i){
            rasterColor[i].append(calcLightVertex(cubeobject, lightScene, k, i));
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



    drawFaces(cam);


    if(phongBool){
    for (int i=0; i<=2; i++){
            rasterColor[i].clear();
       }


    double I=0;
    double dp=0;
    double Ia=100;
    double aux=0;

    std::vector<double> O;
    std::vector<double> h;

    std::vector<double> L;
    std::vector<double> N;

    for(int k=0; k<rasterNormal[0].size(); ++k){

        for(int j=0; j<=2; ++j){
            L.push_back(rasterLightPos[j][k]);
            N.push_back(rasterNormal[j][k]);
            O.push_back(rasterObs[j][k]);
        }
        dp=dot_product(L, N);

        for(int i=0; i<=2; ++i){

            //AMBIENT
            I= Ia*cubeobject.ka[i];

            //DIFFUSE
            I += lightScene[0].color[i]*(lightScene[0].intensity*cubeobject.kd[i]*dp) ;
            I += lightScene[1].color[i]*(lightScene[1].intensity*cubeobject.kd[i]*dp) ;

            //SPECULAR
            h.insert(h.end(), {2*dp*N[0], 2*dp*N[1], 2*dp*N[2]});
            h[0] -= L[0];h[1] -= L[1]; h[2] -= L[2];


            double power= pow(dot_product(O, h),lightScene[2].p);
            aux = lightScene[2].intensity*cubeobject.ke[i]*power ;
            if(aux<0){aux=0;}

            I +=aux;
            if(I>255){I=255;}
            if(I<0){I=0;}


            rasterColor[i].append(I);
        }

        h.clear();
        L.clear();
        N.clear();
        O.clear();
    }
    }



    for (int i=0; i<=2; i++){
            rasterNormal[i].clear();
            rasterLightPos[i].clear();
       }

    vertices.clear();
}



void CamProjection::drawFaces(int cam){
    fillCubeFace(0,1,2,3);
    fillCubeFace(2,3,4,5);
    fillCubeFace(1,2,5,6);
    fillCubeFace(0,3,4,7);


    if(cam==1){
    fillCubeFace(0,1,6,7);
    fillCubeFace(4,5,6,7);

    }
    else{
        fillCubeFace(4,5,6,7);
        fillCubeFace(0,1,6,7);

    }
}

void CamProjection::fillCubeFace(int v0, int v1, int v2, int v3){

        scanFillPoly(v0,v1);
        scanFillPoly(v1,v2);
        scanFillPoly(v2,v3);
        scanFillPoly(v3,v0);

//    qDebug() << "BUFFER 0/////////////////////////////////";
//    for(int y=0;y<400;y++){
//        if(yBuffer[y][0] != 0){
//            qDebug() << "ybuffer0"<< y <<","<<yBuffer[y][0];

//            qDebug() << "colorR:"<< y <<","<< colorBuffer[0][y][0];

//        }

//    }

//    qDebug() << "BUFFER 1/////////////////////////////////";
//    for(int y=0;y<400;y++){
//        if(yBuffer[y][1] != 0){
//            qDebug() << "ybuffer1"<< y <<","<<yBuffer[y][1];
//            qDebug() << "colorR:"<< y <<","<< colorBuffer[0][y][0];ç
//        }

//    }

    int xmin=0,xmax=0;
    double imin[3]={0};
    double imax[3]={0};
    double nmin[3]={0};
    double nmax[3]={0};
    double lmin[3]={0};
    double lmax[3]={0};
    double omin[3]={0};
    double omax[3]={0};

    double deltaI[3];
    double deltaN[3];
    double deltaL[3];
    double deltaO[3];
    QList<int> p_fill;

      for(int y=0;y<400;++y){

                if(yBuffer[y][0]<yBuffer[y][1]){
                    xmin= yBuffer[y][0];
                    xmax= yBuffer[y][1];

                    for(int i=0; i<=2; ++i){
                        imin[i]= colorBuffer[i][y][0];
                        imax[i]= colorBuffer[i][y][1];
                        }
                    for(int i=0; i<=2; ++i){
                        nmin[i]= normalBuffer[i][y][0];
                        nmax[i]= normalBuffer[i][y][1];

                        lmin[i]= lightPosBuffer[i][y][0];
                        lmax[i]= lightPosBuffer[i][y][1];

                        omin[i]= obsBuffer[i][y][0];
                        omax[i]= obsBuffer[i][y][1];


                        }

                }
                else{
                    xmin= yBuffer[y][1];
                    xmax= yBuffer[y][0];

                    for(int i=0; i<=2; ++i){
                        imin[i]= colorBuffer[i][y][1];
                        imax[i]= colorBuffer[i][y][0];
                        }
                    for(int i=0; i<=2; ++i){
                        nmin[i]= normalBuffer[i][y][1];
                        nmax[i]= normalBuffer[i][y][0];

                        lmin[i]= lightPosBuffer[i][y][1];
                        lmax[i]= lightPosBuffer[i][y][0];

                        omin[i]= obsBuffer[i][y][1];
                        omax[i]= obsBuffer[i][y][0];


                        }

                }

                for(int i=0;i<=2;++i){
                    deltaI[i]=(imin[i]-imax[i])/(xmin-xmax);
                    deltaN[i]=(nmin[i]-nmax[i])/(xmin-xmax);
                    deltaL[i]=(lmin[i]-lmax[i])/(xmin-xmax);
                    deltaO[i]=(omin[i]-omax[i])/(xmin-xmax);

                }


           for(int x=xmin; x<=xmax; ++x){
                p_fill.append(x);
                p_fill.append(y);
                rasterPoint.append(p_fill);
                p_fill.clear();

                for(int i=0; i<=2; ++i){
                    rasterColor[i].append(imin[i]);
                    imin[i] += deltaI[i];

                    rasterNormal[i].append(nmin[i]);
                    nmin[i] += deltaN[i];

                    rasterLightPos[i].append(lmin[i]);
                    lmin[i] += deltaL[i];

                    rasterObs[i].append(omin[i]);
                    omin[i] += deltaO[i];

                    }

            }

        }



    //borrar
    for(int y=0;y<400;++y){
        if(yBuffer[y][0] != 0){
                yBuffer[y][0]=0;

                for(int i=0;i<2;++i){
                    colorBuffer[i][y][0]=0;
                    normalBuffer[i][y][0]=0;
                    lightPosBuffer[i][y][0]=0;
                    obsBuffer[i][y][0]=0;
                }

        }

        if(yBuffer[y][1] != 0){
                yBuffer[y][1]=0;

                for(int i=0;i<2;++i){
                    colorBuffer[i][y][1]=0;
                     normalBuffer[i][y][1]=0;
                     lightPosBuffer[i][y][1]=0;
                     obsBuffer[i][y][1]=0;

                }

        }
    }


}




void CamProjection::scanFillPoly(int v1, int v2)
{
    int up=0;

    if(rasterPoint[v1][1]>rasterPoint[v2][1])//cambiar coordenadas
    {
        up=1;
        double temp=v1;
        v1=v2; v2=temp;
    }
    double x1=rasterPoint[v1][0], y1=rasterPoint[v1][1];
    double x2=rasterPoint[v2][0], y2=rasterPoint[v2][1];


    double dx= x2-x1;
    double dy= y2-y1;
    double m = dx/dy;

    double yd=y1+1 - y1;
    double x= x1+ m*yd ;

    int y1p=int(y1+1);
    int y2p= int(y2);

    //INTERPOLACION NORMALES
    double deltaN[3];
    for(int i=0; i<=2; i++){
        deltaN[i]=(rasterNormal[i][v2]-rasterNormal[i][v1])/(dy);
    }

    double N[3]= {rasterNormal[0][v1], rasterNormal[1][v1], rasterNormal[2][v1]};


    double deltaL[3];
    for(int i=0; i<=2; i++){
        deltaL[i]=(rasterLightPos[i][v2]-rasterLightPos[i][v1])/(dy);
    }

    double L[3]= {rasterLightPos[0][v1], rasterLightPos[1][v1], rasterLightPos[2][v1]};


    double deltaO[3];
    for(int i=0; i<=2; i++){
        deltaO[i]=(rasterObs[i][v2]-rasterObs[i][v1])/(dy);
    }

    double O[3]= {rasterObs[0][v1], rasterObs[1][v1], rasterObs[2][v1]};


    //INTERPOLACIOON COLOR GOROUD
    double I_v1[3] = {rasterColor[0][v1], rasterColor[1][v1], rasterColor[2][v1] };
    double I_v2[3] = {rasterColor[0][v2], rasterColor[1][v2], rasterColor[2][v2] };

    double deltaI[3];
    for(int i=0; i<=2; i++){
        deltaI[i]=(I_v2[i]-I_v1[i])/(y2-y1);
    }
    double I[3]={I_v1[0], I_v1[1], I_v1[2]};




    for(int y=y1p ;y<=y2p ;y++){
           yBuffer[y][up]=int(x);
            x+=m;


            //normal
            for(int i=0; i<=2; i++){
                normalBuffer[i][y][up]=N[i];
                N[i] +=deltaN[i];
            }

            for(int i=0; i<=2; i++){
                lightPosBuffer[i][y][up]=L[i];
                L[i] +=deltaL[i];

                obsBuffer[i][y][up]=O[i];
                O[i] +=deltaO[i];
            }



            //color
            for(int i=0; i<=2; i++){
                colorBuffer[i][y][up]=I[i];
                I[i] +=deltaI[i];
            }

           }
}






//void CamProjection::scanDDA()
//{
//     std::vector<int> ddaBuffer;
//    double x1= rasterPoint[2], y1=rasterPoint[3], x2=rasterPoint[4], y2=rasterPoint[5];
//    double mx=x2-x1;
//    double my=y2-y1;

//    double s=0;
//    if (abs(mx)>abs(my)){
//        s = abs(mx);
//    }else{
//            s=abs(my);
//    }

//    double dx= mx/s, dy=my/s;
//    int x=x1, y=y1;


//    for(int i=0; i<s; i++){
//        x=int(x1+dx*i);
//        y=int(y1+dy*i);
//        ddaBuffer.push_back(x);
//        ddaBuffer.push_back(y);

//        rasterPoint.append(x);
//        rasterPoint.append(y);

//    }

//    qDebug()<<ddaBuffer;
//}



//double CamProjection::dot_product(std::vector<double> v1, std::vector<double> v2){

//    double sum=0;
//    for(int i=0; i<v1.size(); ++i){
//        sum += v1[i]*v2[i];
//    }
//    return sum;
//}


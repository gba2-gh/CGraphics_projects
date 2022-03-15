#include "camProjection.h"
#include<QDebug>
#include<vector>
#include"cubeobject.h"


void   CamProjection::projectPoint(CubeObject cubeobject, bool ortho,  double xdmax ,double xdmin,
                                                                 double ydmax, double ydmin)
{
    std::vector<std::vector<double> > vertices= cubeobject.vertices;
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



    double I=0;
    double dp=0;
    double Ia=10, Id=10;

    double kd[3]={0.07568, 0.61424,0.07568};
    double ka[3]={0.3, 0.3,0.3};
    //double kd[3]={1,1,1};
    std::vector<double> Lp={10, 10, 10};
    std::vector<double> L2={0, -5, 10};
    std::vector<double> L;
    for(int k=0; k<vProy.size(); ++k){

        p_dev.append(sx*vProy[k][0] + ox);
        p_dev.append(sy*vProy[k][1] + oy);

        rasterPoint.append(p_dev);
        p_dev.clear();
        //rasterPoint.append(yd);


        //color
        for(int i=0; i<=2; ++i){
            I= Ia*ka[i];
            for (int j=0; j<=2; ++j){
                L.push_back(cubeobject.vertices[k][j]-Lp[j]);
            }
            dp=dot_product(L,cubeobject.vertexNormals[k]);
            I += Id*kd[i]*dp ;
            L.clear();
           //I += 40*kd[i]*dot_product(L2,cubeobject.vertexNormals[k]);

            rasterColor[i].append(I);
         }
       }

//    double min=255, max=0;
//    //0 a 255
//    for(int i=0; i<=rasterColor[0].size(); ++i){
//        for(int j=0; j<=2;++j){
//            if(rasterColor[j][i] < min){
//                 min=rasterColor[j][i];
//            }
//            if(rasterColor[j][i] > max){
//                 max=rasterColor[j][i];
//            }
//        }
//    }
//    I=0;
//    for(int i=0; i<=rasterColor[0].size(); ++i){
//        for(int j=0; j<=2;++j){

//            I= (rasterColor[j][i]-min)*((255-0)/(max-min));
//            rasterColor[j][i]=I;

//        }
//    }

    if(fillPolyBool){
        drawFaces(cubeobject);
    }
   // scanDDA();


    vProy.clear();

}


void CamProjection::drawFaces(CubeObject cubeobject){

//    std::vector<QList<int> > face;

//    std::vector<std::vector<double> > vertexColor;
//    //CARA1
//    face.insert(face.end(), {rasterPoint[0], rasterPoint[1],
//                       rasterPoint[2], rasterPoint[3]});


//    vertexColor.insert(vertexColor.end(), rasterColor[0], rasterColor[1],
//            rasterColor[2], rasterColor[3]);

    fillCubeFace(0,1,2,3);
    fillCubeFace(4,5,6,7);
    fillCubeFace(0,1,6,7);
    fillCubeFace(2,3,4,5);
    fillCubeFace(1,2,5,6);
    fillCubeFace(0,3,4,7);
   // face.clear();

//    //CARA2
//    face.insert(face.end(), {rasterPoint[4], rasterPoint[5],
//                       rasterPoint[6], rasterPoint[7]});

}

void CamProjection::fillCubeFace(int v0, int v1, int v2, int v3){
//    for(int i=0; i<3; ++i){
//         scanFillPoly(face, i, i+1);
//    }
//    scanFillPoly(face, 3, 0);

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
    double I;
    double deltaI[3];
    QList<int> p_fill;

      for(int y=0;y<400;++y){

                if(yBuffer[y][0]<yBuffer[y][1]){
                    xmin= yBuffer[y][0];
                    xmax= yBuffer[y][1];

                    for(int i=0; i<=2; ++i){
                        imin[i]= colorBuffer[i][y][0];
                        imax[i]= colorBuffer[i][y][1];
                        }

                }
                else{
                    xmin= yBuffer[y][1];
                    xmax= yBuffer[y][0];

                    for(int i=0; i<=2; ++i){
                        imin[i]= colorBuffer[i][y][1];
                        imax[i]= colorBuffer[i][y][0];
                        }

                }

                for(int i=0;i<=2;++i){
                    deltaI[i]=(imin[i]-imax[i])/(xmin-xmax);
                }


           for(int x=xmin; x<=xmax; ++x){
                p_fill.append(x);
                p_fill.append(y);
                rasterPoint.append(p_fill);
                p_fill.clear();

                for(int i=0; i<=2; ++i){
                    rasterColor[i].append(imin[i]);
                    imin[i] += deltaI[i];
                    }

            }

        }



    //borrar
    for(int y=0;y<400;++y){
        if(yBuffer[y][0] != 0){
                yBuffer[y][0]=0;
                for(int i=0;i<2;++i){
                    colorBuffer[i][y][0]=0;

                }

        }

        if(yBuffer[y][1] != 0){
                yBuffer[y][1]=0;

                for(int i=0;i<2;++i){
                    colorBuffer[i][y][1]=0;

                }

        }
    }


}




void CamProjection::scanFillPoly(int v1, int v2)
{

    double x1=rasterPoint[v1][0], y1=rasterPoint[v1][1];
    double x2=rasterPoint[v2][0], y2=rasterPoint[v2][1];


    int up=0;

    if(y1>y2)//cambiar coordenadas
    {
        up=1;
        double temp=x1;
        x1=x2;x2=temp;
        temp=y1;y1=y2;y2=temp;

        temp=v1;
        v1=v2; v2=temp;
    }

    double kd[3]={0.07568, 0.61424,0.07568};
    double ka[3]={0.3, 0.3,0.3};
    //double kd[3]={1,1,1};
    std::vector<double> L={-30, 0, 0};
    double I_v1[3] = {rasterColor[0][v1], rasterColor[1][v1], rasterColor[2][v1] };
    double I_v2[3] = {rasterColor[0][v2], rasterColor[1][v2], rasterColor[2][v2] };




//    qDebug() << "v1" << v1;


//    for(int i=0; i<=2; i++){
//            qDebug() << "I_v1:" << I_v1[i];
//    }
//    qDebug() << "v2" << v2;
//    for(int i=0; i<=2; i++){
//            qDebug() << "I_v2:" << I_v2[i];
//    }

    double dx= x2-x1;
    double dy= y2-y1;
    double m = dx/dy;

    double yd=y1+1 - y1;
    double x= x1+ m*yd ;

    int y1p=int(y1+1);
    int y2p= int(y2);


    //INTERPOLACIOON COLOR
    double deltaI[3];
    for(int i=0; i<=2; i++){
        deltaI[i]=(I_v2[i]-I_v1[i])/(y2-y1);
    }
    double I[3]={I_v1[0], I_v1[1], I_v1[2]};

    for(int y=y1p ;y<=y2p ;y++){
           yBuffer[y][up]=int(x);
            x+=m;
            for(int i=0; i<=2; i++){
                //colorBuffer[i].append(I[i]);
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



double CamProjection::dot_product(std::vector<double> v1, std::vector<double> v2){

    double sum=0;
    for(int i=0; i<v1.size(); ++i){
        sum += v1[i]*v2[i];
    }
    return sum;
}


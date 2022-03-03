#include "camProjection.h"
#include<QDebug>
#include<vector>
void   CamProjection::projectPoint(std::vector<std::vector<double> > vertices, bool ortho,  double xdmax ,double xdmin,
                                                                 double ydmax, double ydmin, bool fillPolyBool)
{
    std::vector<double> vProy;
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

    if(ortho){//Ortogonal
        vProy.push_back(p[0]);
        vProy.push_back(p[1]);
    }
    else{//Perspectiva
        vProy.push_back(p[0]*f/-p[2]);
        vProy.push_back(p[1]*f/-p[2]);

    }
 }

    //qDebug()<<vProy;

//RASTERIZAR
    int canvSc=1;
    if(ortho){canvSc=20;}
    int xd, yd;
    double xpmax=2*canvSc, xpmin=-2*canvSc;
    double ypmax=2*canvSc, ypmin=-2*canvSc;
    double sx= ((xdmax-xdmin)/(xpmax-xpmin)), sy= ((ydmin-ydmax)/(ypmax-ypmin));
    double ox = sx*(-xpmin)+ xdmin, oy = sx*(-ypmin)+ ydmin;

    for(int k=0; k<vProy.size()-1;k=k+2){
        xd=sx*vProy[k] + ox;
        yd=sy*vProy[k+1] + oy;

        rasterPoint.append(xd);
        rasterPoint.append(yd);

       }
    qDebug()<< "RASTER: "<<rasterPoint;

    //todo CALCULAR CARAS
    scanFillPoly(rasterPoint[2], rasterPoint[3], rasterPoint[0], rasterPoint[1]);
    scanFillPoly(rasterPoint[0], rasterPoint[1], rasterPoint[6], rasterPoint[7]);
    scanFillPoly(rasterPoint[6], rasterPoint[7], rasterPoint[4], rasterPoint[5]);
    scanFillPoly(rasterPoint[4], rasterPoint[5], rasterPoint[2], rasterPoint[3]);
    //scanDDA();

    //MAXIMOS Y
    for(int i=1;i<8;i=i+2){
        if(rasterPoint[i]>yMax){
            yMax=rasterPoint[i];
        }
        if(rasterPoint[i]<yMin){
            yMin=rasterPoint[i];
        }
    }
    qDebug()<<yMax<<yMin;
    qDebug() << "BUFFER 0/////////////////////////////////";
    for(int y=0;y<400;y++){
        if(yBuffer[y][0] != 0){
            qDebug() << "ybuffer0"<< y <<","<<yBuffer[y][0];
        }

    }

    qDebug() << "BUFFER 1/////////////////////////////////";
    for(int y=0;y<400;y++){
        if(yBuffer[y][1] != 0){
            qDebug() << "ybuffer1"<< y <<","<<yBuffer[y][1];
        }

    }


    if(fillPolyBool){
        for(int y=0;y<400;++y){
            for(int x=yBuffer[y][0]; x<= yBuffer[y][1]; ++x){
                rasterPoint.append(x);
                rasterPoint.append(y);
            }

        }
    }


    //borrar
    for(int y=0;y<400;++y){
        if(yBuffer[y][0] != 0){
                yBuffer[y][0]=0;
        }

        if(yBuffer[y][1] != 0){
                yBuffer[y][1]=0;
        }
    }


//sizeof(yBuffer)/sizeof(yBuffer[0][0])


//    int it=0;
//    int xmin=0,xmax=0;
//    for(int y=yMin; y<yMax; ++y){
//        if(yBufferD[it]>yBufferU[it]){
//            xmax=yBufferD[it];
//            xmin=yBufferU[it];
//        }else{
//            xmax=yBufferU[it];
//            xmin=yBufferD[it];

//        }
//        for(int x= xmin; x<xmax; ++x){
//        rasterPoint.append(x);
//        rasterPoint.append(y);
//        }
//        it=it+1;
 //   }


    yBufferD.clear();
    yBufferU.clear();
    vProy.clear();

}







void CamProjection::scanFillPoly(double x1, double y1, double x2, double y2)
{
    int up=0;

    if(y1>y2)
    {
        up=1;
        double temp=x1;
        x1=x2;x2=temp;
        temp=y1;y1=y2;y2=temp;
    }

    double dx= x2 - x1;
    double dy= y2- y1;
    double m = dx/dy;

    double ey=int(y1+1) - y1;
    double ex= m*ey;


    double Ax= x1+ex ;
    int Ay=int(y1+1);
    int By= int(y2);

    double x=Ax;
    for(int y=Ay; y<By ;y++){
           yBuffer[y][up]=int(x);
                  //yBufferD.push_back(int(x));
                   //yBufferD.push_back(y);
                   //rasterPoint.append(x);
                   //rasterPoint.append(y);
            x+=m;
           }


//    if(dy>0){
//        for(int y=Ay; y<By ;y++){
//                yBufferD.push_back(int(x));
//                yBufferD.push_back(y);
//                //rasterPoint.append(x);
//                //rasterPoint.append(y);
//                    x+=m;
//        }
//        if(By>yMax){
//            yMax=By;
//        }
//        if(Ay<yMin){
//            yMin=Ay;
//        }

//    }
//    else if(dy<0){
//        for(int y=By; y<Ay ;y++){
//                yBufferU.push_back(int(x));
//                yBufferU.push_back(y);
//                //rasterPoint.append(x);
//                //rasterPoint.append(y);
//                    x+=m;
//        }  }
    //for(int i=0; i<sizeof(yBuffer)/sizeof(yBuffer[0]); ++i){
    //qDebug()<<"yBufferD" <<yBufferD;
    //qDebug()<<"yBufferU"<< yBufferU;
    //qDebug()<<"y"<< yMax << ","<<yMin;
    //}
}


void CamProjection::scanDDA()
{
     std::vector<int> ddaBuffer;
    double x1= rasterPoint[0], y1=rasterPoint[1], x2=rasterPoint[2], y2=rasterPoint[3];
    double mx=x2-x1;
    double my=y2-y1;

    double s=0;
    if (abs(mx)>abs(my)){
        s = abs(mx);
    }else{
            s=abs(my);
    }

    double dx= mx/s, dy=my/s;
    int x=x1, y=y1;


    for(int i=0; i<s; i++){
        x=int(x1+dx*i);
        y=int(y1+dy*i);
        ddaBuffer.push_back(x);
        ddaBuffer.push_back(y);

        rasterPoint.append(x);
        rasterPoint.append(y);

    }

    qDebug()<<ddaBuffer;
}



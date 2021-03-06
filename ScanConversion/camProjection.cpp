#include "camProjection.h"
#include<QDebug>
#include<vector>
void   CamProjection::projectPoint(std::vector<std::vector<double> > vertices, bool ortho,  double xdmax ,double xdmin,
                                                                 double ydmax, double ydmin)
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
    //qDebug()<< "RASTER: "<<rasterPoint;


    if(fillPolyBool){
    //////////////////////DRAW FACES
        std::vector<int> face;
        //CARA1
        face.insert(face.end(), {rasterPoint[0], rasterPoint[1],
                           rasterPoint[2], rasterPoint[3],
                        rasterPoint[4], rasterPoint[5],
                        rasterPoint[6], rasterPoint[7]});
        fillCubeFace(face);
        face.clear();

        //CARA2
        face.insert(face.end(), {rasterPoint[8], rasterPoint[9],
                           rasterPoint[10], rasterPoint[11],
                        rasterPoint[12], rasterPoint[13],
                        rasterPoint[14], rasterPoint[15]});
        fillCubeFace(face);

        fillCubeFace(face);
        face.clear();

        //CARA3
        face.insert(face.end(), {rasterPoint[4], rasterPoint[5],
                           rasterPoint[10], rasterPoint[11],
                        rasterPoint[12], rasterPoint[13],
                        rasterPoint[2], rasterPoint[3]});
        fillCubeFace(face);
        face.clear();

        //CARA4
        face.insert(face.end(), {rasterPoint[4], rasterPoint[5],
                           rasterPoint[10], rasterPoint[11],
                        rasterPoint[8], rasterPoint[9],
                        rasterPoint[6], rasterPoint[7]});
        fillCubeFace(face);
        face.clear();

        //CARA5
        face.insert(face.end(), {rasterPoint[0], rasterPoint[1],
                           rasterPoint[14], rasterPoint[15],
                        rasterPoint[12], rasterPoint[13],
                        rasterPoint[2], rasterPoint[3]});
        fillCubeFace(face);
        face.clear();

        //CARA6
        face.insert(face.end(), {rasterPoint[0], rasterPoint[1],
                           rasterPoint[14], rasterPoint[15],
                        rasterPoint[8], rasterPoint[9],
                        rasterPoint[6], rasterPoint[7
                                 ]});
        fillCubeFace(face);
        face.clear();

    }
   // scanDDA();


    vProy.clear();

}


void CamProjection::fillCubeFace(std::vector<int> face){
    for(int i=0; i<5; i+=2){
         scanFillPoly(face[i], face[i+1], face[i+2], face[i+3]);
    }

    scanFillPoly(face[6], face[7], face[0], face[1]);

//    qDebug() << "BUFFER 0/////////////////////////////////";
//    for(int y=0;y<400;y++){
//        if(yBuffer[y][0] != 0){
//            qDebug() << "ybuffer0"<< y <<","<<yBuffer[y][0];
//        }

//    }

//    qDebug() << "BUFFER 1/////////////////////////////////";
//    for(int y=0;y<400;y++){
//        if(yBuffer[y][1] != 0){
//            qDebug() << "ybuffer1"<< y <<","<<yBuffer[y][1];
//        }

//    }

    int xmin=0,xmax=0;


        for(int y=0;y<400;++y){
            if(yBuffer[y][0]<yBuffer[y][1]){
                xmin= yBuffer[y][0];
                xmax= yBuffer[y][1];
            } else{
                xmin= yBuffer[y][1];
                xmax= yBuffer[y][0];
            }

            for(int x=xmin; x<=xmax; ++x){
                rasterPoint.append(x);
                rasterPoint.append(y);
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


}




void CamProjection::scanFillPoly(double x1, double y1, double x2, double y2)
{
    int up=0;

    if(y1>y2)//cambiar coordenadas
    {
        up=1;
        double temp=x1;
        x1=x2;x2=temp;
        temp=y1;y1=y2;y2=temp;
    }

    double dx= x2-x1;
    double dy= y2-y1;
    double m = dx/dy;

    double yd=y1+1 - y1;
    double x= x1+ m*yd ;

    int y1p=int(y1+1);
    int y2p= int(y2);

    for(int y=y1p ;y<=y2p ;y++){
           yBuffer[y][up]=int(x);
            x+=m;
           }
}



void CamProjection::scanDDA()
{
     std::vector<int> ddaBuffer;
    double x1= rasterPoint[2], y1=rasterPoint[3], x2=rasterPoint[4], y2=rasterPoint[5];
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



#include <cmath>
#include<vector>
#include<QDebug>


std::vector<std::vector<double> > rotateEuler(std::vector<std::vector<double> > coord, double rotMat[4][4]){

    double aux[4];
    double suma=0;

    for(int i=0; i<8;++i){
        for(int j=0; j<4;++j){
            for(int k=0; k<4;++k){

                suma += coord[i][k]*rotMat[j][k];

            }
            aux[j]=suma;
            suma=0;
        }

        for(int w=0; w<4;++w){

            coord[i][w] = aux[w];

        }

    }

    return coord;
}

std::vector<double> sum_vectors(std::vector<double> v1, std::vector<double> v2)
{
    std::vector<double> sum;
    for(int i=0; i<v1.size(); ++i ){
        sum.push_back(v1[i]+v2[i]);
    }

    return sum;

}

std::vector<double> norm_vector(std::vector<double> v)
{
    double norm=0;
    std::vector<double> norm_vec;
    for(int i=0; i<v.size(); ++i ){
        norm += v[i]*v[i];
    }

    norm=sqrt(norm);

    for(int i=0; i<v.size(); ++i ){
        norm_vec.push_back(v[i]/norm);

    }

    //qDebug() << norm;
    return norm_vec;

}

double dot_product(std::vector<double> v1, std::vector<double> v2){

    double sum=0;
    for(int i=0; i<v1.size(); ++i){
        sum += v1[i]*v2[i];
    }
    return sum;
}



QList<double> interpolar(int yBuffer[400][2], double colorBuffer[400][2]){

    QList<double>  rasterColor;

    int xmin=0,xmax=0;
    double imin=0;
    double imax=0;

    double deltaI;

      for(int y=0;y<400;++y){

                if(yBuffer[y][0]<yBuffer[y][1]){
                    xmin= yBuffer[y][0];
                    xmax= yBuffer[y][1];

                        imin= colorBuffer[y][0];
                        imax= colorBuffer[y][1];


                }
                else{
                    xmin= yBuffer[y][1];
                    xmax= yBuffer[y][0];

                        imin= colorBuffer[y][1];
                        imax= colorBuffer[y][0];

               }

                    deltaI=(imin-imax)/(xmin-xmax);

           for(int x=xmin; x<=xmax; ++x){

               rasterColor.append(imin);
               imin += deltaI;

            }

        }

      return rasterColor;



}

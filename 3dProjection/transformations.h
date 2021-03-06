#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H
#include <cmath>
#include<vector>
 std::vector<std::vector<double>> rotateEuler( std::vector<std::vector<double> > coord, double rotMat[4][4]){

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


#endif // TRANSFORMATIONS_H

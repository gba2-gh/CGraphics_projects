#ifndef LIGHTS_H
#define LIGHTS_H
#include<vector>

class lights
{
public:
    lights();

    double intensity=1;
    double color[3]={0,0,0};
    //double reflecCoef;
    double p=0;

    std::vector<double> lightPos;

};


class ambientLight : public lights {

  public:


};


#endif // LIGHTS_H

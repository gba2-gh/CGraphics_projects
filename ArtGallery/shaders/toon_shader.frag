#version 450 core

///LIGHTS !!
struct Light{
    int type;
    vec3 position;
    vec3 color;
    vec3 direction;
    float intensity;

    float attFactor_k;
    float attFactor_l;
    float attFactor_q;

    float cut_angle;

    mat4 view;
    mat4 projection;
};
uniform int numL=9;
uniform Light lights[9];
vec4 pos_lightProj;


///OUT
out vec4 FragColor;

///IN
in vec3 Normals;
in vec3 p;
in vec2 UVcoords;


uniform vec3 eyePos;
//TEXTURE
uniform sampler2D textImage;
uniform sampler2D shadowMap;
uniform bool use_texture;

float calculateShadow(vec4 pos_lightProj, int i){

           float  shadowV = 1.0;
           //proyectar pos_lightProj, posiciondel fragmento desde la cámara
           vec3 lightProy =  pos_lightProj.xyz / pos_lightProj.w;
           //coordenas de textura 0 a 1

           float u= lightProy.x;
           float v= lightProy.y;
           float z= lightProy.z * 0.5 +0.5;

           float offsetU;
           float offsetV;

           if(i<=3){
                offsetU= i*0.25;
                offsetV=0;
           }else{
                offsetU = i %4;
                offsetV = floor(i/4);
           }

            u= u*0.125+0.125 +offsetU;
            v= v*0.125+0.125 +offsetV;

           //profundidad desde el mapa de sombras //buffer de profundidad
           float shadowMapDepth = texture(shadowMap, vec2(u,v)).r;

           float bias=0.005;
           if(shadowMapDepth < z-bias){ //cuando buffer de profundidad es menor a la posicion, está en la sombra
                 shadowV = 0.0;}

            return shadowV;
}


void main(){
             vec3 color;
             vec3 salida;


        float value = dot(normalize(lights[0].position), Normals);

           if(value > 0.90){
              color = vec3(1.0, 1.0, 1.0);
           }
           else if(value > 0.65)
           {
               color = vec3(0.7, 0.7, 0.7);
           }
            else if(value > 0.30){
                     color = vec3(0.4, 0.4, 0.4);
               }else{
                    color = vec3(0.1, 0.1, 0.1);}

        salida = color;
//    if(use_texture){
//       FragColor = texture(textImage, UVcoords) *vec4(I, 1.0f);
//    }else{
//       FragColor = vec4(I, 1.0f);
//    }

    FragColor = vec4(salida, 1.0f);


 };






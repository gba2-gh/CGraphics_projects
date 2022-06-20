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
uniform Light lights[16];
vec4 pos_lightProj;


///OUT
out vec4 FragColor;

///IN
in vec3 Normals;
in vec3 p;
in vec2 UVcoords;
//in vec4 pos_lightProj;

//material
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ke;
uniform float shininess;
uniform vec3 eyePos;
//TEXTURE
uniform sampler2D textImage;
uniform sampler2D shadowMap;
uniform bool use_texture;

uniform float Time;

vec2 updatedCoords;

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

    //luz punto
    float distance;

    float att;


    //ambiente
    vec3 I = 1.0 * ka;
    vec3 L;
  //L1
    for(int i=0; i<numL; i++){

        pos_lightProj = lights[i].projection * lights[i].view * vec4(p, 1.0f);

        distance= length(lights[i].position - p);
        att = 1.0 / (lights[i].attFactor_k + lights[i].attFactor_l * distance + lights[i].attFactor_q * (distance * distance));


        if(lights[i].type == 0){
            L = normalize(lights[i].position); //para sun
        }else if(lights[i].type == 1){
            L = normalize(lights[i].position - p); //para spotlights
        }

        //DIFFUSE
        float dp = max(dot(L,Normals), 0.0);
        vec3 Idiff = dp * kd;
        Idiff = clamp(Idiff, 0.0, 1.0);

        //SPECULAR
        vec3 R = normalize(-reflect(L,Normals));
        vec3 E = normalize(eyePos-p);
        float dp2 = max(dot(R,E),0.0);
        vec3 Ispec = ke * pow(dp2, shininess);
        Ispec = clamp(Ispec, 0.0, 1.0) ;

        //SHADOW
        float shadow = calculateShadow(pos_lightProj, i);

        if(lights[i].type == 0){
            I = I + (Idiff +Ispec)* lights[i].color *lights[i].intensity *shadow;
        }else if(lights[i].type == 1){
            float cos_spot = dot(L, normalize(-lights[i].direction)); //para spotlight
            if (lights[i].cut_angle < cos_spot){

                    Idiff *=att;
                    Ispec *=att;
                    I +=  I + (Idiff + Ispec)* lights[i].color *lights[i].intensity *shadow;    //* light.intensity
                }
        }

    }

    updatedCoords = vec2(UVcoords.x + Time, UVcoords.y);

    if(use_texture){
       FragColor = texture(textImage, updatedCoords) *vec4(I, 1.0f);
    }else{
       FragColor = vec4(I, 1.0f);
    }

 };






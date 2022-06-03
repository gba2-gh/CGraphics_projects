#version 450 core

struct Light{
    vec3 pos;
    vec3 color;
    float intensity;

    float attFactor_k;
    float attFactor_l;
    float attFactor_q;
};

uniform Light light;
uniform Light light2;
uniform float cut_spot;
uniform vec3 spotlightDir;

out vec4 FragColor;

in vec3 Normals;
in vec3 p;
in vec2 UVcoords;

in vec4 pos_lightProj;

//material
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ke;
uniform float shininess;
uniform vec3 eyePos;
//uniform sampler2D textImage;

uniform sampler2D shadowMap;

float calculateShadow(vec4 pos_lightProj){

   float  shadowV = 1.0;
   //proyectar pos_lightProj, posiciondel fragmento desde la cámara
   vec3 lightProy =  pos_lightProj.xyz / pos_lightProj.w;
   //coordenas de 0 a 1
   vec2 lightProyXY =  lightProy.xy * 0.5 +0.5;

   //profundidad desde el mapa de sombras //buffer de profundidad
   float shadowMapDepth = texture(shadowMap, lightProyXY).z;


   float div = 1/pos_lightProj.w;
   float lightDepth = (pos_lightProj.z)*div*0.5+0.5  ;

   if(shadowMapDepth < lightDepth){ //cuando buffer de profundidad es menor a la posicion, está en la sombra
         shadowV = 0.0;}

    return shadowV;
}


void main(){

    float distance = length(light.pos - p);
    float att = 1.0 / (light.attFactor_k + light.attFactor_l * distance + light.attFactor_q * (distance * distance));

    //ambiente
    vec3 I = 1.0 * ka;

  //L1
    vec3 L = normalize(light.pos - p);
    float dp = max(dot(L,Normals), 0.0);
    //DIFFUSE
    vec3 Idiff = dp * kd;
    Idiff = clamp(Idiff, 0.0, 1.0);  

    //SPECULAR
    vec3 R = normalize(-reflect(L,Normals));
    vec3 E = normalize(eyePos-p);
    float dp2 = max(dot(R,E),0.0);
    vec3 Ispec = ke * pow(dp2, shininess);
    Ispec = clamp(Ispec, 0.0, 1.0);

    float shadowV = calculateShadow(pos_lightProj);
    float cos_spot = dot(L, normalize(-spotlightDir));
    if (cos_spot > cut_spot){

            //I +=  I + (Idiff + Ispec)* light.color ;    //* light.intensity
    }

  //L2
    L = normalize(vec3(0.0f, 10.0f, 5.0f));
    dp = max(dot(L,Normals), 0.0);

    Idiff = dp * kd;
    Idiff = clamp(Idiff, 0.0, 1.0);

    R = normalize(-reflect(L,Normals));
    E = normalize(eyePos-p);
    dp2 = max(dot(R,E),0.0);

    Ispec = ke * pow(dp2, shininess);
    Ispec = clamp(Ispec, 0.0, 1.0);


     float shadow = calculateShadow(pos_lightProj);
     I = I + (Idiff +Ispec)* light2.color *2 *(shadow) ;


     //FragColor = texture(textImage, UVcoords) *vec4(I, 1.0f);
     FragColor = vec4(I, 1.0f);

        };






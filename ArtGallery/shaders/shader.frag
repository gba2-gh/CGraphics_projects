#version 450 core

struct Light{
    vec3 pos;
    vec3 color;
    vec3 direction;
    float intensity;

    float attFactor_k;
    float attFactor_l;
    float attFactor_q;
};
uniform Light light;
uniform Light sun_light;
uniform float cut_spot;
uniform vec3 spotlightDir;

///OUT
out vec4 FragColor;

///IN
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
//TEXTURE
uniform sampler2D textImage;
uniform sampler2D shadowMap;

float calculateShadow(vec4 pos_lightProj){

   float  shadowV = 1.0;
   //proyectar pos_lightProj, posiciondel fragmento desde la cámara
   vec3 lightProy =  pos_lightProj.xyz / pos_lightProj.w;
   //coordenas de textura 0 a 1

   float u= lightProy.x;
   float v= lightProy.y;

   lightProy =  lightProy.xyz * 0.5 +0.5;

    u= u*0.25+0.25;
    v= v*0.25+0.25;

   //profundidad desde el mapa de sombras //buffer de profundidad
   float shadowMapDepth = texture(shadowMap, vec2(u,v)).r;

   float bias=0.005;
   if(shadowMapDepth < lightProy.z -bias){ //cuando buffer de profundidad es menor a la posicion, está en la sombra
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

    float cos_spot = dot(L, normalize(-spotlightDir));
    if (cos_spot > cut_spot){
            I +=  I + (Idiff + Ispec)* light.color ;    //* light.intensity
    }

  //L2
    L = normalize(sun_light.pos);
    dp = max(dot(L,Normals), 0.0);

    Idiff = dp * kd;
    Idiff = clamp(Idiff, 0.0, 1.0);

    R = normalize(-reflect(L,Normals));
    E = normalize(eyePos-p);
    dp2 = max(dot(R,E),0.0);

    Ispec = ke * pow(dp2, shininess);
    Ispec = clamp(Ispec, 0.0, 1.0);

    float shadow = calculateShadow(pos_lightProj);
     I = I + (Idiff +Ispec)* sun_light.color *3 *(shadow) ;

     FragColor = texture(textImage, UVcoords) *vec4(I, 1.0f);
      // FragColor = vec4(I, 1.0f);

        };






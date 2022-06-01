#version 450 core

out vec4 FragColor;

in vec3 Normals;
in vec3 p;

uniform bool shaderPhong;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 lightPos2;
uniform vec3 lightColor2;
uniform float lightIntensity2;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ke;
uniform float shininess;
uniform vec3 eyePos;


void main(){

    vec3 Iamb = 1.0 * ka;

  //L1
    vec3 L = normalize(lightPos - p);
    float dp = max(dot(L,Normals), 0.0);
    
    vec3 Idiff = dp * kd;
    Idiff = clamp(Idiff, 0.0, 1.0);
    
    vec3 R = normalize(-reflect(L,Normals));
    vec3 E = normalize(eyePos-p);
    float dp2 = max(dot(R,E),0.0);

    vec3 Ispec = ke * pow(dp2, shininess);
    Ispec = clamp(Ispec, 0.0, 1.0);
    

    vec3 I = Iamb + (Idiff)* lightColor* lightIntensity;

        if(shaderPhong)
            I =  Iamb + (Idiff + Ispec)* lightColor* lightIntensity;
    
  //L2
    L = normalize(lightPos2 - p);
    dp = max(dot(L,Normals), 0.0);

    Idiff = dp * kd;
    Idiff = clamp(Idiff, 0.0, 1.0);

    R = normalize(-reflect(L,Normals));
    E = normalize(eyePos-p);
    dp2 = max(dot(R,E),0.0);

    Ispec = ke * pow(dp2, shininess);
    Ispec = clamp(Ispec, 0.0, 1.0);


    if(shaderPhong){
        I = I + (Idiff+Ispec)* lightColor2* lightIntensity2;
    }else{
        I = I + (Idiff)* lightColor2* lightIntensity2 ;
    }


    //gl_FragColor = vec4(I, 1.0f);
     FragColor = vec4(0.0f ,1.0f , 0.0f , 1.0f);


        };


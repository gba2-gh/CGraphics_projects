#version 450 core


in vec3 p;
in vec3 Normals;

uniform samplerCube cubetexture;
uniform vec3 eyePos;

out vec4 FragColor;

void main(){
            vec3 L = normalize(p - eyePos); //para spotlights
            vec3 R = normalize(reflect(L,Normals));
            FragColor = texture(cubetexture, R);
        };


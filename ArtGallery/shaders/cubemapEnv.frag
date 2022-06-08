#version 450 core

in vec3 pos;
uniform samplerCube cubetexture;

out vec4 FragColor;

void main(){

            FragColor = texture(cubetexture, pos);
        };


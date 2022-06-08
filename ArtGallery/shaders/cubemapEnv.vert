#version 450 core

layout (location = 0) in vec3 inPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 pos;

void main(){ 

        pos = inPos;
        gl_Position = proj * view vec4(inPos, 1.0) ;

        };

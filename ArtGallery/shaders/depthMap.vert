#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

//uniform mat4 lightSpaceMatrix;

void main(){ 

        gl_Position = proj * view * model *vec4(inPos, 1.0) ;

        };

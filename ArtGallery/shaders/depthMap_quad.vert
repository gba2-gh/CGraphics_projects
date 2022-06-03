#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;

out vec2 uvCoord;


void main(){ 

        uvCoord = inUV;
        gl_Position = vec4(inPos, 1.0) ;

        };

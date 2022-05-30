#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

in vec4 vert;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 normalMat;

out vec3 p;
out vec3 Normals;

void main(){ 

 	gl_Position = proj * view * model *vec4(inPos, 1.0) ;
 	p = vec3(model * vec4(inPos, 1.0));

        Normals = normalize( mat3(normalMat) * inNormal);
        };

#version 450 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 normalMat;

uniform mat4 view_light;
uniform mat4 proj_light;

out vec3 p;
out vec3 Normals;
out vec2 UVcoords;

out vec4 pos_lightProj;


void main(){ 

        //POINT PROJECTION FROM CAMERA
        Normals = normalize( mat3(normalMat) * inNormal);
        UVcoords = inUV;
 	p = vec3(model * vec4(inPos, 1.0));
        gl_Position = proj * view * model *vec4(inPos, 1.0) ;

        //POINT PROJECTION FROM LIGHT POSITION
        pos_lightProj =proj_light * view_light * model *vec4(inPos, 1.0);

        };

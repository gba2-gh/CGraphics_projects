#version 450 core

out vec4 FragColor;

in vec2 uvCoord;

uniform sampler2D depthTexture;
uniform float near_plane;
uniform float far_plane;



void main()
{
    float depthValue = texture(depthTexture, uvCoord).r;


    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}

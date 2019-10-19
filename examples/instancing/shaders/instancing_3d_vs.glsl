#version 330 core

layout ( location = 0 ) in vec3 position; // position in model space
layout ( location = 1 ) in vec3 color;
layout ( location = 2 ) in mat4 modelMatrix; // world-transform in world-space

out vec3 fColor;

uniform mat4 u_viewProjMatrix;

void main()
{
    gl_Position = u_viewProjMatrix * modelMatrix * vec4( position, 1.0f );
    fColor = color;
}
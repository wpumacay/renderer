#version 330 core

layout ( location = 0 ) in vec3 position; // position in model space
layout ( location = 1 ) in vec3 color;

out vec3 fColor;

uniform mat4 u_viewProjMatrix;
uniform mat4 u_modelMatrix;

void main()
{
    gl_Position = u_viewProjMatrix * u_modelMatrix * vec4( position, 1.0f );
    fColor = color;
}
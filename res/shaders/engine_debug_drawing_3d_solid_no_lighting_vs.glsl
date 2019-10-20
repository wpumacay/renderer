#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec4 color;
layout ( location = 3 ) in mat4 modelMatrix;

out vec4 fColor;

uniform mat4 u_viewProjMatrix;

void main()
{
    gl_Position = u_viewProjMatrix * modelMatrix * vec4( position, 1.0f );
    fColor = color;
}
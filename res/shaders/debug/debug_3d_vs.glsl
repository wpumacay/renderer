#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 color;

uniform mat4 u_tView;
uniform mat4 u_tProj;

out vec3 vColor;

void main()
{
    gl_Position = u_tProj * u_tView * vec4( position, 1.0f );
    vColor = color;
}
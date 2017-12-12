#version 330 core

layout ( location = 0 ) in vec3 position;

uniform mat4 u_tView;
uniform mat4 u_tProj;

void main()
{
    gl_Position = u_tProj * u_tView * vec4( position, 1.0f );
}
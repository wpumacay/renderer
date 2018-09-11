#version 330 core

in vec3 fNormal;
out vec4 color;

uniform vec3 u_color;

void main()
{
    color = vec4( u_color, 1.0f );
}
#version 330 core

uniform vec4 u_color;

out vec4 color;

void main()
{
    color = vec4( u_color );
}
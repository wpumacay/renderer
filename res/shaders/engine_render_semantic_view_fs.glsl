#version 330 core

out vec4 fColor;

uniform vec3 u_maskColor;

void main()
{
    fColor = vec4( u_maskColor, 1.0f );
}

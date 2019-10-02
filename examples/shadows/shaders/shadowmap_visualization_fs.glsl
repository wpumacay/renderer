#version 330 core

in vec2 fTexcoord;
out vec4 fColor;

uniform sampler2D u_shadowmap;

void main()
{
    fColor = vec4( vec3( texture( u_shadowmap, fTexcoord ).r ) , 1.0f );
}
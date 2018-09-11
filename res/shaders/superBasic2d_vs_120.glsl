
#version 120

attribute vec2 pos;

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

void main()
{
    gl_Position = u_ProjMatrix * u_ViewMatrix * vec4( pos.x, 0.0, pos.y, 1.0 );
}
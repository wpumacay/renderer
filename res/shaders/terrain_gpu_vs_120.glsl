#version 120

attribute vec2 position;

uniform mat4 u_tView;
uniform mat4 u_tProj;

uniform vec2 u_tTerrainPosition;

varying vec3 fNormal;

void main()
{
    gl_Position = u_tProj * u_tView * vec4( position, 1.0f );
    fNormal = normal;
}
#version 120

attribute vec3 position;
attribute vec3 normal;

uniform mat4 u_tModel;
uniform mat4 u_tView;
uniform mat4 u_tProj;

varying vec3 fNormal;

void main()
{
    gl_Position = u_tProj * u_tView * u_tModel * vec4( position, 1.0f );
    fNormal = mat3( u_tModel ) * normal;
}
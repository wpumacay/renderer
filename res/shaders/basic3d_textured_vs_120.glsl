#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

uniform mat4 u_tModel;
uniform mat4 u_tView;
uniform mat4 u_tProj;

varying vec3 fNormal;
varying vec2 fTexCoord;

void main()
{
    gl_Position = u_tProj * u_tView * u_tModel * vec4( position, 1.0f );
    fNormal = mat3( u_tModel ) * normal;
    fTexCoord = vec2( texCoord.x, texCoord.y );
}
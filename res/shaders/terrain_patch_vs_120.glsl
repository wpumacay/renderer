#version 120

attribute vec3 position;
attribute vec3 normal;
attribute vec2 texCoord;

uniform mat4 u_tView;
uniform mat4 u_tProj;

varying vec3 fNormal;
varying vec3 fFragPos;
varying vec2 fTexCoord;

void main()
{
    gl_Position = u_tProj * u_tView * vec4( position, 1.0f );
    fNormal = normal;
    fFragPos = position;
    fTexCoord = texCoord;
}

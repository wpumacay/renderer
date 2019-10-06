#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 texCoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewProjMatrix;
uniform mat4 u_normalMatrix;
uniform mat4 u_viewMatrix;// required for fog calculations

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexcoord;
out vec3 fViewPosition; // position w.r.t. camera

void main()
{
    vec4 _posInWorld = u_modelMatrix * vec4( position, 1.0f );
    gl_Position = u_viewProjMatrix * _posInWorld;

    fPosition = vec3( _posInWorld );
    fViewPosition = vec3( u_viewMatrix * _posInWorld );
    fNormal = vec3( u_normalMatrix * vec4( normal, 0.0f ) );
    fTexcoord = texCoord;
}
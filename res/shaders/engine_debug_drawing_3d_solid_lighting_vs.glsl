#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec4 color;
layout ( location = 3 ) in mat4 modelMatrix;
layout ( location = 7 ) in mat4 normalMatrix;

out vec3 fPosition;
out vec3 fNormal;
out vec4 fColor;

uniform mat4 u_viewProjMatrix;

void main()
{
    vec4 _posInWorld = modelMatrix * vec4( position, 1.0f );
    gl_Position = u_viewProjMatrix * _posInWorld;

    fPosition = vec3( _posInWorld );
    fNormal = vec3( normalMatrix * vec4( normal, 0.0f ) );
    fColor = color;
}
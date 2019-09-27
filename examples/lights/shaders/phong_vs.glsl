#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;

uniform mat4 u_modelMat;
uniform mat4 u_viewProjMat;
uniform mat4 u_normalMat;

out vec3 fPosition;
out vec3 fNormal;

void main()
{
    gl_Position = u_viewProjMat * u_modelMat * vec4( position, 1.0f );
    fNormal = mat3( u_normalMat ) * normal;
    fPosition = vec3( u_modelMat * vec4( position, 1.0f ) );
}
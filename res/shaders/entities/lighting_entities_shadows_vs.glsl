#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;

uniform mat4 u_tModel;
uniform mat4 u_tView;
uniform mat4 u_tProj;

uniform mat4 u_tLightSpaceViewMatrix;
uniform mat4 u_tLightSpaceProjMatrix;

out vec3 vNormal;
out vec3 vFragPos;
out vec4 vPosLightSpace;

void main()
{
    gl_Position = u_tProj * u_tView * u_tModel * vec4( position, 1.0f );
    vFragPos = vec3( u_tModel * vec4( position, 1.0f ) );
    vNormal = mat3( transpose( inverse( u_tModel ) ) ) * normal;

    vPosLightSpace = u_tLightSpaceProjMatrix * u_tLightSpaceViewMatrix * vec4( vFragPos, 1.0 );
}
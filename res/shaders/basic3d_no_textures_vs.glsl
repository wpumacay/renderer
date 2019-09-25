#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;

uniform mat4 u_tModel;
uniform mat4 u_tView;
uniform mat4 u_tProj;

out vec3 fNormal;

void main()
{
    gl_Position = u_tProj * u_tView * u_tModel * vec4( position, 1.0f );
    fNormal = mat3( transpose( inverse( u_tModel ) ) ) * normal;
}
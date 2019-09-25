#version 330 core

layout ( location = 0 ) in vec3 position;

uniform mat4 u_modelMatrix;
uniform mat4 u_lightSpaceViewMatrix;
uniform mat4 u_lightSpaceProjMatrix;


void main()
{
    gl_Position = u_lightSpaceProjMatrix * u_lightSpaceViewMatrix * u_modelMatrix * vec4( position, 1.0 );
}
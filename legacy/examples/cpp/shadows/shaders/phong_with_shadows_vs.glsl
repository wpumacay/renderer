#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;
layout ( location = 2 ) in vec2 texcoord;

uniform mat4 u_modelMatrix;
uniform mat4 u_normalMatrix;
uniform mat4 u_viewProjMatrix;
uniform mat4 u_viewProjLightSpaceMatrix;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexcoord;
out vec4 fLightClipSpacePosition;

void main()
{
    fPosition = vec3( u_modelMatrix * vec4( position, 1.0f ) );
    fNormal = vec3( u_normalMatrix * vec4( normal, 0.0f ) );
    fTexcoord = texcoord;
    fLightClipSpacePosition = u_viewProjLightSpaceMatrix * vec4( fPosition, 1.0f );

    gl_Position = u_viewProjMatrix * vec4( fPosition, 1.0f );
}

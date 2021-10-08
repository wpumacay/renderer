#version 330 core

layout ( location = 0 ) in vec3 position;
layout ( location = 1 ) in vec3 normal;

uniform mat4 u_modelMat;
uniform mat4 u_viewProjMat;
uniform mat4 u_normalMat;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_viewerPosition;
uniform vec3 u_lightPosition;

out vec3 fColor;

void main()
{
    gl_Position = u_viewProjMat * u_modelMat * vec4( position, 1.0f );
    vec3 vPosition = vec3( u_modelMat * vec4( position, 1.0f ) );
    vec3 vNormal = mat3( u_normalMat ) * normal;
    // do light computations here
    vec3 _lAmbientComp = 0.1f * u_lightColor;

    vec3 _lDiffuseComp = max( dot( normalize( u_lightPosition - vPosition ),
                                   normalize( vNormal ) ), 0.0f ) * u_lightColor;

    vec3 _lSpecularComp = 0.5f * pow( max( dot( normalize( u_viewerPosition - vPosition ),
                                                reflect( -normalize( u_lightPosition - vPosition ),
                                                         normalize( vNormal ) ) ), 0.0f ), 32 ) * u_lightColor;

    fColor = ( _lAmbientComp + _lDiffuseComp + _lSpecularComp ) * u_objectColor;
}

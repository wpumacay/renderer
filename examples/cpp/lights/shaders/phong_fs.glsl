#version 330 core

in vec3 fPosition;
in vec3 fNormal;
out vec4 fColor;

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPosition;
uniform vec3 u_viewerPosition;

void main()
{
    vec3 _lAmbientComp = 0.1 * u_lightColor;
    vec3 _lDiffuseComp = max( dot( normalize( fNormal ), 
                                   normalize( u_lightPosition - fPosition ) ), 0.0f ) * u_lightColor;
    vec3 _lSpecularComp = 0.5 * pow( max( dot( normalize( u_viewerPosition - fPosition ),
                                               reflect( -normalize( u_lightPosition - fPosition ),
                                                        normalize( fNormal ) ) ), 0.0f ), 32 ) * u_lightColor;

    vec3 _oAmbiengComp = _lAmbientComp * u_objectColor;
    vec3 _oDiffuseComp = _lDiffuseComp * u_objectColor;
    vec3 _oSpecularComp = _lSpecularComp * u_objectColor;

    vec3 _oResultColor = _oAmbiengComp + _oDiffuseComp + _oSpecularComp;

    fColor = vec4( _oResultColor, 1.0f );
}
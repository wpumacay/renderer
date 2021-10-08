#version 330 core

in vec3 fPosition;
in vec3 fNormal;
out vec4 fColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material u_material;

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    vec3 position;
};
uniform Light u_light;

uniform vec3 u_viewerPosition;

void main()
{
    vec3 _lAmbientComp = u_light.ambient;
    vec3 _lDiffuseComp = max( dot( normalize( fNormal ),
                                   normalize( u_light.position - fPosition ) ), 0.0f ) * u_light.diffuse;
    vec3 _lSpecularComp = pow( max( dot( normalize( u_viewerPosition - fPosition ),
                                         reflect( -normalize( u_light.position - fPosition ),
                                                  normalize( fNormal ) ) ), 0.0f ), u_material.shininess ) * u_light.specular;

    vec3 _oAmbiengComp = u_light.intensity * _lAmbientComp * u_material.ambient;
    vec3 _oDiffuseComp = u_light.intensity * _lDiffuseComp * u_material.diffuse;
    vec3 _oSpecularComp = u_light.intensity * _lSpecularComp * u_material.specular;

    vec3 _oResultColor = _oAmbiengComp + _oDiffuseComp + _oSpecularComp;

    fColor = vec4( _oResultColor, 1.0f );
}

#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexcoord;
out vec4 fColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D albedoMap;
    int albedoMapActive;
    sampler2D specularMap;
    int specularMapActive;
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

vec3 _computeObjectAmbientComp( vec3 lightAmbientComp )
{
    if ( u_material.albedoMapActive == 1 )
        return vec3( texture( u_material.albedoMap, fTexcoord ) ) * u_material.ambient * lightAmbientComp;

    return u_material.ambient * lightAmbientComp;
}

vec3 _computeObjectDiffuseComp( vec3 lightDiffuseComp )
{
    if ( u_material.albedoMapActive == 1 )
        return vec3( texture( u_material.albedoMap, fTexcoord ) ) * u_material.diffuse * lightDiffuseComp;

    return u_material.diffuse * lightDiffuseComp;
}

vec3 _computeObjectSpecularComp( vec3 lightSpecularComp )
{
    if ( u_material.specularMapActive == 1 )
        return vec3( texture( u_material.specularMap, fTexcoord ) ) * u_material.specular * lightSpecularComp;

    return u_material.specular * lightSpecularComp;
}

void main()
{
    vec3 _lAmbientComp = u_light.ambient;
    vec3 _lDiffuseComp = max( dot( normalize( fNormal ), 
                                   normalize( u_light.position - fPosition ) ), 0.0f ) * u_light.diffuse;
    vec3 _lSpecularComp = pow( max( dot( normalize( u_viewerPosition - fPosition ),
                                         reflect( -normalize( u_light.position - fPosition ),
                                                  normalize( fNormal ) ) ), 0.0f ), u_material.shininess ) * u_light.specular;

    vec3 _oAmbientComp = u_light.intensity * _computeObjectAmbientComp( _lAmbientComp );
    vec3 _oDiffuseComp = u_light.intensity * _computeObjectDiffuseComp( _lDiffuseComp );
    vec3 _oSpecularComp = u_light.intensity * _computeObjectSpecularComp( _lSpecularComp );

    vec3 _oResultColor = _oAmbientComp + _oDiffuseComp + _oSpecularComp;

    fColor = vec4( _oResultColor, 1.0f );
}

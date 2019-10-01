#version 330 core

#define MAX_POINTLIGHTS 4
#define MAX_SPOTLIGHTS 4

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
    sampler2D diffuseMap;
    int diffuseMapActive;
    sampler2D specularMap;
    int specularMapActive;
    sampler2D normalMap;
    int normalMapActive;
};
uniform Material u_material;

struct DirectionalLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    int enabled;

    vec3 direction;
};
uniform DirectionalLight u_directionalLight;

struct PointLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    int enabled;

    vec3 position;
    float attnk0;
    float attnk1;
    float attnk2;
};
uniform PointLight u_pointLights[MAX_POINTLIGHTS];
uniform int u_numberOfPointLights;

struct SpotLight
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float intensity;
    int enabled;

    vec3 position;
    float attnk0;
    float attnk1;
    float attnk2;
    vec3 direction;
    float innerCutoffCos;
    float outerCutoffCos;
};
uniform SpotLight u_spotLights[MAX_SPOTLIGHTS];
uniform int u_numberOfSpotLights;

uniform vec3 u_viewerPosition;

vec3 _computeDirectionalLightContrib( DirectionalLight light, vec3 normal, vec3 viewDir );
vec3 _computePointLightContrib( PointLight light, vec3 normal, vec3 viewDir );
vec3 _computeSpotLightContrib( SpotLight light, vec3 normal, vec3 viewDir );

vec3 _computeObjectAmbientComp( vec3 lightAmbientComp );
vec3 _computeObjectDiffuseComp( vec3 lightDiffuseComp );
vec3 _computeObjectSpecularComp( vec3 lightSpecularComp );

void main()
{
    vec3 _resultColor = vec3(0.0);
    // compute some required vectors
    vec3 _normal = normalize( fNormal );
    vec3 _viewDir = normalize( u_viewerPosition - fPosition );

    // compute directional-light contribution
    _resultColor += _computeDirectionalLightContrib( u_directionalLight, _normal, _viewDir );
    // compute contribution of other lights (as requested by user in numlights)
    for ( int i = 0; i < u_numberOfPointLights; i++ )
        _resultColor += _computePointLightContrib( u_pointLights[i], _normal, _viewDir );
    for ( int i = 0; i < u_numberOfSpotLights; i++ )
        _resultColor += _computeSpotLightContrib( u_spotLights[i], _normal, _viewDir );

    fColor = vec4( _resultColor, 1.0f );
}

vec3 _computeDirectionalLightContrib( DirectionalLight light, vec3 normal, vec3 viewDir )
{
    if ( light.enabled == 0 )
        return vec3(0.0f);

    // precompute lightdir (recall this is from fragment to light "position"(inf))
    vec3 _lightdir = normalize( -light.direction );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity;

    // compute specular contribution factor
    vec3 _lSpecularComp = pow( max( dot( reflect( -_lightdir, normal ), 
                                         viewDir ), 0.0f ), u_material.shininess ) * light.specular * light.intensity;

    // finally, combine these contributions with the material to get the resulting color
    return _computeObjectAmbientComp( _lAmbientComp ) + 
           _computeObjectDiffuseComp( _lDiffuseComp ) +
           _computeObjectSpecularComp( _lSpecularComp );
}

vec3 _computePointLightContrib( PointLight light, vec3 normal, vec3 viewDir )
{
    if ( light.enabled == 0 )
        return vec3(0.0f);

    // precompute lightdir (fragment to light position) and attenuation factor
    vec3 _lightdir = normalize( light.position - fPosition );
    float _lightDist = length( light.position - fPosition );
    float _lightAttn = 1.0 / ( light.attnk0 + light.attnk1 * _lightDist + light.attnk2 * _lightDist * _lightDist );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity * _lightAttn;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity * _lightAttn;

    // compute specular contribution factor
    vec3 _lSpecularComp = pow( max( dot( reflect( -_lightdir, normal ),
                                         viewDir ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn;

    // finally, combine these contributions with the material to get the resulting color
    return _computeObjectAmbientComp( _lAmbientComp ) + 
           _computeObjectDiffuseComp( _lDiffuseComp ) +
           _computeObjectSpecularComp( _lSpecularComp );
}

vec3 _computeSpotLightContrib( SpotLight light, vec3 normal, vec3 viewDir )
{
    if ( light.enabled == 0 )
        return vec3(0.0f);

    // precompute lightdir, attenuation factor and soft-edges factor
    vec3 _lightdir = normalize( light.position - fPosition );
    float _lightDist = length( light.position - fPosition );
    float _lightAttn = 1.0 / ( light.attnk0 + light.attnk1 * _lightDist + light.attnk2 * _lightDist * _lightDist );
    float _costheta = dot( -_lightdir, normalize( light.direction ) );
    float _epsilon = light.innerCutoffCos - light.outerCutoffCos;
    float _softEdgeIntensity = clamp( ( _costheta - light.outerCutoffCos ) / _epsilon, 0.0f, 1.0f );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity * _lightAttn * _softEdgeIntensity;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity * _lightAttn * _softEdgeIntensity;

    // compute specular contribution factor
    vec3 _lSpecularComp = pow( max( dot( reflect( -_lightdir, normal ),
                                         viewDir ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn * _softEdgeIntensity;

    // finally, combine these contributions with the material to get the resulting color
    return _computeObjectAmbientComp( _lAmbientComp ) + 
           _computeObjectDiffuseComp( _lDiffuseComp ) +
           _computeObjectSpecularComp( _lSpecularComp );
}

vec3 _computeObjectAmbientComp( vec3 lightAmbientComp )
{
    if ( u_material.diffuseMapActive == 1 )
        return vec3( texture( u_material.diffuseMap, fTexcoord ) ) * lightAmbientComp;

    return u_material.ambient * lightAmbientComp;
}

vec3 _computeObjectDiffuseComp( vec3 lightDiffuseComp )
{
    if ( u_material.diffuseMapActive == 1 )
        return vec3( texture( u_material.diffuseMap, fTexcoord ) ) * lightDiffuseComp;

    return u_material.diffuse * lightDiffuseComp;
}

vec3 _computeObjectSpecularComp( vec3 lightSpecularComp )
{
    if ( u_material.specularMapActive == 1 )
        return vec3( texture( u_material.specularMap, fTexcoord ) ) * lightSpecularComp;

    return u_material.specular * lightSpecularComp;
}

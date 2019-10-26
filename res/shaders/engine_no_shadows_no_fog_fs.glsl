#version 330 core

// vertex-pair              : engine_no_shadows_no_fog_vs
// materials                : lambert, phong, blinn-phong (user selects type)
// render-mode              : forward-rendering
// num-active-lights        : 1 (only one can be active)
// shadows                  : no
// fog                      : no
// recommended-tex-slots    : order used in the engine
//              (texture-slot-0): albedoMap
//              (texture-slot-1): specularMap

in vec3 fPosition;               // vertex position
in vec3 fNormal;                 // vertex normal (not-normalized in fs. due to interpolation)
in vec2 fTexcoord;               // vertex texture coordinates (uvs)

out vec4 fColor;

struct Material
{
    int type; // 0: lambert, 1: phong, 2: blinn-phong
    int transparent; // 0: opaque, 1: transparent
    float alpha; // transparency factor
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
uniform PointLight u_pointLight;

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
uniform SpotLight u_spotLight;

uniform vec3 u_viewerPosition;

vec3 _computeColorWithDirectionalLight( DirectionalLight light, vec3 normal, vec3 viewDir );
vec3 _computeColorWithPointLight( PointLight light, vec3 normal, vec3 viewDir );
vec3 _computeColorWithSpotLight( SpotLight light, vec3 normal, vec3 viewDir );

vec3 _computeFragmentAmbientComp( vec3 lightAmbientComp );
vec3 _computeFragmentDiffuseComp( vec3 lightDiffuseComp );
vec3 _computeFragmentSpecularComp( vec3 lightSpecularComp );

void main()
{
    // texture color has alpha < eps, then discard it
    if ( u_material.albedoMapActive == 1 )
        if ( texture( u_material.albedoMap, fTexcoord ).a < 0.1f )
            discard;

    vec3 _resultColor = vec3(0.0);
    // compute some required vectors
    vec3 _normal = normalize( fNormal );
    vec3 _viewDir = normalize( u_viewerPosition - fPosition );

    if ( u_directionalLight.enabled == 1 )
        _resultColor = _computeColorWithDirectionalLight( u_directionalLight, _normal, _viewDir );
    else if ( u_pointLight.enabled == 1 )
        _resultColor = _computeColorWithPointLight( u_pointLight, _normal, _viewDir );
    else if ( u_spotLight.enabled == 1 )
        _resultColor = _computeColorWithSpotLight( u_spotLight, _normal, _viewDir );

    if ( u_material.transparent == 0 )
        fColor = vec4( _resultColor, 1.0f );
    else
        fColor = vec4( _resultColor, u_material.alpha );
}

vec3 _computeColorWithDirectionalLight( DirectionalLight light, vec3 normal, vec3 viewDir )
{
    // precompute lightdir (recall this is from fragment to light "position"(inf))
    vec3 _lightdir = normalize( -light.direction );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity;

    if ( u_material.type == 0 ) // lambert materials don't use specular-color calculations
        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               _computeFragmentDiffuseComp( _lDiffuseComp );

    if ( u_material.type == 1 ) // phong materials use reflection vs view vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( reflect( -_lightdir, normal ), 
                                             viewDir ), 0.0f ), u_material.shininess ) * light.specular * light.intensity;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               _computeFragmentSpecularComp( _lSpecularComp );
    }

    if ( u_material.type == 2 ) // blinn-phong materials use half-way vs normal vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( normalize( viewDir + _lightdir ), 
                                             normal ), 0.0f ), u_material.shininess ) * light.specular * light.intensity;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               _computeFragmentSpecularComp( _lSpecularComp );
    }

    return vec3(0.0);
}

vec3 _computeColorWithPointLight( PointLight light, vec3 normal, vec3 viewDir )
{
    // precompute lightdir (fragment to light position) and attenuation factor
    vec3 _lightdir = normalize( light.position - fPosition );
    float _lightDist = length( light.position - fPosition );
    float _lightAttn = 1.0 / ( light.attnk0 + light.attnk1 * _lightDist + light.attnk2 * _lightDist * _lightDist );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity * _lightAttn;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity * _lightAttn;

    if ( u_material.type == 0 ) // lambert materials don't use specular-color calculations
        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               _computeFragmentDiffuseComp( _lDiffuseComp );

    if ( u_material.type == 1 ) // phong materials use reflection vs view vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( reflect( -_lightdir, normal ),
                                             viewDir ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               _computeFragmentSpecularComp( _lSpecularComp );
    }

    if ( u_material.type == 2 ) // blinn-phong materials use half-way vs normal vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( normalize( viewDir + _lightdir ),
                                             normal ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               _computeFragmentSpecularComp( _lSpecularComp );
    }

    return vec3(0.0);
}

vec3 _computeColorWithSpotLight( SpotLight light, vec3 normal, vec3 viewDir )
{
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

    if ( u_material.type == 0 ) // lambert materials don't use specular-color calculations
    {
        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               _computeFragmentDiffuseComp( _lDiffuseComp );
    }

    if ( u_material.type == 1 ) // phong materials use reflection vs view vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( reflect( -_lightdir, normal ),
                                             viewDir ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn * _softEdgeIntensity;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               _computeFragmentSpecularComp( _lSpecularComp );
    }

    if ( u_material.type == 2 ) // blinn-phong materials use half-way vs normal vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( normalize( viewDir + _lightdir ),
                                             normal ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn * _softEdgeIntensity;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               _computeFragmentSpecularComp( _lSpecularComp );
    }

    return vec3(0.0);
}

vec3 _computeFragmentAmbientComp( vec3 lightAmbientComp )
{
    if ( u_material.albedoMapActive == 1 )
        return vec3( texture( u_material.albedoMap, fTexcoord ) ) * u_material.ambient * lightAmbientComp;

    return u_material.ambient * lightAmbientComp;
}

vec3 _computeFragmentDiffuseComp( vec3 lightDiffuseComp )
{
    if ( u_material.albedoMapActive == 1 )
        return vec3( texture( u_material.albedoMap, fTexcoord ) ) * u_material.diffuse * lightDiffuseComp;

    return u_material.diffuse * lightDiffuseComp;
}

vec3 _computeFragmentSpecularComp( vec3 lightSpecularComp )
{
    if ( u_material.specularMapActive == 1 )
        return vec3( texture( u_material.specularMap, fTexcoord ) ) * u_material.specular * lightSpecularComp;

    return u_material.specular * lightSpecularComp;
}

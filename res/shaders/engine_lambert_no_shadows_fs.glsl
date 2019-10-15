#version 330 core

// vertex-pair              : engine_lambert_no_shadows_vs
// material                 : lambert
// render-mode              : forward-rendering
// num-active-lights        : 1 (only one can be active)
// shadows                  : no
// fog                      : yes (if enabled)
// recommended-tex-slots    : order used in the engine
//              (texture-slot-0): albedoMap

in vec3 fPosition;         // vertex position
in vec3 fNormal;           // vertex normal (not-normalized in fs. due to interpolation)
in vec2 fTexcoord;         // vertex texture coordinates (uvs)
in vec3 fPositionToCamera; // position w.r.t. camera (for fog calculation)

out vec4 fColor;

struct LambertMaterial
{
    vec3 ambient;
    vec3 diffuse;
    sampler2D albedoMap; // texture-slot => 0
    int albedoMapActive;
};
uniform LambertMaterial u_material;

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

struct Fog
{
    int type;
    int enabled;
    vec3 color;
    float density;
    float gradient;
    float distStart;
    float distEnd;
};
uniform Fog u_fog;

vec3 _computeColorWithDirectionalLight( DirectionalLight light, vec3 normal );
vec3 _computeColorWithPointLight( PointLight light, vec3 normal );
vec3 _computeColorWithSpotLight( SpotLight light, vec3 normal );

vec3 _computeFragmentAmbientComp( vec3 lightAmbientComp );
vec3 _computeFragmentDiffuseComp( vec3 lightDiffuseComp );

float _computeFragmentFogFactor();

void main()
{
    vec3 _resultColor = vec3(0.0);
    // compute some required vectors
    vec3 _normal = normalize( fNormal );

    if ( u_directionalLight.enabled == 1 )
        _resultColor = _computeColorWithDirectionalLight( u_directionalLight, _normal );
    else if ( u_pointLight.enabled == 1 )
        _resultColor = _computeColorWithPointLight( u_pointLight, _normal );
    else if ( u_spotLight.enabled == 1 )
        _resultColor = _computeColorWithSpotLight( u_spotLight, _normal );

    if ( u_fog.enabled == 1 )
        _resultColor = mix( _resultColor, u_fog.color, _computeFragmentFogFactor() ); // res_color * (1-factor) + fog_color * (factor)

    fColor = vec4( _resultColor, 1.0f );
}

vec3 _computeColorWithDirectionalLight( DirectionalLight light, vec3 normal )
{
    // precompute lightdir (recall this is from fragment to light "position"(inf))
    vec3 _lightdir = normalize( -light.direction );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity;

    // finally, combine these contributions with the material to get the resulting color
    return _computeFragmentAmbientComp( _lAmbientComp ) + 
           _computeFragmentDiffuseComp( _lDiffuseComp );
}

vec3 _computeColorWithPointLight( PointLight light, vec3 normal )
{
    // precompute lightdir (fragment to light position) and attenuation factor
    vec3 _lightdir = normalize( light.position - fPosition );
    float _lightDist = length( light.position - fPosition );
    float _lightAttn = 1.0 / ( light.attnk0 + light.attnk1 * _lightDist + light.attnk2 * _lightDist * _lightDist );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity * _lightAttn;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity * _lightAttn;

    // finally, combine these contributions with the material to get the resulting color
    return _computeFragmentAmbientComp( _lAmbientComp ) + 
           _computeFragmentDiffuseComp( _lDiffuseComp );
}

vec3 _computeColorWithSpotLight( SpotLight light, vec3 normal )
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

    // finally, combine these contributions with the material to get the resulting color
    return _computeFragmentAmbientComp( _lAmbientComp ) + 
           _computeFragmentDiffuseComp( _lDiffuseComp );
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

float _computeFragmentFogFactor()
{
    float _fogFactor = 0.0f;
    if ( u_fog.type == 0 ) // linear
        _fogFactor = ( length( fPositionToCamera ) - u_fog.distStart ) / ( u_fog.distEnd - u_fog.distStart );
    else if ( u_fog.type == 1 ) //exponential
        _fogFactor = 1.0f - exp( -pow( ( max( length( fPositionToCamera ) - u_fog.distStart, 0.001f ) ) * u_fog.density, u_fog.gradient ) );

    _fogFactor = clamp( _fogFactor, 0.0f, 1.0f );

    /* ( 1.0 -> in full fog | 0.0 -> no fog )*/
    return _fogFactor;
}
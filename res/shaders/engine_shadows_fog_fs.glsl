#version 330 core

// vertex-pair              : engine_shadows_fog_vs
// material                 : lambert, phong, blinn-phong (user selects type)
// render-mode              : forward-rendering
// num-active-lights        : 1 (only one can be active)
// shadows                  : yes
// fog                      : yes
// recommended-tex-slots    : order used in the engine
//              (texture-slot-0): albedoMap
//              (texture-slot-1): specularMap
//              (texture-slot-2): depthMap

in vec3 fPosition;               // vertex position
in vec3 fNormal;                 // vertex normal (not-normalized in fs. due to interpolation)
in vec2 fTexcoord;               // vertex texture coordinates (uvs)
in vec4 fLightSpaceClipPosition; // clip-position w.r.t. light frustum
in vec3 fPositionToCamera;       // position w.r.t. camera (for fog calculation)

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

struct ShadowMap
{
    int         size;       // width=depth of the shadow-map texture
    sampler2D   depthMap;   // texture representing the depthMap
    int         pcfCount;   // size of pcf window used for smoothing with PCF
};
uniform ShadowMap u_shadowMap;

struct Fog
{
    int type;
    vec3 color;
    float density;
    float gradient;
    float distStart;
    float distEnd;
};
uniform Fog u_fog;

vec3 _computeColorWithDirectionalLight( DirectionalLight light, vec3 normal, vec3 viewDir, float shadowFactor );
vec3 _computeColorWithPointLight( PointLight light, vec3 normal, vec3 viewDir, float shadowFactor );
vec3 _computeColorWithSpotLight( SpotLight light, vec3 normal, vec3 viewDir, float shadowFactor );

vec3 _computeFragmentAmbientComp( vec3 lightAmbientComp );
vec3 _computeFragmentDiffuseComp( vec3 lightDiffuseComp );
vec3 _computeFragmentSpecularComp( vec3 lightSpecularComp );

float _computeFragmentShadowFactor( vec3 normal );
float _computeFragmentFogFactor();

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
    // compute shadow factor (transparent objects dont cast shadows)
    float _shadowFactor = ( u_material.transparent == 1 ) ? 0.0f : _computeFragmentShadowFactor( _normal );

    if ( u_directionalLight.enabled == 1 )
        _resultColor = _computeColorWithDirectionalLight( u_directionalLight, _normal, _viewDir, _shadowFactor );
    else if ( u_pointLight.enabled == 1 )
        _resultColor = _computeColorWithPointLight( u_pointLight, _normal, _viewDir, _shadowFactor );
    else if ( u_spotLight.enabled == 1 )
        _resultColor = _computeColorWithSpotLight( u_spotLight, _normal, _viewDir, _shadowFactor );

    _resultColor = mix( _resultColor, u_fog.color, _computeFragmentFogFactor() ); // res_color * (1-factor) + fog_color * (factor)

    if ( u_material.transparent == 0 )
        fColor = vec4( _resultColor, 1.0f );
    else
        fColor = vec4( _resultColor, u_material.alpha );
}

vec3 _computeColorWithDirectionalLight( DirectionalLight light, vec3 normal, vec3 viewDir, float shadowFactor )
{
    // precompute lightdir (recall this is from fragment to light "position"(inf))
    vec3 _lightdir = normalize( -light.direction );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity;

    if ( u_material.type == 0 ) // lambert materials don't require specular-color calculations
        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentDiffuseComp( _lDiffuseComp );

    if ( u_material.type == 1 ) // phong materials use reflection vs view vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( reflect( -_lightdir, normal ), 
                                             viewDir ), 0.0f ), u_material.shininess ) * light.specular * light.intensity;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentSpecularComp( _lSpecularComp );
    }

    if ( u_material.type == 2 ) // blinn-phong materials use half-way vs normal vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( normalize( viewDir + _lightdir ), 
                                             normal ), 0.0f ), u_material.shininess ) * light.specular * light.intensity;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentSpecularComp( _lSpecularComp );
    }

    return vec3(0.0);
}

vec3 _computeColorWithPointLight( PointLight light, vec3 normal, vec3 viewDir, float shadowFactor )
{
    // precompute lightdir (fragment to light position) and attenuation factor
    vec3 _lightdir = normalize( light.position - fPosition );
    float _lightDist = length( light.position - fPosition );
    float _lightAttn = 1.0 / ( light.attnk0 + light.attnk1 * _lightDist + light.attnk2 * _lightDist * _lightDist );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity * _lightAttn;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity * _lightAttn;

    if ( u_material.type == 0 ) // lambert materials don't require specular-color calculations
        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentDiffuseComp( _lDiffuseComp );

    if ( u_material.type == 1 ) // phong materials use reflection vs view vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( reflect( -_lightdir, normal ),
                                             viewDir ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentSpecularComp( _lSpecularComp );
    }

    if ( u_material.type == 2 ) // blinn-phong materials use half-way vs normal vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( normalize( viewDir + _lightdir ),
                                             normal ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentSpecularComp( _lSpecularComp );
    }

    return vec3(0.0);
}

vec3 _computeColorWithSpotLight( SpotLight light, vec3 normal, vec3 viewDir, float shadowFactor )
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

    if ( u_material.type == 0 ) // lambert materials don't require specular-color calculations
        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentDiffuseComp( _lDiffuseComp );

    if ( u_material.type == 1 ) // phong materials use reflection vs view vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( reflect( -_lightdir, normal ),
                                             viewDir ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn * _softEdgeIntensity;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentSpecularComp( _lSpecularComp );
    }

    if ( u_material.type == 2 ) // blinn-phong materials use half-way vs normal vectors
    {
        // compute specular contribution factor
        vec3 _lSpecularComp = pow( max( dot( normalize( viewDir + _lightdir ),
                                             normal ), 0.0f ), u_material.shininess ) * light.specular * light.intensity * _lightAttn * _softEdgeIntensity;

        return _computeFragmentAmbientComp( _lAmbientComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentDiffuseComp( _lDiffuseComp ) + 
               ( 1.0f - shadowFactor ) * _computeFragmentSpecularComp( _lSpecularComp );
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

float _computeFragmentShadowFactor( vec3 normal )
{
    /* convert to NDC in order to sample from the depthMap */
    vec3 _projCoords = fLightSpaceClipPosition.xyz / fLightSpaceClipPosition.w; // perspective divide -> clip-space
    _projCoords = _projCoords * 0.5 + 0.5; // from clip-space to NDC

    /* fragments outside of the light-space view frustum have depth > 1.0, so discard shadows for those fragments */
    if ( _projCoords.z > 1.0 )
        return 0.0;

    /* compute a bias term to avoid peter panning */
    float _bias = 0.005;
    if ( u_directionalLight.enabled == 1 )
        _bias = max( 0.05 * ( 1.0f - dot( normal, -normalize( u_directionalLight.direction ) ) ), 0.005 );
    else if ( u_pointLight.enabled == 1 ) // point lights seem to need a smaller bias
        _bias = max( 0.005 * ( 1.0f - dot( normal, normalize( u_pointLight.position - fPosition ) ) ), 0.0005 );
    else if ( u_spotLight.enabled == 1 ) // spot lights also seem to need a smaller bias
        _bias = max( 0.005 * ( 1.0f - dot( normal, normalize( u_spotLight.position - fPosition ) ) ), 0.0005 );

    /* grab depths required to check if we are in shadow or not */
    float _closestDepth = texture( u_shadowMap.depthMap, _projCoords.xy ).r;
    float _currentDepth = _projCoords.z;
    float _shadowFactor = ( ( _currentDepth - _bias ) > _closestDepth ) ? 1.0 : 0.0;

    if ( u_shadowMap.pcfCount == 0 )
        return _shadowFactor;

    float _sizeTexels = 1.0f / float(u_shadowMap.size);
    float _numTexels = ( 2.0f * float(u_shadowMap.pcfCount) + 1.0f ) * ( 2.0f * float(u_shadowMap.pcfCount) + 1.0f );

    /* use PCF to smooth the edges */
    for ( int x = -u_shadowMap.pcfCount; x <= u_shadowMap.pcfCount; x++ )
    {
        for ( int y = -u_shadowMap.pcfCount; y <= u_shadowMap.pcfCount; y++ )
        {
            float _sampleDepth = texture( u_shadowMap.depthMap, _projCoords.xy + vec2( x, y ) * _sizeTexels ).r;
            _shadowFactor += ( ( _currentDepth - _bias ) > _sampleDepth ) ? 1.0 : 0.0;
        }
    }

    _shadowFactor /= _numTexels;

    /* ( 1.0 -> in shadow | 0.0 -> no shadow )*/
    return _shadowFactor;
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
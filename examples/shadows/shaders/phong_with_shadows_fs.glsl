#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexcoord;
in vec4 fLightClipSpacePosition;

out vec4 fColor;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D diffuseMap;// slot|unit 0
    int diffuseMapActive;
    sampler2D specularMap;// slot|unit 1
    int specularMapActive;
    sampler2D normalMap;// slot|unit 2
    int normalMapActive;
};
uniform Material u_material;

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
uniform sampler2D u_depthmapTexture; // slot|unit 3

vec3 _computeLightAmbientFactor();
vec3 _computeLightDiffuseFactor();
vec3 _computeLightSpecularFactor();
vec3 _computeObjectAmbientComp( vec3 lightAmbientComp );
vec3 _computeObjectDiffuseComp( vec3 lightDiffuseComp );
vec3 _computeObjectSpecularComp( vec3 lightSpecularComp );
float _computeObjectShadowFactor();

const int PCF_COUNT = 2;
const float PCF_TOTAL_TEXELS = ( PCF_COUNT * 2.0 + 1.0 ) * ( PCF_COUNT * 2.0 + 1.0 );
const float PCF_TEXEL_SIZE = 1.0 / 2048.0;

void main()
{
    vec3 _lAmbientFactor = _computeLightAmbientFactor();
    vec3 _lDiffuseFactor = _computeLightDiffuseFactor();
    vec3 _lSpecularFactor = _computeLightSpecularFactor();

    vec3 _oAmbientComp = _computeObjectAmbientComp( _lAmbientFactor );
    vec3 _oDiffuseComp = _computeObjectDiffuseComp( _lDiffuseFactor );
    vec3 _oSpecularComp = _computeObjectSpecularComp( _lSpecularFactor );

    float _shadowFactor = _computeObjectShadowFactor();

    fColor = vec4( _oAmbientComp + ( 1.0f - _shadowFactor ) * ( _oDiffuseComp + _oSpecularComp ), 1.0f );
}

float _computeObjectShadowFactor()
{
    /* convert to NDC in order to sample from the depthmap */
    vec3 _projCoords = fLightClipSpacePosition.xyz / fLightClipSpacePosition.w; // perspective divide -> clip-space
    _projCoords = _projCoords * 0.5 + 0.5; // from clip-space to NDC

    /* fragments outside of the light-space view frustum have depth > 1.0, so discard shadows for those fragments */
    if ( _projCoords.z > 1.0 )
        return 0.0;

    /* compute a bias term to avoid peter panning */
    float _bias = 0.005;
    if ( u_directionalLight.enabled == 1 )
        _bias = max( 0.05 * ( 1.0 - dot( normalize( fNormal ), -normalize( u_directionalLight.direction ) ) ), 0.005 );

    /* grab depths required to check if we are in shadow or not */
    float _closestDepth = texture( u_depthmapTexture, _projCoords.xy ).r;
    float _currentDepth = _projCoords.z;
    float _shadowFactor = ( ( _currentDepth - _bias ) > _closestDepth ) ? 1.0 : 0.0;

    /* use PCF to smooth the edges */
    for ( int x = -PCF_COUNT; x <= PCF_COUNT; x++ )
    {
        for ( int y = -PCF_COUNT; y <= PCF_COUNT; y++ )
        {
            float _sampleDepth = texture( u_depthmapTexture, _projCoords.xy + vec2( x, y ) * PCF_TEXEL_SIZE ).r;
            _shadowFactor += ( ( _currentDepth - _bias ) > _sampleDepth ) ? 1.0 : 0.0;
        }
    }

    _shadowFactor /= PCF_TOTAL_TEXELS;

    /* compute and return shadow-factor accordingly ( 1.0 -> in shadow | 0.0 -> no shadow )*/
    return _shadowFactor;
}

vec3 _computeLightAmbientFactor()
{
    if ( u_directionalLight.enabled == 1 )
        return u_directionalLight.intensity * u_directionalLight.ambient;

    if ( u_pointLight.enabled == 1 )
    {
        // @OPTIM: should avoid doing 3-times the attenuation computation
        float _dist = length( u_pointLight.position - fPosition );
        float _attn = 1.0 / ( u_pointLight.attnk0 + u_pointLight.attnk1 * _dist + u_pointLight.attnk2 * _dist * _dist );
        return u_directionalLight.intensity * _attn * u_pointLight.ambient;
    }

    if ( u_spotLight.enabled == 1 )
        return u_directionalLight.intensity * u_directionalLight.ambient;

    // default value, in case some configuration went wrong
    return vec3( 0.2f, 0.2f, 0.2f );
}

vec3 _computeLightDiffuseFactor()
{
    if ( u_directionalLight.enabled == 1 )
        return max( dot( normalize( -u_directionalLight.direction ),
                         normalize( fNormal ) ), 0.0f ) * u_directionalLight.intensity * u_directionalLight.diffuse;

    if ( u_pointLight.enabled == 1 )
    {
        // @OPTIM: should avoid doing 3-times the attenuation computation
        // @OPTIM: should reuse some operations to save some cycles (dist and normalization can go together)
        float _dist = length( u_pointLight.position - fPosition );
        float _attn = 1.0 / ( u_pointLight.attnk0 + u_pointLight.attnk1 * _dist + u_pointLight.attnk2 * _dist * _dist );

        return max( dot( normalize( u_pointLight.position - fPosition ),
                         normalize( fNormal ) ), 0.0f ) * u_pointLight.intensity * _attn * u_pointLight.diffuse;
    }

    if ( u_spotLight.enabled == 1 )
    {
        // @OPTIM: should avoid doing 3-times the attenuation computation
        float _dist = length( u_spotLight.position - fPosition );
        float _attn = 1.0 / ( u_spotLight.attnk0 + u_spotLight.attnk1 * _dist + u_spotLight.attnk2 * _dist * _dist );

        // @OPTIM: should do this once for all contributions
        float _costheta = dot( -normalize( u_spotLight.position - fPosition ), normalize( u_spotLight.direction ) );
        float _epsilon = u_spotLight.innerCutoffCos - u_spotLight.outerCutoffCos;
        float _intensity = clamp( ( _costheta - u_spotLight.outerCutoffCos ) / _epsilon, 0.0, 1.0 );
        return max( dot( normalize( u_spotLight.position - fPosition ),
                         normalize( fNormal ) ), 0.0f ) * u_spotLight.intensity * _attn * _intensity * u_spotLight.diffuse;
    }

    // default value (constant everywhere) in case some configuration went wrong
    return vec3( 0.2f, 0.2f, 0.2f );
}

vec3 _computeLightSpecularFactor()
{
    if ( u_directionalLight.enabled == 1 )
        return pow( max( dot( normalize( reflect( normalize( u_directionalLight.direction ), fNormal ) ),
                              normalize( u_viewerPosition - fPosition ) ), 0.0f ), u_material.shininess ) * u_directionalLight.intensity * u_directionalLight.specular;

    if ( u_pointLight.enabled == 1 )
    {
        // @OPTIM: should avoid doing 3-times the attenuation computation
        // @OPTIM: should reuse some operations to save some cycles (dist and normalization can go together)
        float _dist = length( u_pointLight.position - fPosition );
        float _attn = 1.0 / ( u_pointLight.attnk0 + u_pointLight.attnk1 * _dist + u_pointLight.attnk2 * _dist * _dist );
        return pow( max( dot( normalize( reflect( -normalize( u_pointLight.position - fPosition ), fNormal ) ),
                              normalize( u_viewerPosition - fPosition ) ), 0.0f ), u_material.shininess ) * u_pointLight.intensity * _attn * u_pointLight.specular;
    }

    if ( u_spotLight.enabled == 1 )
    {
        // @OPTIM: should avoid doing 3-times the attenuation computation
        float _dist = length( u_spotLight.position - fPosition );
        float _attn = 1.0 / ( u_spotLight.attnk0 + u_spotLight.attnk1 * _dist + u_spotLight.attnk2 * _dist * _dist );

        // @OPTIM: should do this once for all contributions
        float _costheta = dot( -normalize( u_spotLight.position - fPosition ), normalize( u_spotLight.direction ) );
        float _epsilon = u_spotLight.innerCutoffCos - u_spotLight.outerCutoffCos;
        float _intensity = clamp( ( _costheta - u_spotLight.outerCutoffCos ) / _epsilon, 0.0, 1.0 );
        return pow( max( dot( normalize( reflect( -normalize( u_spotLight.position - fPosition ), fNormal ) ),
                              normalize( u_viewerPosition - fPosition ) ), 0.0f ), u_material.shininess ) * u_spotLight.intensity * _attn * _intensity * u_pointLight.specular;
    }

    // default value (constant everywhere) in case some configuration went wrong
    return vec3( 0.2f, 0.2f, 0.2f );
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

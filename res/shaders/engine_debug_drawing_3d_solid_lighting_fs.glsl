#version 330 core

in vec3 fPosition;
in vec3 fNormal;
in vec4 fColor;

out vec4 FragmentColor;

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

vec3 _computeColorWithDirectionalLight( DirectionalLight light, vec3 normal );
vec3 _computeColorWithPointLight( PointLight light, vec3 normal );
vec3 _computeColorWithSpotLight( SpotLight light, vec3 normal );

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

    FragmentColor = vec4( _resultColor, fColor.a );
}

vec3 _computeColorWithDirectionalLight( DirectionalLight light, vec3 normal )
{
    // precompute lightdir (recall this is from fragment to light "position"(inf))
    vec3 _lightdir = normalize( -light.direction );

    // compute ambient contribution factor
    vec3 _lAmbientComp = light.ambient * light.intensity;

    // compute diffuse contribution factor
    vec3 _lDiffuseComp = max( dot( normal, _lightdir ), 0.0f ) * light.diffuse * light.intensity;

    return fColor.rgb * ( _lAmbientComp + _lDiffuseComp );
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

    return fColor.rgb *( _lAmbientComp + _lDiffuseComp );
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

    return fColor.rgb * ( _lAmbientComp + _lDiffuseComp );
}
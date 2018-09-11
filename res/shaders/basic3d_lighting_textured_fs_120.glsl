#version 120


struct LLightDirectional
{
    vec3 direction;
    // phong model components
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int isActive;
};

struct LLightPoint
{
    vec3 position;
    // attenuation parameters
    float constant;
    float linear;
    float quadratic;
    // phong model components
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int isActive;
};

struct LLightSpot
{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;
    // phong model components
    float constant;
    float linear;
    float quadratic;
    // phong model components
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int isActive;
};

struct LMaterial
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 5
#define MAX_SPOT_LIGHTS 5

uniform LLightDirectional u_directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform LLightPoint u_pointLights[MAX_POINT_LIGHTS];
uniform LLightSpot u_spotLights[MAX_SPOT_LIGHTS];
uniform int u_numDirectionalLights;// To see how many of these have we created
uniform int u_numPointLights;// To see how many of these have we created
uniform int u_numSpotLights;// To see how many of these have we created
uniform vec3 u_globalAmbientLight;

uniform LMaterial u_material;
uniform vec3 u_viewPos;

varying vec3 vNormal;
varying vec3 vFragPos;

struct LFog
{
    int type;
    vec4 color;
    float density;

    float start;
    float end;

    int isActive;
};

uniform LFog u_fog = LFog( 0, vec4( 0.7, 0.7, 0.7, 0.5 ),
                           0.0, 18.0, 1.5, 0 );
uniform sampler2D u_matTex;

varying float vVisibility;

varying vec2 vTexCoord;

vec3 computeDirectionalContribution( LLightDirectional light, vec3 normal, vec3 viewDir );
vec3 computePointContribution( LLightPoint light, vec3 normal, vec3 fragPos, vec3 viewDir );
vec3 computeSpotContribution( LLightSpot light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main()
{

    vec3 _normal = normalize( vNormal );
    vec3 _viewDir = normalize( u_viewPos - vFragPos );

    vec3 _result = u_material.ambient * u_globalAmbientLight;

    for ( int q = 0; q < u_numDirectionalLights; q++ )
    {
        if ( q > MAX_DIRECTIONAL_LIGHTS - 1 )
        {
            break;
        }
        if ( u_directionalLights[q].isActive == 0 )
        {
            continue;
        }
        _result += computeDirectionalContribution( u_directionalLights[q],
                                                   _normal, _viewDir );
    }

    for ( int q = 0; q < u_numPointLights; q++ )
    {
        if ( q > MAX_POINT_LIGHTS - 1 )
        {
            break;
        }
        if ( u_pointLights[q].isActive == 0 )
        {
            continue;
        }
        _result += computePointContribution( u_pointLights[q],
                                              _normal, vFragPos, _viewDir );
    }

    for ( int q = 0; q < u_numSpotLights; q++ )
    {
        if ( q > MAX_SPOT_LIGHTS - 1 )
        {
            break;
        }
        if ( u_spotLights[q].isActive == 0 )
        {
            continue;
        }
        _result += computeSpotContribution( u_spotLights[q],
                                            _normal, vFragPos, _viewDir );
    }

    _result = vec3( texture2D( u_matTex, vTexCoord ) ) * _result;
    gl_FragColor = vec4( _result, 1.0 );
    gl_FragColor.rgb = mix( u_fog.color, gl_FragColor, vVisibility ).rgb;
}

vec3 computeDirectionalContribution( LLightDirectional light, vec3 normal, vec3 viewDir )
{
    vec3 _lightDir = normalize( -light.direction );
    // diffuse shadding
    float _diff = max( dot( normal, _lightDir ), 0.0 );
    // specular shading
    vec3 _reflectDir = reflect( -light.direction, normal );
    float _spec = pow( max( dot( viewDir, _reflectDir ), 0.0 ), u_material.shininess );
    // calculate components
    vec3 _ambient  = light.ambient * u_material.ambient;
    vec3 _diffuse  = _diff * light.diffuse * u_material.diffuse;
    vec3 _specular = _spec * light.specular * u_material.specular;
    
    return _ambient + _diffuse + _specular;
}

vec3 computePointContribution( LLightPoint light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 _lightDir = normalize( light.position - fragPos );
    // diffuse shadding
    float _diff = max( dot( normal, _lightDir ), 0.0 );
    // specular shading
    vec3 _reflectDir = reflect( -_lightDir, normal );
    float _spec = pow( max( dot( viewDir, _reflectDir ), 0.0 ), u_material.shininess );
    // attenuation
    float _distance = length( light.position - fragPos );
    float _attenuation = 1.0 / ( light.constant + 
                                 light.linear * _distance + 
                                 light.quadratic * _distance * _distance );
    // calculate components
    vec3 _ambient  = light.ambient * u_material.ambient;
    vec3 _diffuse  = _diff * light.diffuse * u_material.diffuse;
    vec3 _specular = _spec * light.specular * u_material.specular;
    _ambient  *= _attenuation;
    _diffuse  *= _attenuation;
    _specular *= _attenuation;
    
    return _ambient + _diffuse + _specular;
}

vec3 computeSpotContribution( LLightSpot light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 _lightDir = normalize( light.position - fragPos );
    // diffuse shadding
    float _diff = max( dot( normal, _lightDir ), 0.0 );
    // specular shading
    vec3 _reflectDir = reflect( -_lightDir, normal );
    float _spec = pow( max( dot( viewDir, _reflectDir ), 0.0 ), u_material.shininess );
    // attenuation
    float _distance = length( light.position - fragPos );
    float _attenuation = 1.0 / ( light.constant + 
                                 light.linear * _distance + 
                                 light.quadratic * _distance * _distance );
    // spotlight intensity
    float _ctheta = dot( _lightDir, normalize( -light.direction ) );
    float _epsilon = light.cutOff - light.outerCutOff;
    float _intensity = clamp( ( _ctheta - light.outerCutOff ) / _epsilon, 0.0, 1.0 );
    // calculate components
    vec3 _ambient  = light.ambient * u_material.ambient;
    vec3 _diffuse  = _diff * light.diffuse * u_material.diffuse;
    vec3 _specular = _spec * light.specular * u_material.specular;
    _ambient  *= _attenuation * pow( _intensity, 15 );
    _diffuse  *= _attenuation * pow( _intensity, 15 );
    _specular *= _attenuation * pow( _intensity, 15 );
    
    return _ambient + _diffuse + _specular;
}
#version 330 core


struct LLightDirectional
{
    vec3 direction;
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

uniform LLightDirectional u_directionalLight;
uniform vec3 u_globalAmbientLight;

uniform LMaterial u_material;
uniform vec3 u_viewPos;
uniform vec3 u_lightPos;
uniform sampler2D u_shadowMap;
uniform sampler2D u_texture;

in vec3 vNormal;
in vec3 vFragPos;
in vec4 vPosLightSpace;
in vec2 vTexCoord;
out vec4 color;

vec3 computeDirectionalContribution( LLightDirectional light, vec3 normal, vec3 viewDir, float shadowValue );

float calculateShadow( vec4 fragPosLightSpace, vec3 normal, vec3 lightDir )
{
    // perspective division
    vec3 _projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform from the range [-1, 1] to the range [0,1]
    _projCoords = _projCoords * 0.5 + 0.5;

    float _closestDepth = texture( u_shadowMap, _projCoords.xy ).r; // depth from shadow map
    float _currentDepth = _projCoords.z; // depth for the current fragment respect to the light point of view

    float _bias = max( 0.005 * ( 1.0 - dot( normal, lightDir ) ), 0.0005 );

    float _shadow = ( _currentDepth - _bias ) > _closestDepth ? 1.0 : 0.0; // check if fragment is behind something

    if ( _projCoords.z > 1.0 )
    {
        _shadow = 0.0;
    }

    return _shadow;
}

void main()
{

    vec3 _normal = normalize( vNormal );
    vec3 _viewDir = normalize( u_viewPos - vFragPos );

    vec3 _result = u_material.ambient * u_globalAmbientLight;


    float _shadowValue = calculateShadow( vPosLightSpace, _normal, u_directionalLight.direction );

    _result += computeDirectionalContribution( u_directionalLight,
                                               _normal, _viewDir,
                                               _shadowValue );

    _result = vec3( texture2D( u_texture, vTexCoord ) ) * _result;

    color = vec4( _result, 1.0 );
}

vec3 computeDirectionalContribution( LLightDirectional light, vec3 normal, vec3 viewDir, float shadowValue )
{
    vec3 _lightDir = normalize( -light.direction );
    // diffuse shadding
    float _diff = max( dot( normal, _lightDir ), 0.0 );
    // specular shading
    vec3 _reflectDir = reflect( _lightDir, normal );
    float _spec = pow( max( dot( viewDir, _reflectDir ), 0.0 ), u_material.shininess );
    // calculate components
    vec3 _ambient  = light.ambient * u_material.ambient;
    vec3 _diffuse  = ( 1.0 - shadowValue ) * _diff * light.diffuse * u_material.diffuse;
    vec3 _specular = ( 1.0 - shadowValue ) * _spec * light.specular * u_material.specular;
    
    return _ambient + _diffuse + _specular;
}
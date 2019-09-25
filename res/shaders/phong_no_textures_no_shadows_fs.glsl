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

in vec3 vNormal;
in vec3 vFragPos;
out vec4 color;

vec3 computeDirectionalContribution( LLightDirectional light, vec3 normal, vec3 viewDir );

void main()
{

    vec3 _normal = normalize( vNormal );
    vec3 _viewDir = normalize( u_viewPos - vFragPos );

    vec3 _result = u_material.ambient * u_globalAmbientLight;

    if ( u_directionalLight.isActive == 1 )
    {
        _result += computeDirectionalContribution( u_directionalLight,
                                                    _normal, _viewDir );
    }

    color = vec4( _result, 1.0 );
}

vec3 computeDirectionalContribution( LLightDirectional light, vec3 normal, vec3 viewDir )
{
    vec3 _lightDir = normalize( -light.direction );
    // diffuse shadding
    float _diff = max( dot( normal, _lightDir ), 0.0 );
    // specular shading
    vec3 _reflectDir = reflect( _lightDir, normal );
    float _spec = pow( max( dot( viewDir, _reflectDir ), 0.0 ), u_material.shininess );
    // calculate components
    vec3 _ambient  = light.ambient * u_material.ambient;
    vec3 _diffuse  = _diff * light.diffuse * u_material.diffuse;
    vec3 _specular = _spec * light.specular * u_material.specular;
    
    return _ambient + _diffuse + _specular;
}
#version 120


struct LMaterial
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct LLightDirectional
{
    vec3 direction;
    // phong model components
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_DIRECTIONAL_LIGHTS 2

uniform LLightDirectional u_directionalLight[MAX_DIRECTIONAL_LIGHTS];
uniform int u_numDirectionalLights;

uniform vec3 u_globalAmbientLight;

#define MAX_MATERIALS 7
uniform vec3 u_viewPos;

varying vec3 fNormal;
varying vec3 fFragPos;
varying vec2 fTexCoord;

uniform LMaterial u_material[MAX_MATERIALS];
uniform sampler2D u_textures[MAX_MATERIALS];

uniform int u_variation = 0;

const vec4 TERRAIN_RANGES = vec4( 0.01f, 2.0f, 6.0f, 50.0f );
const ivec4 TEXTURE_MAP = ivec4( 0, /* region 0 */
                                 1, /* region 1 */
                                 2, /* region 2 has 4 variations */
                                 6 ); /* region 3 */
const ivec4 TEXTURE_MAP_BEF = ivec4( 0, /* region 0 */
                                     0, /* region 0 */
                                     1, /* region 2 has 4 variations */
                                     2 ); /* region 3 */

vec3 computeDirectionalContribution( LLightDirectional light, LMaterial material,
                                     vec3 normal, vec3 viewDir );

void main()
{

    vec3 _normal = normalize( fNormal );
    vec3 _viewDir = normalize( u_viewPos - fFragPos );

    int _materialId = TEXTURE_MAP[3];
    int _materialIdBef = TEXTURE_MAP_BEF[3] + u_variation;
    int _zone = 3;

    float _blendValue = 0.0f;

    for ( int q = 0; q < 3; q++ )
    {
        if ( fFragPos.y < TERRAIN_RANGES[q] ) 
        { 
            if ( q == 2 ) /* if in region 2, we have 4 variations */
            {
                _materialId = TEXTURE_MAP[q] + u_variation;
            }
            else
            {
                _materialId = TEXTURE_MAP[q]; 
            }

            _materialIdBef = TEXTURE_MAP_BEF[q]; 

            _zone = q;

            break;
        }
    }

    if ( _zone > 0 )
    {
        float _percent = 1 - ( fFragPos.y - TERRAIN_RANGES[_zone - 1] ) / ( TERRAIN_RANGES[_zone] - TERRAIN_RANGES[_zone - 1] );
        _blendValue = pow( _percent, 10.0 );
    }


    vec3 _result = u_material[_materialId].ambient * u_globalAmbientLight;

    for ( int q = 0; q < u_numDirectionalLights; q++ )
    {
        if ( q > MAX_DIRECTIONAL_LIGHTS - 1 )
        {
            break;
        }

        _result += computeDirectionalContribution( u_directionalLight[q], u_material[_materialId], 
                                                   _normal, _viewDir );
    }

    vec4 _blendedTex = mix( texture2D( u_textures[_materialId], fTexCoord ),
                            texture2D( u_textures[_materialIdBef], fTexCoord ),
                            _blendValue );

    _result = vec3( _blendedTex ) * _result;

    gl_FragColor = vec4( _result, 1.0f );
}


vec3 computeDirectionalContribution( LLightDirectional light, LMaterial material, 
                                     vec3 normal, vec3 viewDir )
{
    vec3 _lightDir = normalize( -light.direction );
    // diffuse shadding
    float _diff = max( dot( normal, _lightDir ), 0.0 );
    // specular shading
    vec3 _reflectDir = reflect( -light.direction, normal );
    float _spec = pow( max( dot( viewDir, _reflectDir ), 0.0 ), material.shininess );
    // calculate components
    vec3 _ambient  = light.ambient * material.ambient;
    vec3 _diffuse  = _diff * light.diffuse * material.diffuse;
    vec3 _specular = _spec * light.specular * material.specular;
    
    return _ambient + _diffuse + _specular;
}
#version 330 core

in vec3 fSampleDir;
out vec4 fColor;

uniform samplerCube u_textureSkybox;

struct Fog
{
    int enabled;
    vec3 color;
    float lowerLimit;
    float upperLimit;
};
uniform Fog u_fog;

void main()
{
    /* compute a factor to mix the fog and skybox colors */
    float _factor = 1.0f;
    if ( u_fog.enabled == 1 )
        _factor = ( fSampleDir.y - u_fog.lowerLimit ) / ( u_fog.upperLimit - u_fog.lowerLimit );

    fColor = mix( vec4( u_fog.color, 1.0f ), texture( u_textureSkybox, fSampleDir ), clamp( _factor, 0.0f, 1.0f ) );
}
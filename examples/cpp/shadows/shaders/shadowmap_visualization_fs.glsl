#version 330 core

in vec2 fTexcoord;
out vec4 fColor;

uniform sampler2D u_shadowmap;

uniform int u_linearizeDepth;
uniform float u_znear;
uniform float u_zfar;

float linearizeDepth( float depth )
{
    float z = depth * 2.0 - 1.0; // to NDC range [-1,1]
    return ( 2.0 * u_znear * u_zfar ) / ( ( u_zfar + u_znear ) - z * ( u_zfar - u_znear ) );
}

void main()
{
    if ( u_linearizeDepth == 1 )
        fColor = vec4( vec3( linearizeDepth( texture( u_shadowmap, fTexcoord ).r ) / u_zfar ) , 1.0f );
    else
        fColor = vec4( vec3( texture( u_shadowmap, fTexcoord ).r ) , 1.0f );
}
#version 330 core

out vec4 fColor;

uniform float u_znear;
uniform float u_zfar;

uniform float u_zmin;
uniform float u_zmax;
uniform vec3 u_zminColor;
uniform vec3 u_zmaxColor;

float _linearizeDepth( float depth );

void main()
{
    float _zFactor = ( clamp( _linearizeDepth( gl_FragCoord.z ), u_zmin, u_zmax ) - u_zmin ) / ( u_zmax - u_zmin );
    fColor = vec4( mix( u_zminColor, u_zmaxColor, _zFactor ), 1.0f );
}

float _linearizeDepth( float depth )
{
    float z = depth * 2.0 - 1.0; // to NDC range [-1,1]
    return ( 2.0 * u_znear * u_zfar ) / ( ( u_zfar + u_znear ) - z * ( u_zfar - u_znear ) );
}
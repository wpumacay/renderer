#version 120

varying vec3 fNormal;

uniform vec3 u_color;

void main()
{
    gl_FragColor = vec4( u_color, 1.0f );
}
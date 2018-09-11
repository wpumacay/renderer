#version 120

varying vec3 fNormal;
varying vec2 fTexCoord;

uniform vec3 u_color;
uniform sampler2D u_texture;

void main()
{
    gl_FragColor = vec4( vec3( texture2D( u_texture, fTexCoord ) ) * u_color, 1.0f );
}
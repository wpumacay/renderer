#version 120

uniform vec3 u_viewPos;
uniform samplerCube cubeTexture;

varying vec3 vNormal;
varying vec3 vFragPos;

void main()
{

    vec3 _normal = normalize( vNormal );
    vec3 _viewDir = normalize( vFragPos - u_viewPos );
    vec3 _sampleDir = reflect( _viewDir, _normal );

    gl_FragColor = vec4( textureCube( cubeTexture, _sampleDir ).rgb, 1.0 );
}
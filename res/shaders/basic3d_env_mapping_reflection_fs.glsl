#version 330 core

uniform vec3 u_viewPos;
uniform samplerCube cubeTexture;

in vec3 vNormal;
in vec3 vFragPos;
out vec4 color;

void main()
{

    vec3 _normal = normalize( vNormal );
    vec3 _viewDir = normalize( u_viewPos - vFragPos );
    vec3 _sampleDir = reflect( _viewDir, _normal );

    color = vec4( texture( cubeTexture, _sampleDir ).rgb, 1.0 );
}

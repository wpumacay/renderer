#version 120

varying vec3 vTexDir;

uniform samplerCube cubeTexture;

void main()
{    
    gl_FragColor = textureCube( cubeTexture, vTexDir );
}
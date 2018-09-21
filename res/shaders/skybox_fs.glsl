#version 330 core

in vec3 vTexDir;
out vec4 FragColor;

uniform samplerCube cubeTexture;

void main()
{    
    FragColor = texture( cubeTexture, vTexDir );
}
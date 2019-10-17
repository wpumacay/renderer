#version 330 core

in vec3 fSampleDir;
out vec4 fColor;

uniform samplerCube u_textureSkybox;

void main()
{
    fColor = texture( u_textureSkybox, fSampleDir );
}
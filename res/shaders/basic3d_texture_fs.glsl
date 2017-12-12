#version 330 core

in vec2 vTexCoord;
out vec4 color;

uniform vec3 u_color;

uniform sampler2D u_texture;

void main()
{
    color = texture( u_texture, vTexCoord );// * vec4( u_color, 1 );
}
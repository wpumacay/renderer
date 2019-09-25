#version 330 core

uniform sampler2D u_texture;

in vec3 fColor;
in vec2 fUV;

out vec4 color;

void main()
{
    color = texture( u_texture, fUV ) * vec4( fColor, 1.0 );
}

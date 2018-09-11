#version 330 core

out vec4 color;

in vec2 vTexCoords;

uniform sampler2D screenTexture;
uniform int u_texChannel;

void main()
{
    vec3 col = texture( screenTexture, vTexCoords ).rgb;

    //u_texChannel = max( u_texChannel, 0 );
    //u_texChannel = min( u_texChannel, 2 );

    color = vec4( vec3( col[u_texChannel] ), 1.0 );
} 


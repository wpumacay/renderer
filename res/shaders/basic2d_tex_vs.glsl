#version 330 core

layout ( location = 0 ) in vec2 pos;
layout ( location = 1 ) in vec3 color;
layout ( location = 2 ) in vec2 uv;
 
out vec3 fColor;
out vec2 fUV;

void main() 
{
     gl_Position = vec4( pos, 0.0f, 1.0f );
     fColor = color;
     fUV = uv;
}
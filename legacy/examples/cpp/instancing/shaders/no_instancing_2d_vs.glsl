#version 330 core

layout ( location = 0 ) in vec2 position;
layout ( location = 1 ) in vec3 color;

uniform vec2 u_screenPosition;

out vec3 fColor;

void main()
{
    gl_Position = vec4( u_screenPosition + position, 0.0f, 1.0f );
    fColor = color;
}

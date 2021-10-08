#version 330 core

layout ( location = 0 ) in vec2 position;
layout ( location = 1 ) in vec3 color;
layout ( location = 2 ) in vec2 screenPosition;

out vec3 fColor;

void main()
{
    gl_Position = vec4( screenPosition + position, 0.0f, 1.0f );
    fColor = color;
}

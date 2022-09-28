#version 330 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

uniform float u_time;
uniform float u_radius;

out vec2 f_texCoord;

void main() {
    float pos_x = position.x + clamp(u_radius, -0.2, 0.2) * cos(u_time);
    float pos_y = position.y + clamp(u_radius, -0.2, 0.2) * sin(u_time);
    gl_Position = vec4(pos_x, pos_y, 0.0f, 1.0f);
    f_texCoord = texCoord;
}

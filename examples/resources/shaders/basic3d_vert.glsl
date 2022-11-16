#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_proj_matrix;

void main() {
    gl_Position = u_proj_matrix * u_view_matrix * u_model_matrix * vec4(position, 1.0f);
}

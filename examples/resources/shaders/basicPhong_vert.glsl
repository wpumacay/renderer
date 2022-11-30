#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

// Transform that sets the vertices from local space to world space
uniform mat4 u_model_matrix;
// Matrix used to transform the normals of the geometry being rendered
uniform mat4 u_normal_matrix;
// Combined view and projection matrices
uniform mat4 u_view_proj_matrix;

// Fragment position
out vec3 f_position;
// Fragment normal
out vec3 f_normal;
// Fragment texture coordinates
out vec2 f_uv;

void main() {
    gl_Position = u_view_proj_matrix * u_model_matrix * vec4(position, 1.0f);
    f_position = vec3(u_model_matrix * vec4(position, 1.0f));
    f_normal = mat3(u_normal_matrix) * normal;
    f_uv = uv;
}

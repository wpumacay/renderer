#version 330 core

in vec3 f_position;
in vec3 f_normal;
in vec2 f_uv;

out vec4 f_color;

// Uniforms for a directional light source
struct DirectionalLight {
    // The direction this light comes from
    vec3 direction;
    // The color of this light
    vec3 color;
    // The intensity (factor) of this light
    float intensity;
};
uniform DirectionalLight u_dir_light;

// The color of the object being rendered
uniform vec3 u_object_color;
// The viewer position (camera position in world space)
uniform vec3 u_viewer_position;
// The global ambient light component
uniform vec3 u_ambient_light;

vec3 compute_directional_light(DirectionalLight light,
                               vec3 normal_dir,
                               vec3 view_dir);

void main() {
    vec3 total_color = vec3(0.0f);
    vec3 normal_dir = normalize(f_normal);
    vec3 view_dir = normalize(u_viewer_position - f_position);

    // Global ambient contribution
    total_color += u_object_color * u_ambient_light;

    // Directional light contribution
    total_color += compute_directional_light(u_dir_light,
                                             normal_dir,
                                             view_dir);

    f_color = vec4(total_color, 1.0f);
}

vec3 compute_directional_light(DirectionalLight light,
                               vec3 normal_dir,
                               vec3 view_dir) {
    vec3 light_dir = normalize(-light.direction);

    vec3 diffuse_comp = max(dot(normal_dir, light_dir), 0.0f) *
                                    light.color * light.intensity;

    vec3 specular_comp = pow(max(dot(reflect(-light_dir, normal_dir),
                                     view_dir), 0.0f), 32.0f)
                                        * light.color * light.intensity;

    return diffuse_comp * u_object_color + specular_comp * u_object_color;
}

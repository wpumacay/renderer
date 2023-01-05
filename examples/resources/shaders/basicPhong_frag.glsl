#version 330 core

in vec3 f_position;
in vec3 f_normal;
in vec2 f_uv;

out vec4 f_color;

// NOTE(wilbert): To ease testing functionality with the examples we're using
// a conditional branch (if) here to check for the light type, and then take the
// appropriate path of light computations. We should avoid using branching in
// shader code, as if might kill performance due various issues

// Uniforms for a directional light source
struct DirectionalLight {
    // The direction this light comes from
    vec3 direction;
    // The color of this light
    vec3 color;
    // The intensity (factor) of this light
    float intensity;
    // Whether or not this light is enabled
    int enabled;
};
uniform DirectionalLight u_dir_light;

// Uniforms for a point light source
struct PointLight {
    // The position of this light in world space
    vec3 position;
    // The color of this light
    vec3 color;
    // The intensity (factor) of this light
    float intensity;
    // Whether or not this light is enabled
    int enabled;
};
uniform PointLight u_point_light;

// Uniforms for a spot light source
struct SpotLight {
    // The position of this light in world space
    vec3 position;
    // The direction(axis) of the light cone
    vec3 direction;
    // The cosine of the inner cutoff angle
    float innerCutOffCos;
    // The cosine of the outer cutoff angle
    float outerCutOffCos;
    // The color of this light
    vec3 color;
    // the intensity (factor) of this light
    float intensity;
    // Whether or not this light is enabled
    int enabled;
};
uniform SpotLight u_spot_light;

// The color of the object being rendered
uniform vec3 u_object_color = vec3(1.0f, 0.5f, 0.31f);
// The viewer position (camera position in world space)
uniform vec3 u_viewer_position = vec3(1.0f, 1.0f, 3.0f);
// The global ambient light component
uniform vec3 u_ambient_light = vec3(0.1f, 0.1f, 0.1f);

vec3 compute_directional_light(const in DirectionalLight light,
                               const in vec3 normal_dir,
                               const in vec3 view_dir);

vec3 compute_point_light(const in PointLight light,
                         const in vec3 normal_dir,
                         const in vec3 view_dir);

vec3 compute_spot_light(const in SpotLight light,
                        const in vec3 normal_dir,
                        const in vec3 view_dir);

float compute_diffuse(const in vec3 normal_dir, const in vec3 light_dir) {
    return max(dot(normal_dir, -light_dir), 0.0f);
}

float compute_specular(const in vec3 normal_dir,
                       const in vec3 light_dir,
                       const in vec3 view_dir) {
    return pow(max(dot(reflect(light_dir, normal_dir), view_dir), 0.0f), 32.0f);
}

void main() {
    vec3 total_color = vec3(0.0f);
    vec3 normal_dir = normalize(f_normal);
    vec3 view_dir = normalize(u_viewer_position - f_position);

    // Global ambient contribution
    total_color += u_object_color * u_ambient_light;

    // Directional light contribution
    if (u_dir_light.enabled != 0) {
        total_color += compute_directional_light(u_dir_light, normal_dir, view_dir);
    }

    // Point light contribution
    if (u_point_light.enabled != 0) {
        total_color += compute_point_light(u_point_light, normal_dir, view_dir);
    }

    // Spot light contribution
    if (u_spot_light.enabled != 0) {
        total_color += compute_spot_light(u_spot_light, normal_dir, view_dir);
    }

    f_color = vec4(total_color, 1.0f);
}

vec3 compute_directional_light(const in DirectionalLight light,
                               const in vec3 normal_dir,
                               const in vec3 view_dir) {
    vec3 light_dir = normalize(light.direction);

    vec3 diffuse_comp = compute_diffuse(normal_dir, light_dir) *
                            light.color * light.intensity;

    vec3 specular_comp = compute_specular(normal_dir, light_dir, view_dir) *
                            light.color * light.intensity;

    return diffuse_comp * u_object_color + specular_comp * u_object_color;
}

vec3 compute_point_light(const in PointLight light,
                         const in vec3 normal_dir,
                         const in vec3 view_dir) {
    vec3 light_dir = normalize(f_position - light.position);

    vec3 diffuse_comp = compute_diffuse(normal_dir, light_dir) *
                            light.color * light.intensity;

    vec3 specular_comp = compute_specular(normal_dir, light_dir, view_dir) *
                            light.color * light.intensity;

    return diffuse_comp * u_object_color + specular_comp * u_object_color;
}

vec3 compute_spot_light(const in SpotLight light,
                        const in vec3 normal_dir,
                        const in vec3 view_dir) {
    vec3 light_dir = normalize(f_position - light.position);

    float cos_theta = dot(normalize(f_position - light.position),
                          normalize(light.direction));

    const float cos_phi_in = light.innerCutOffCos;
    const float cos_phi_out = light.outerCutOffCos;

    float cos_factor = (cos_theta - cos_phi_out) / (cos_phi_in - cos_phi_out);
    float spot_intensity = clamp(cos_factor, 0.0f, 1.0f);

    vec3 diffuse_comp = compute_diffuse(normal_dir, light_dir) *
                            light.color * light.intensity * spot_intensity;

    vec3 specular_comp = compute_specular(normal_dir, light_dir, view_dir) *
                            light.color * light.intensity * spot_intensity;

    return (diffuse_comp + specular_comp) * u_object_color;
}

#include <renderer/light/light_t.hpp>

namespace renderer {

auto ToString(const eLightType& light_type) -> std::string {
    switch (light_type) {
        case eLightType::DIRECTIONAL:
            return "directional";
        case eLightType::POINT:
            return "point";
        case eLightType::SPOT:
            return "spot";
    }

    return "undefined";
}

DirectionalLight::DirectionalLight(const Vec3& p_direction, const Vec3& p_color,
                                   float p_intensity) {
    type = eLightType::DIRECTIONAL;
    direction = p_direction;
    color = p_color;
    intensity = p_intensity;
}

PointLight::PointLight(const Vec3& p_position, const Vec3& p_color,
                       float p_intensity, float p_attn_constant,
                       float p_attn_linear, float p_attn_quadratic) {
    type = eLightType::POINT;
    position = p_position;
    color = p_color;
    intensity = p_intensity;
    attnConstant = p_attn_constant;
    attnLinear = p_attn_linear;
    attnQuadratic = p_attn_quadratic;
}

SpotLight::SpotLight(const Vec3& p_position, const Vec3& p_direction,
                     float p_inner_angle, float p_outer_angle,
                     const Vec3& p_color, float p_intensity,
                     float p_attn_constant, float p_attn_linear,
                     float p_attn_quadratic) {
    type = eLightType::SPOT;
    position = p_position;
    direction = p_direction;
    innerCutoffAngle = p_inner_angle;
    outerCutoffAngle = p_outer_angle;
    color = p_color;
    intensity = p_intensity;
    attnConstant = p_attn_constant;
    attnLinear = p_attn_linear;
    attnQuadratic = p_attn_quadratic;
}

}  // namespace renderer

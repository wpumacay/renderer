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
}

DirectionalLight::DirectionalLight(const Vec3& direction, const Vec3& color,
                                   float intensity) {
    this->type = eLightType::DIRECTIONAL;
    this->direction = direction;
    this->color = color;
    this->intensity = intensity;
}

PointLight::PointLight(const Vec3& position, const Vec3& color, float intensity,
                       float attn_constant, float attn_linear,
                       float attn_quadratic) {
    this->type = eLightType::POINT;
    this->position = position;
    this->color = color;
    this->intensity = intensity;
    this->attnConstant = attn_constant;
    this->attnLinear = attn_linear;
    this->attnQuadratic = attn_quadratic;
}

SpotLight::SpotLight(const Vec3& position, const Vec3& direction,
                     float inner_angle, float outer_angle, const Vec3& color,
                     float intensity, float attn_constant, float attn_linear,
                     float attn_quadratic) {
    this->type = eLightType::SPOT;
    this->position = position;
    this->direction = direction;
    this->innerCutoffAngle = inner_angle;
    this->outerCutoffAngle = outer_angle;
    this->color = color;
    this->intensity = intensity;
    this->attnConstant = attn_constant;
    this->attnLinear = attn_linear;
    this->attnQuadratic = attn_quadratic;
}

}  // namespace renderer

#pragma once

#include <string>

#include <renderer/common.hpp>

namespace renderer {

/// All available light types in the scene
enum class eLightType {
    /// Directional light type, gives off light as if it were parallel beams
    DIRECTIONAL,
    /// Point light type, gives off light equally in all directions
    POINT,
    /// Spot light type, gives off light according to a light cone
    SPOT,
};

/// Returns the string representation of the given light type
auto ToString(const eLightType& light_type) -> std::string;

/// Representation of a light source in the scene
class Light {
    // cppcheck-suppress unknownMacro
    DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Light)

    DEFINE_SMART_POINTERS(Light)

 public:
    /// The type of this light source
    eLightType type = eLightType::DIRECTIONAL;
    /// Position of this light source (valid for spot and point types)
    Vec3 position;
    /// Direction of this light source (valid for directional and spot types)
    Vec3 direction;
    /// Color of this light source
    Vec3 color = {1.0F, 1.0F, 1.0F};
    /// Intensity of this light source
    float intensity = 0.0F;
    /// Attenuation parameter (constant factor)
    float attnConstant = 1.0F;
    /// Attenuation parameter (linear factor)
    float attnLinear = 0.0F;
    /// Attenuation parameter (quadratic factor)
    float attnQuadratic = 0.0F;
    /// Inner cutoff angle of the light cone (valid for spot lights)
    float innerCutoffAngle = PI / 4.0F;
    /// Outer cutoff angle of the light cone (valid for spot lights)
    float outerCutoffAngle = PI / 3.0F;

 public:
    /// Creates a light with default configuration
    Light() = default;

    /// Releases all resources allocated by this light source
    virtual ~Light() = default;
};

/// Representation of a directional light source
class DirectionalLight : public Light {
    DEFINE_SMART_POINTERS(DirectionalLight)

 public:
    /// Creates a directional light source
    /// \param[in] direction The direction of the light beams from the source
    /// \param[in] color The color of the light emmited by the light source
    /// \param[in] intensity The intensity of the light source
    explicit DirectionalLight(const Vec3& p_direction,
                              const Vec3& p_color = {1.0F, 1.0F, 1.0F},
                              float p_intensity = 1.0F);
};

/// Representation of a point light source
class PointLight : public Light {
    DEFINE_SMART_POINTERS(PointLight)

 public:
    /// Creates a point light source
    /// \param[in] position The positions of the light source
    /// \param[in] color The color of the light emmited by the light source
    /// \param[in] intensity The intensity of the light source
    /// \param[in] attn_constant The attenuation constant factor
    /// \param[in] attn_linear The attenuation linear factor
    /// \param[in] attn_quadratic The attenuation quadratic factor
    explicit PointLight(const Vec3& p_position,
                        const Vec3& p_color = {1.0F, 1.0F, 1.0F},
                        float p_intensity = 1.0F, float p_attn_constant = 1.0F,
                        float p_attn_linear = 0.0F,
                        float p_attn_quadratic = 0.0F);
};

/// Representation of a spot light source
class SpotLight : public Light {
    DEFINE_SMART_POINTERS(SpotLight)

 public:
    /// Creates a spot light source
    /// \param[in] position The positions of the light source
    /// \param[in] direction The direction of the light source
    /// \param[in] inner_angle The inner angle of the inner light cone
    /// \param[in] outer_angle The outer angle of the outer light cone
    /// \param[in] color The color of the light emmited by the light source
    /// \param[in] intensity The intensity of the light source
    /// \param[in] attn_constant The attenuation constant factor
    /// \param[in] attn_linear The attenuation linear factor
    /// \param[in] attn_quadratic The attenuation quadratic factor
    explicit SpotLight(const Vec3& p_position, const Vec3& p_direction,
                       float p_inner_angle = PI / 4,
                       float p_outer_angle = PI / 3,
                       const Vec3& p_color = {1.0F, 1.0F, 1.0F},
                       float p_intensity = 1.0F, float p_attn_constant = 1.0F,
                       float p_attn_linear = 0.0F,
                       float p_attn_quadratic = 0.0F);
};

}  // namespace renderer

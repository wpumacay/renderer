#pragma once

// Implementation based on Three.JS OrbitControls
// mrdoob @ three.js/blob/dev/examples/jsm/controls/OrbitControls.js

#include <utility>
#include <limits>
#include <string>

#include <renderer/common.hpp>
#include <utils/common.hpp>
#include <math/utils/spherical_coordinates.hpp>

#include <renderer/camera/camera_controller_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

/// Available states for the orbit camera controller
enum class eOrbitState {
    IDLE,    /// The user doesn't interact nor use the controller
    ROTATE,  /// The user is using the left mouse button to rotate the view
    PAN,     /// The user is using the right mouse button to pan the view
    DOLLY    /// The user is using the mouse wheel to zoom in/out the view
};

/// Returns the string representation of the given orbit state enum
auto ToString(eOrbitState state) -> std::string;

class OrbitCameraController : public ICameraController {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(OrbitCameraController)

    DEFINE_SMART_POINTERS(OrbitCameraController)

 public:
    explicit OrbitCameraController(Camera::ptr camera, float viewportWidth,
                                   float viewportHeight);

    ~OrbitCameraController() override = default;

    auto UpdateViewport(float width, float height) -> void;

    auto Update() -> void override;

    auto OnKeyCallback(int key, int action, int modifier) -> void override {}

    auto OnMouseButtonCallback(int button, int action, double x, double y)
        -> void override;

    auto OnMouseMoveCallback(double x, double y) -> void override;

    auto OnScrollCallback(double xOff, double yOff) -> void override {}

    auto state() const -> eOrbitState { return m_State; }

 public:
    /// How far can we orbit vertically (lower limit)
    float minPolar = 0.0F;
    /// How far can we orbit vertically (upper limit)
    float maxPolar = static_cast<float>(math::PI);
    /// How far can we orbit horizontally (lower limit)
    float minAzimuth = -std::numeric_limits<float>::infinity();
    /// How far can we orbit horizontally (upper limit)
    float maxAzimuth = std::numeric_limits<float>::infinity();
    /// How far can we dolly out
    float minDistance = 0;
    /// How far can we dolly in
    float maxDistance = std::numeric_limits<float>::infinity();
    /// The speed at which a rotation with the mouse can be achieved
    float rotateSpeed = 1.0F;
    /// The location where the object orbits around
    Vec3 target;

 private:
    /// Current state of this orbit controller
    eOrbitState m_State = eOrbitState::IDLE;

    /// Current position in spherical coordinates
    math::SphericalCoords<float> m_Spherical;
    math::SphericalCoords<float> m_SphericalDelta;

    /// Where the mouse cursor is when pressed at the start of a rotation
    Vec2 m_RotateStart;
    /// Where the mouse cursor currently is when a move move event is received
    Vec2 m_RotateCurrent;
    /// Keeps tracks of the delta movement when doing a rotation
    Vec2 m_RotateDelta;

    /// The width currently being used by the screen when rendering
    float m_ViewportWidth = 800.0F;
    /// The height currently being used by the screen when rendering
    float m_ViewportHeight = 600.0F;
};

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

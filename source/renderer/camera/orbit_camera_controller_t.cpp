#include <cmath>
#include <algorithm>

#include <renderer/camera/orbit_camera_controller_t.hpp>
#include <renderer/input/buttons.hpp>

namespace renderer {

OrbitCameraController::OrbitCameraController(Camera::ptr camera,
                                             float viewportWidth,
                                             float viewportHeight)
    : ICameraController(std::move(camera)),
      m_ViewportWidth(viewportWidth),
      m_ViewportHeight(viewportHeight) {}

auto OrbitCameraController::UpdateViewport(float width, float height) -> void {
    m_ViewportWidth = width;
    m_ViewportHeight = height;
}

auto OrbitCameraController::Update() -> void {
    auto offset = m_Camera->position() - m_Target;

    m_Spherical.SetFromCartesian(offset);
    m_Spherical.theta += m_SphericalDelta.theta;
    m_Spherical.phi += m_SphericalDelta.phi;

    constexpr auto TWO_PI = static_cast<float>(math::PI);

    auto min = m_MinTheta;
    auto max = m_MaxTheta;
    if (std::isfinite(min) || std::isfinite(max)) {
        // Make sure (min, max) are in the range [-PI, PI]
        min = (min < -PI) ? (min + TWO_PI) : min;
        min = (min > PI) ? (min - TWO_PI) : min;
        max = (max < -PI) ? (max + TWO_PI) : max;
        max = (max > PI) ? (max - TWO_PI) : max;

        // By updating the (min, max) we might have flipped the order, so take
        // this into account when updating the azimuthal angle
        if (min <= max) {
            m_Spherical.theta = std::max(min, std::min(max, m_Spherical.theta));
        } else {
            m_Spherical.theta = (m_Spherical.theta > (min + max) / 2.0F)
                                    ? std::max(min, m_Spherical.theta)
                                    : std::min(max, m_Spherical.theta);
        }
    }
    m_Spherical.phi = std::max(m_MinPhi, std::min(m_MaxPhi, m_Spherical.phi));
    m_Spherical.MakeSafe();

    m_Spherical.rho =
        std::max(m_MinDistance, std::min(m_MaxDistance, m_Spherical.rho));

    offset = m_Spherical.GetCartesian();

    m_Camera->SetPosition(m_Target + offset);
    // m_Camera->SetTarget(new_target)

    m_SphericalDelta.rho = 0.0F;
    m_SphericalDelta.phi = 0.0F;
    m_SphericalDelta.theta = 0.0F;
}

auto OrbitCameraController::OnMouseButtonCallback(int button, int action,
                                                  double x, double y) -> void {
    if (action == button_action::BUTTON_RELEASED) {
        m_State = eOrbitState::IDLE;
    } else if (action == button_action::BUTTON_PRESSED) {
        switch (button) {
            case mouse::BUTTON_LEFT:
                // Handle rotation
                m_RotateStart.x() = static_cast<float>(x);
                m_RotateStart.y() = static_cast<float>(y);

                m_State = eOrbitState::ROTATE;
                break;
            case mouse::BUTTON_RIGHT:
                break;
            case mouse::BUTTON_MIDDLE:
                break;
        }
    }
}

auto OrbitCameraController::OnMouseMoveCallback(double x, double y) -> void {
    switch (m_State) {
        case eOrbitState::ROTATE: {
            constexpr auto TWO_PI = static_cast<float>(math::PI);
            m_RotateCurrent.x() = static_cast<float>(x);
            m_RotateCurrent.y() = static_cast<float>(y);
            m_RotateDelta = (m_RotateCurrent - m_RotateStart) *
                            static_cast<double>(m_RotateSpeed);

            m_SphericalDelta.theta -=
                (TWO_PI * m_RotateDelta.x() / m_ViewportWidth);
            m_SphericalDelta.phi -=
                (TWO_PI * m_RotateDelta.y() / m_ViewportHeight);

            m_RotateStart = m_RotateCurrent;
            Update();
            break;
        }
        case eOrbitState::DOLLY:
            break;
        case eOrbitState::PAN:
            break;
        default:
            break;
    }
}

}  // namespace renderer

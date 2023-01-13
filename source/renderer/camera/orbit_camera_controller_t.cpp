#include <cmath>
#include <algorithm>

#include <renderer/camera/orbit_camera_controller_t.hpp>
#include <renderer/input/buttons.hpp>

namespace renderer {

auto ToString(eOrbitState state) -> std::string {
    switch (state) {
        case eOrbitState::IDLE:
            return "idle";
        case eOrbitState::ROTATE:
            return "rotate";
        case eOrbitState::PAN:
            return "pan";
        case eOrbitState::DOLLY:
            return "dolly";
    }
}

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
    auto offset = m_Camera->position() - target;

    m_Spherical.SetFromCartesian(offset);
    if (enableDamping) {
        m_Spherical.theta += m_SphericalDelta.theta * dampingFactor;
        m_Spherical.phi += m_SphericalDelta.phi * dampingFactor;
    } else {
        m_Spherical.theta += m_SphericalDelta.theta;
        m_Spherical.phi += m_SphericalDelta.phi;
    }

    constexpr auto TWO_PI = static_cast<float>(math::PI);

    auto min = minAzimuth;
    auto max = maxAzimuth;
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
    m_Spherical.phi = std::max(minPolar, std::min(maxPolar, m_Spherical.phi));
    m_Spherical.MakeSafe();

    m_Spherical.rho =
        std::max(minDistance, std::min(maxDistance, m_Spherical.rho));

    offset = m_Spherical.GetCartesian();

    m_Camera->SetPosition(target + offset);
    // m_Camera->SetTarget(new_target)

    if (enableDamping) {
        m_SphericalDelta.theta *= (1.0F - dampingFactor);
        m_SphericalDelta.phi *= (1.0F - dampingFactor);
    } else {
        m_SphericalDelta.rho = 0.0F;
        m_SphericalDelta.phi = 0.0F;
        m_SphericalDelta.theta = 0.0F;
    }
}

auto OrbitCameraController::OnMouseButtonCallback(int button, int action,
                                                  double x, double y) -> void {
    if (!enabled) {
        return;
    }

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
    if (!enabled) {
        return;
    }

    switch (m_State) {
        case eOrbitState::ROTATE: {
            constexpr auto TWO_PI = static_cast<float>(math::PI);
            m_RotateCurrent.x() = static_cast<float>(x);
            m_RotateCurrent.y() = static_cast<float>(y);
            m_RotateDelta = (m_RotateCurrent - m_RotateStart) *
                            static_cast<double>(rotateSpeed);

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

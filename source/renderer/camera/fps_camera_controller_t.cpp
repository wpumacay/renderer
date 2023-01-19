#include <cmath>
#include <algorithm>

#include <renderer/camera/fps_camera_controller_t.hpp>
#include <renderer/input/buttons.hpp>
#include <renderer/input/keycodes.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

FpsCameraController::FpsCameraController(Camera::ptr camera)
    : ICameraController(std::move(camera)) {
    m_Type = eCameraController::FPS;
}

auto FpsCameraController::Update() -> void {
    constexpr auto DT = 1.66e-2F;  // TODO(wilbert): use dt given as func. arg.
    // Move forward (along camera's Z axis)
    auto dfront =
        m_Camera->front() * (static_cast<double>(DT * m_ForwardSpeed));
    // Move sideways (along camera's X axis)
    auto dright =
        m_Camera->right() * (static_cast<double>(DT * m_SidewaysSpeed));
    auto dtotal = dfront + dright;

    m_Camera->SetTargetNoUpdate(m_Camera->target() + dtotal);
    m_Camera->SetPositionNoUpdate(m_Camera->position() + dtotal);
    // Update the orientation (changes in pitch and yaw)
    // TODO(wilbert): implement remaining updates

    m_Camera->ComputeBasisVectors();
    m_Camera->UpdateViewMatrix();
}

auto FpsCameraController::OnKeyCallback(int key, int action, int modifier)
    -> void {
    if (!enabled) {
        return;
    }

    if (action == key_action::PRESSED || action == key_action::REPEAT) {
        switch (key) {
            case keys::KEY_UP:
            case keys::KEY_W: {
                m_ForwardSpeed = this->movSpeed;
                break;
            }

            case keys::KEY_DOWN:
            case keys::KEY_S: {
                m_ForwardSpeed = -this->movSpeed;
                break;
            }

            case keys::KEY_LEFT:
            case keys::KEY_A: {
                m_SidewaysSpeed = -this->movSpeed;
                break;
            }

            case keys::KEY_RIGHT:
            case keys::KEY_D: {
                m_SidewaysSpeed = this->movSpeed;
                break;
            }
        }
    } else if (action == key_action::RELEASED) {
        switch (key) {
            case keys::KEY_UP:
            case keys::KEY_W:
            case keys::KEY_DOWN:
            case keys::KEY_S: {
                m_ForwardSpeed = 0.0F;
                break;
            }

            case keys::KEY_LEFT:
            case keys::KEY_A:
            case keys::KEY_RIGHT:
            case keys::KEY_D: {
                m_SidewaysSpeed = 0.0F;
                break;
            }
        }
    }
}

auto FpsCameraController::OnMouseMoveCallback(double x, double y) -> void {
    if (!enabled) {
        return;
    }
}

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

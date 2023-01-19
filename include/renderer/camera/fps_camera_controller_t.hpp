#pragma once

// Implementation based on ThreeJS PointerLockControls [4]

#include <string>

#include <renderer/camera/camera_controller_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

class FpsCameraController : public ICameraController {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(FpsCameraController)

    DEFINE_SMART_POINTERS(FpsCameraController)

 public:
    explicit FpsCameraController(Camera::ptr camera);

    ~FpsCameraController() override = default;

    /// Called when the cursor is locked; the controller can be used and updated
    auto OnCursorLocked() -> void { this->enabled = true; }

    /// Called when the cursor is released; the controller is disabled
    auto OnCursorReleased() -> void { this->enabled = false; }

    /// Update the internal state of the
    auto Update() -> void override;

    /// Handle move logic on this callback (moving horizontally or vertically)
    auto OnKeyCallback(int key, int action, int modifier) -> void override;

    /// This controller doesn't setup nor use any resources during this callback
    auto OnMouseButtonCallback(int button, int action, double x, double y)
        -> void override {}

    /// Handle the orientation of the camera by focusing on a target point
    auto OnMouseMoveCallback(double x, double y) -> void override;

    /// This controller doesn't setup nor use any resources during this callback
    auto OnScrollCallback(double xOff, double yOff) -> void override {}

    /// This controller doesn't setup nor use any resources during this callback
    auto OnResizeCallback(int width, int height) -> void override {}

    auto forward_speed() const -> float { return m_ForwardSpeed; }

    auto sideways_speed() const -> float { return m_SidewaysSpeed; }

 public:
    /// The lower limit of the pitch of the camera
    float minPolar = 0.0F;
    /// The upper limit of the pitch of the camera
    float maxPolar = PI;
    /// The speed at which we can pitch
    float pointerSpeed = 1.0F;
    /// The speed at which we can translate forward and sideways
    float movSpeed = 10.0F;

 private:
    /// Speed at which we are moving forward
    float m_ForwardSpeed = 0.0F;
    /// Speed at which we are moving sideways
    float m_SidewaysSpeed = 0.0F;
};

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif

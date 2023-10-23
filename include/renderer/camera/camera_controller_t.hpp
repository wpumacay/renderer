#pragma once

#include <string>
#include <utility>

#include <renderer/common.hpp>
#include <utils/common.hpp>

#include <renderer/camera/camera_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

/// Available controller types
enum class eCameraController {
    NONE,   /// Dummy controller (used for testing purposes)
    ORBIT,  /// Handles orbiting, dollying, and panning
    FPS,    /// Handles free movement like an fps game
};

/// Returns the string representation of the given controller type
auto ToString(const eCameraController& controller_type) -> std::string;

/// Common interface for all supported camera controllers
class ICameraController {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(ICameraController)

    DEFINE_SMART_POINTERS(ICameraController)

 public:
    /// Creates an empty/dummy controller for testing purposes
    explicit ICameraController(Camera::ptr camera)
        : m_Camera(std::move(camera)) {}

    /// Frees the resources handled by this controller
    virtual ~ICameraController() = default;

    /// Updates the current state of the controller
    virtual auto Update(float dt) -> void = 0;

    /// Called when a keyboard event is received
    virtual auto OnKeyCallback(int key, int action, int modifier) -> void = 0;

    /// Called when a mouse-button event is received
    virtual auto OnMouseButtonCallback(int button, int action, double x,
                                       double y) -> void = 0;

    /// Called when a mouse-move event is received
    virtual auto OnMouseMoveCallback(double x, double y) -> void = 0;

    /// Called when a scroll event is received
    virtual auto OnScrollCallback(double xOff, double yOff) -> void = 0;

    /// Called when a resize event is received
    virtual auto OnResizeCallback(int width, int height) -> void = 0;

    /// Returns the type of this controller
    auto type() const -> eCameraController { return m_Type; }

    /// Returns the camera being handled by this controller
    auto camera() const -> Camera::ptr { return m_Camera; }

 public:
    /// Whether or not this controller is enabled
    bool enabled = true;

 protected:
    /// Type of camera controller
    eCameraController m_Type = eCameraController::NONE;
    /// Camera being handled by this controller
    Camera::ptr m_Camera = nullptr;
};

/// Dummy camera controller (events are just NOPs)
class DummyCameraController : public ICameraController {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(DummyCameraController)

    DEFINE_SMART_POINTERS(DummyCameraController)

 public:
    explicit DummyCameraController(Camera::ptr camera)
        : ICameraController(std::move(camera)) {}

    ~DummyCameraController() override = default;

    auto Update(float dt) -> void override {}

    auto OnKeyCallback(int key, int action, int modifier) -> void override {}

    auto OnMouseButtonCallback(int button, int action, double x, double y)
        -> void override {}

    auto OnMouseMoveCallback(double x, double y) -> void override {}

    auto OnScrollCallback(double xOff, double yOff) -> void override {}

    auto OnResizeCallback(int width, int height) -> void override {}
};

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

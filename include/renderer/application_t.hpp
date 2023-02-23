#pragma once

#include <renderer/common.hpp>
#include <renderer/window/window_t.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/camera/camera_controller_t.hpp>
#include <renderer/input/input_manager_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/assets/texture_manager_t.hpp>
#include <renderer/debug/debug_drawer_t.hpp>

namespace renderer {

class Application {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Application)

    DEFINE_SMART_POINTERS(Application)

    static constexpr int DEFAULT_WINDOW_WIDTH = 800;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 600;

    static constexpr float DEFAULT_CAM_FOV = 45.0F;
    static constexpr float DEFAULT_CAM_NEAR = 0.1F;
    static constexpr float DEFAULT_CAM_FAR = 1000.0F;
    static constexpr float DEFAULT_CAM_FRUSTUM_SIZE = 20.0F;

 public:
    /// Creates an application with a window of the given properties
    explicit Application(
        int window_width = DEFAULT_WINDOW_WIDTH,
        int window_height = DEFAULT_WINDOW_HEIGHT,
        eWindowBackend window_backend = eWindowBackend::TYPE_GLFW);

    /// Deallocates any owned resources by this application
    ~Application() = default;

    /// \brief Prepares the application for a render step
    auto Begin() -> void;

    /// \brief Renders the current scene using the view of the current camera
    auto Render() -> void;

    /// \brief Cleans up any resources of this application after the render step
    auto End() -> void;

    // Some getters. We try to avoid returning raw pointers by returning
    // both mutable and unmutable references, but dereferencing a nullptr leads
    // to a segfault. These objects must be kept alive (point to valid objects)
    // for dereferencing to work correctly, and we try to keep this contract in
    // the internal implementation of this helper class

    auto window() -> Window&;

    auto window() const -> const Window&;

    auto camera() -> Camera&;

    auto camera() const -> const Camera&;

    auto camera_controller() -> ICameraController&;

    auto camera_controller() const -> const ICameraController&;

    auto input_manager() -> InputManager&;

    auto input_manager() const -> const InputManager&;

    auto texture_manager() -> TextureManager&;

    auto texture_manager() const -> const TextureManager&;

    auto shader_manager() -> ShaderManager&;

    auto shader_manager() const -> const ShaderManager&;

    auto debug_drawer() -> DebugDrawer&;

    auto debug_drawer() const -> const DebugDrawer&;

 protected:
    /// Internal window used for drawing stuff into
    ::renderer::Window::uptr m_Window = nullptr;
    /// The current camera used for rendering the current scene)
    ::renderer::Camera::ptr m_CurrentCamera = nullptr;
    /// The camera controller used for the current camera
    ::renderer::ICameraController::ptr m_CameraController = nullptr;
    /// The input manager used keep track of the state of input events
    ::renderer::InputManager::ptr m_InputManager = nullptr;
    /// The texture manager used to handle loading and sharing of textures
    ::renderer::TextureManager::ptr m_TextureManager = nullptr;
    /// The shader manager used to handle loading and sharing of shader programs
    ::renderer::ShaderManager::ptr m_ShaderManager = nullptr;
    /// The debug drawer used to handle drawing lines and other debug primitives
    ::renderer::DebugDrawer::ptr m_DebugDrawer = nullptr;
};

}  // namespace renderer

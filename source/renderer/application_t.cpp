#include <renderer/application_t.hpp>
#include <renderer/camera/orbit_camera_controller_t.hpp>

#include <glad/gl.h>

#include <utils/logging.hpp>
#include <utils/timing.hpp>

namespace renderer {

Application::Application(int window_width, int window_height,
                         eWindowBackend window_backend) {
    LOG_CORE_INFO("Application >>> Initializing ...");

    utils::Clock::Init();

    m_Window =
        std::make_unique<Window>(window_width, window_height, window_backend);

    m_InputManager = std::make_shared<InputManager>();
    m_Window->RegisterKeyboardCallback([&](int key, int action, int) {
        if (m_InputManager) {
            m_InputManager->CallbackKey(key, action);
        }
    });
    m_Window->RegisterMouseButtonCallback([&](int button, int action, double x,
                                              double y) {
        if (m_InputManager) {
            m_InputManager->CallbackMouseButton(
                button, action, static_cast<float>(x), static_cast<float>(y));
        }
    });
    m_Window->RegisterMouseMoveCallback([&](double x, double y) {
        if (m_InputManager) {
            m_InputManager->CallbackMouseMove(static_cast<float>(x),
                                              static_cast<float>(y));
        }
    });
    m_Window->RegisterScrollCallback([&](double xOff, double yOff) {
        if (m_InputManager) {
            m_InputManager->CallbackScroll(static_cast<float>(xOff),
                                           static_cast<float>(yOff));
        }
    });

    const Vec3 CAM_POSITION = {5.0F, 5.0F, 5.0F};
    const Vec3 CAM_TARGET = {0.0F, 0.0F, 0.0F};
    const Vec3 CAM_WORLD_UP = {0.0F, 0.0F, 1.0F};

    ::renderer::ProjectionData proj_data;
    proj_data.fov = DEFAULT_CAM_FOV;
    proj_data.aspect =
        static_cast<float>(window_width) / static_cast<float>(window_height);
    proj_data.near = DEFAULT_CAM_NEAR;
    proj_data.far = DEFAULT_CAM_FAR;
    proj_data.width = DEFAULT_CAM_FRUSTUM_SIZE * proj_data.aspect;
    proj_data.height = DEFAULT_CAM_FRUSTUM_SIZE;
    m_CurrentCamera = std::make_shared<Camera>(CAM_POSITION, CAM_TARGET,
                                               CAM_WORLD_UP, proj_data);

    m_CameraController = std::make_shared<OrbitCameraController>(
        m_CurrentCamera, window_width, window_height);
    m_Window->RegisterResizeCallback([&](int width, int height) {
        glViewport(0, 0, width, height);
        // Update camera projection accordingly
        auto data = m_CurrentCamera->proj_data();
        data.aspect = static_cast<float>(width) / static_cast<float>(height);
        data.width = DEFAULT_CAM_FRUSTUM_SIZE * data.aspect;
        data.height = DEFAULT_CAM_FRUSTUM_SIZE;
        // @todo(wilbert): check if lambda notices a change of camera
        if (m_CurrentCamera) {
            m_CurrentCamera->SetProjectionData(data);
        }
        // @todo(wilbert): check if lambda notices a change of camera controller
        if (m_CameraController) {
            m_CameraController->OnResizeCallback(width, height);
        }
    });

    m_Window->RegisterKeyboardCallback([&](int key, int action, int modifier) {
        if (m_CameraController) {
            m_CameraController->OnKeyCallback(key, action, modifier);
        }
    });
    m_Window->RegisterMouseButtonCallback(
        [&](int button, int action, double x, double y) {
            if (m_CameraController) {
                m_CameraController->OnMouseButtonCallback(button, action, x, y);
            }
        });
    m_Window->RegisterMouseMoveCallback([&](double x, double y) {
        if (m_CameraController) {
            m_CameraController->OnMouseMoveCallback(x, y);
        }
    });
    m_Window->RegisterScrollCallback([&](double xOff, double yOff) {
        if (m_CameraController) {
            m_CameraController->OnScrollCallback(xOff, yOff);
        }
    });

    LOG_CORE_INFO("Application >>> Successfully initialized :D");
}

auto Application::Begin() -> void {
    utils::Clock::Tick();

    if (!m_Window) {
        return;
    }

    m_Window->Begin();
}

auto Application::Render() -> void {
    if (m_CameraController) {
        m_CameraController->Update(utils::Clock::GetAvgTimeStep());
    }
}

auto Application::End() -> void {
    utils::Clock::Tock();

    if (!m_Window) {
        return;
    }

    m_Window->End();
}

}  // namespace renderer

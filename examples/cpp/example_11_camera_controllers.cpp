#include <array>
#include <glad/gl.h>

#include <memory>
#include <renderer/window/window_t.hpp>
#include <renderer/input/input_manager_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/camera/camera_controller_t.hpp>
#include <renderer/camera/orbit_camera_controller_t.hpp>
#include <renderer/camera/fps_camera_controller_t.hpp>
#include <renderer/geometry/geometry_t.hpp>
#include <renderer/geometry/geometry_factory.hpp>
#include <renderer/light/light_t.hpp>

#include <utils/logging.hpp>
#include <utils/timing.hpp>
#include <example_common_utils.hpp>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

// NOLINTNEXTLINE (avoid warning on cognitive complexity)
auto main() -> int {
    int g_window_width = 800;
    int g_window_height = 600;
    float g_window_aspect = static_cast<float>(g_window_width) /
                            static_cast<float>(g_window_height);

    utils::Clock::Init();

    renderer::Window window(g_window_width, g_window_height);
    renderer::ShaderManager shader_manager;
    renderer::InputManager input_manager;
    window.RegisterKeyboardCallback([&](int key, int action, int) {
        input_manager.CallbackKey(key, action);
    });

    const std::string VERT_SHADER =
        renderer::EXAMPLES_PATH + "/resources/shaders/basicPhong_vert.glsl";
    const std::string FRAG_SHADER =
        renderer::EXAMPLES_PATH + "/resources/shaders/basicPhong_frag.glsl";
    auto program =
        shader_manager.LoadProgram("basicPhong", VERT_SHADER, FRAG_SHADER);

    constexpr auto BOX_WIDTH = 2.0F;
    constexpr auto BOX_DEPTH = 2.0F;
    constexpr auto BOX_HEIGHT = 2.0F;
    auto geometry = renderer::CreateBox(BOX_WIDTH, BOX_DEPTH, BOX_HEIGHT);

    const Vec3 CAM_POSITION = {5.0F, 5.0F, 5.0F};
    const Vec3 CAM_TARGET = {0.0F, 0.0F, 0.0F};
    renderer::ProjectionData proj_data;
    // Parameters related to perspective projection
    proj_data.fov = 45.0F;
    proj_data.aspect = g_window_aspect;
    proj_data.near = 0.1F;
    proj_data.far = 1000.0F;
    // Parameters related to orthographic projection
    constexpr float FRUSTUM_SIZE = 20.0F;
    proj_data.width = FRUSTUM_SIZE * g_window_aspect;
    proj_data.height = FRUSTUM_SIZE;
    auto camera = std::make_shared<renderer::Camera>(
        CAM_POSITION, CAM_TARGET, Vec3(0.0F, 0.0F, 1.0F), proj_data);
    renderer::ICameraController::ptr camera_controller =
        std::make_shared<renderer::OrbitCameraController>(
            camera, g_window_width, g_window_height);

    window.RegisterResizeCallback([&](int width, int height) {
        // Bookkeeping the new viewport
        g_window_width = width;
        g_window_height = height;
        auto window_aspect =
            static_cast<float>(width) / static_cast<float>(height);
        glViewport(0, 0, width, height);
        // Update the camera projection accordingly
        auto data = camera->proj_data();
        data.aspect = window_aspect;
        data.width = FRUSTUM_SIZE * window_aspect;
        data.height = FRUSTUM_SIZE;
        camera->SetProjectionData(data);

        // Notify any controller of the change of viewport
        if (camera_controller != nullptr) {
            camera_controller->OnResizeCallback(width, height);
        }
    });
    window.RegisterKeyboardCallback([&](int key, int action, int modifier) {
        camera_controller->OnKeyCallback(key, action, modifier);
    });
    window.RegisterMouseButtonCallback(
        [&](int button, int action, double x, double y) {
            camera_controller->OnMouseButtonCallback(button, action, x, y);
        });
    window.RegisterMouseMoveCallback([&](double x, double y) {
        camera_controller->OnMouseMoveCallback(x, y);
    });
    window.RegisterScrollCallback([&](double xOff, double yOff) {
        camera_controller->OnScrollCallback(xOff, yOff);
    });

    const Vec3 LIGHT_DIRECTION = {-1.0F, -2.0F, -3.0F};
    auto light = std::make_shared<renderer::DirectionalLight>(LIGHT_DIRECTION);

    const Vec3 AMBIENT_LIGHT = {0.1F, 0.1F, 0.1F};
    const Vec3 OBJECT_COLOR = {1.0F, 0.5F, 0.31F};

    while (window.active()) {
        utils::Clock::Tick();
        window.Begin();

        if (input_manager.IsKeyDown(renderer::keys::KEY_ESCAPE)) {
            window.RequestClose();
            continue;
        }

#if defined(RENDERER_IMGUI)
        ImGui::Begin("Camera Controller Options");
        ImGui::Checkbox("Enabled", &camera_controller->enabled);
        // NOLINTNEXTLINE
        ImGui::Text("CameraPosition: (%.2f, %.2f, %.2f)",
                    static_cast<double>(camera->position().x()),
                    static_cast<double>(camera->position().y()),
                    static_cast<double>(camera->position().z()));
        // NOLINTNEXTLINE
        ImGui::Text("CameraTarget: (%.2f, %.2f, %.2f)",
                    static_cast<double>(camera->target().x()),
                    static_cast<double>(camera->target().y()),
                    static_cast<double>(camera->target().z()));

        Euler orientation_euler(camera->orientation());
        // NOLINTNEXTLINE
        ImGui::Text("CameraEuler: (%.2f, %.2f, %.2f)",
                    static_cast<double>(orientation_euler.x),
                    static_cast<double>(orientation_euler.y),
                    static_cast<double>(orientation_euler.z));

        // NOLINTNEXTLINE
        ImGui::Text("CameraRight: (%.2f, %.2f, %.2f)",
                    static_cast<double>(camera->right().x()),
                    static_cast<double>(camera->right().y()),
                    static_cast<double>(camera->right().z()));

        // NOLINTNEXTLINE
        ImGui::Text("CameraUp: (%.2f, %.2f, %.2f)",
                    static_cast<double>(camera->up().x()),
                    static_cast<double>(camera->up().y()),
                    static_cast<double>(camera->up().z()));

        // NOLINTNEXTLINE
        ImGui::Text("CameraFront: (%.2f, %.2f, %.2f)",
                    static_cast<double>(camera->front().x()),
                    static_cast<double>(camera->front().y()),
                    static_cast<double>(camera->front().z()));

        std::array<const char*, 2> items_projections = {"perspective",
                                                        "orthographic"};
        IMGUI_COMBO(items_projections, "CameraProjection",
                    [&](size_t combo_index) -> void {
                        switch (combo_index) {
                            case 0:  // perspective
                                camera->SetProjectionType(
                                    renderer::eProjectionType::PERSPECTIVE);
                                break;
                            case 1:  // orthographic
                                camera->SetProjectionType(
                                    renderer::eProjectionType::ORTHOGRAPHIC);
                                break;
                            default:
                                break;
                        }
                        LOG_INFO(
                            "Using projection type: {0}",
                            renderer::ToString(camera->proj_data().projection));
                    });

        proj_data = camera->proj_data();
        switch (proj_data.projection) {
            case renderer::eProjectionType::PERSPECTIVE: {
                float fov = proj_data.fov;
                float near = proj_data.near;
                float far = proj_data.far;
                ImGui::SliderFloat("CameraFOV", &fov, 10.0F, 150.0F);
                ImGui::SliderFloat("CameraNear", &near, 0.1F, 10.0F);
                ImGui::SliderFloat("CameraFar", &far, near + 1e-3F, 1000.0F);
                // NOLINTNEXTLINE
                ImGui::Text("CameraAspectRatio= %.2f",
                            static_cast<double>(proj_data.aspect));
                proj_data.fov = fov;
                proj_data.near = near;
                proj_data.far = far;
                break;
            }
            case renderer::eProjectionType::ORTHOGRAPHIC: {
                float width = proj_data.width;
                float height = proj_data.height;
                ImGui::SliderFloat("CameraWidth", &width, 1.0F, 100.0F);
                ImGui::SliderFloat("CameraHeight", &height, 1.0F, 100.0F);
                proj_data.width = width;
                proj_data.height = height;
                break;
            }
        }
        camera->SetProjectionData(proj_data);
        auto zoom = camera->zoom();
        ImGui::SliderFloat("CameraZoom", &zoom, 0.1F, 10.0F);
        camera->SetZoom(zoom);

        std::array<const char*, 3> items_controllers = {"none", "orbit", "fps"};
        IMGUI_COMBO(
            items_controllers, "CameraController",
            [&](size_t combo_index) -> void {
                switch (combo_index) {
                    case 0: {  // Dummy controller
                        camera_controller =
                            std::make_shared<renderer::DummyCameraController>(
                                camera);
                        break;
                    }
                    case 1: {  // Orbit controller
                        camera_controller =
                            std::make_shared<renderer::OrbitCameraController>(
                                camera, g_window_width, g_window_height);
                        break;
                    }
                    case 2: {  // Fps controller
                        camera_controller =
                            std::make_shared<renderer::FpsCameraController>(
                                camera);
                        break;
                    }
                }
                // Attach callbacks for this new controller
                window.RegisterKeyboardCallback(
                    [&](int key, int action, int modifier) {
                        camera_controller->OnKeyCallback(key, action, modifier);
                    });
                window.RegisterMouseButtonCallback(
                    [&](int button, int action, double x, double y) {
                        camera_controller->OnMouseButtonCallback(button, action,
                                                                 x, y);
                    });
                window.RegisterMouseMoveCallback([&](double x, double y) {
                    camera_controller->OnMouseMoveCallback(x, y);
                });
                window.RegisterScrollCallback([&](double xOff, double yOff) {
                    camera_controller->OnScrollCallback(xOff, yOff);
                });
                LOG_INFO("Using camera-controller: {0}",
                         renderer::ToString(camera_controller->type()));
            });

        if (auto orbit_controller =
                std::dynamic_pointer_cast<renderer::OrbitCameraController>(
                    camera_controller)) {
            if (ImGui::CollapsingHeader("Orbit Controller Options")) {
                // NOLINTNEXTLINE
                ImGui::Text(
                    "State: %s",
                    renderer::ToString(orbit_controller->state()).c_str());
                ImGui::Checkbox("EnableDamping",
                                &orbit_controller->enableDamping);
                ImGui::SliderFloat("DampingFactor",
                                   &orbit_controller->dampingFactor, 0.0F,
                                   1.0F);
                ImGui::Spacing();

                ImGui::Checkbox("EnableRotate",
                                &orbit_controller->enableRotate);
                if (orbit_controller->enableRotate) {
                    ImGui::SliderFloat("RotationSpeed",
                                       &orbit_controller->rotateSpeed, 0.0F,
                                       2.0F);
                    ImGui::Checkbox("AutoRotate",
                                    &orbit_controller->enableAutoRotate);
                    ImGui::SliderFloat("AutoRotateSpeed",
                                       &orbit_controller->autoRotateSpeed, 0.0F,
                                       4.0F);
                    ImGui::SliderFloat("MinPolar", &orbit_controller->minPolar,
                                       0.0F, PI);
                    ImGui::SliderFloat("MaxPolar", &orbit_controller->maxPolar,
                                       orbit_controller->minPolar, PI);
                    ImGui::SliderFloat("MinAzimuth",
                                       &orbit_controller->minAzimuth,
                                       -2.0F * PI, 2.0F * PI);
                    ImGui::SliderFloat("MaxAzimuth",
                                       &orbit_controller->maxAzimuth,
                                       orbit_controller->minAzimuth, 2.0F * PI);
                }
                ImGui::Spacing();

                ImGui::Checkbox("EnablePan", &orbit_controller->enablePan);
                if (orbit_controller->enablePan) {
                    ImGui::SliderFloat("PanSpeed", &orbit_controller->panSpeed,
                                       0.0F, 2.0F);
                    ImGui::Checkbox("ScreenSpacePanning",
                                    &orbit_controller->screenSpacePanning);
                }
                ImGui::Spacing();

                ImGui::Checkbox("EnableZoom", &orbit_controller->enableZoom);
                if (orbit_controller->enableZoom) {
                    ImGui::SliderFloat(
                        "ZoomSpeed", &orbit_controller->zoomSpeed, 1.0F, 4.0F);
                }
            }
        } else if (auto fps_controller =
                       std::dynamic_pointer_cast<renderer::FpsCameraController>(
                           camera_controller)) {
            if (ImGui::CollapsingHeader("Fps Controller Options")) {
                ImGui::SliderFloat("FpsPointerSpeed",
                                   &fps_controller->pointerSpeed, 0.0F, 10.0F);
            }
        }
        ImGui::End();
#endif

        if (camera_controller != nullptr) {
            camera_controller->Update(utils::Clock::GetTimeStep());
        }

        auto model_matrix = Mat4::Identity();
        auto normal_matrix = math::inverse(math::transpose(model_matrix));

        program->Bind();
        program->SetMat4("u_model_matrix", model_matrix);
        program->SetMat4("u_normal_matrix", normal_matrix);
        program->SetMat4("u_view_proj_matrix",
                         camera->proj_matrix() * camera->view_matrix());
        program->SetVec3("u_viewer_position", camera->position());
        program->SetVec3("u_object_color", OBJECT_COLOR);
        program->SetVec3("u_ambient_light", AMBIENT_LIGHT);
        // Light-related uniforms ---------------
        program->SetVec3("u_dir_light.direction", light->direction);
        program->SetVec3("u_dir_light.color", light->color);
        program->SetFloat("u_dir_light.intensity", light->intensity);
        program->SetInt("u_dir_light.enabled", GL_TRUE);
        program->SetInt("u_point_light.enabled", GL_FALSE);
        program->SetInt("u_spot_light.enabled", GL_FALSE);
        // --------------------------------------

        geometry->VAO().Bind();

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLint>(geometry->VAO().index_buffer().count()),
            GL_UNSIGNED_INT, nullptr);

        geometry->VAO().Unbind();
        program->Unbind();
        window.End();
        utils::Clock::Tock();
    }
}

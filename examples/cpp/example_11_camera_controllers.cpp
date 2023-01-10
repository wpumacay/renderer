#include <array>
#include <glad/gl.h>

#include <renderer/window/window_t.hpp>
#include <renderer/input/input_manager_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/camera/camera_controller_t.hpp>
#include <renderer/geometry/geometry_t.hpp>
#include <renderer/geometry/geometry_factory.hpp>
#include <renderer/light/light_t.hpp>

#include <utils/logging.hpp>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

auto main() -> int {
    renderer::Window window(WINDOW_WIDTH, WINDOW_HEIGHT);
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
    auto camera = std::make_shared<renderer::Camera>(CAM_POSITION, CAM_TARGET);
    auto camera_controller =
        std::make_shared<renderer::DummyCameraController>(camera);

    window.RegisterResizeCallback([&](int width, int height) {
        camera->SetAspectRatio(static_cast<float>(width) /
                               static_cast<float>(height));
        glViewport(0, 0, width, height);
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
        window.Begin();

        if (input_manager.IsKeyDown(renderer::keys::KEY_ESCAPE)) {
            window.RequestClose();
            continue;
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
    }
}

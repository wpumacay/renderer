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
#include <renderer/debug/debug_drawer_t.hpp>
#include <renderer/application_t.hpp>

#include <utils/logging.hpp>
#include <utils/timing.hpp>
#include <example_common_utils.hpp>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

// NOLINTNEXTLINE (avoid warning on cognitive complexity)
auto main() -> int {
    auto app = std::make_unique<::renderer::Application>();

    renderer::ShaderManager shader_manager;
    renderer::DebugDrawer debug_drawer;

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

    const Vec3 LIGHT_DIRECTION = {-1.0F, -2.0F, -3.0F};
    auto light = std::make_shared<renderer::DirectionalLight>(LIGHT_DIRECTION);

    const Vec3 AMBIENT_LIGHT = {0.1F, 0.1F, 0.1F};
    const Vec3 OBJECT_COLOR = {1.0F, 0.5F, 0.31F};

    while (app->window().active()) {
        app->window().Begin();

        if (app->input_manager().IsKeyDown(renderer::keys::KEY_ESCAPE)) {
            app->window().RequestClose();
            continue;
        }

        auto model_matrix = Mat4::Identity();
        auto normal_matrix = math::inverse(math::transpose(model_matrix));

        program->Bind();
        program->SetMat4("u_model_matrix", model_matrix);
        program->SetMat4("u_normal_matrix", normal_matrix);
        program->SetMat4("u_view_proj_matrix", app->camera().proj_matrix() *
                                                   app->camera().view_matrix());
        program->SetVec3("u_viewer_position", app->camera().position());
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

        debug_drawer.DrawLine({0.0F, 0.0F, 0.0F}, {5.0F, 0.0F, 0.0F},
                              {1.0F, 0.0F, 0.0F});
        debug_drawer.DrawLine({0.0F, 0.0F, 0.0F}, {0.0F, 5.0F, 0.0F},
                              {0.0F, 1.0F, 0.0F});
        debug_drawer.DrawLine({0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 5.0F},
                              {0.0F, 0.0F, 1.0F});
        debug_drawer.Render(app->camera());

        app->window().End();
    }
}

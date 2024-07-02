#include <memory>
#include <glad/gl.h>

#include <renderer/geometry/geometry_factory.hpp>
#include <renderer/light/light_t.hpp>
#include <renderer/engine/application_t.hpp>

auto main() -> int {
    ::renderer::Application app;

    auto program = app.shader_manager().LoadProgram(
        "basicPhong",
        renderer::EXAMPLES_PATH + "/resources/shaders/basicPhong_vert.glsl",
        renderer::EXAMPLES_PATH + "/resources/shaders/basicPhong_frag.glsl");

    constexpr auto BOX_WIDTH = 1.0F;
    constexpr auto BOX_DEPTH = 1.0F;
    constexpr auto BOX_HEIGHT = 1.0F;
    auto geometry = renderer::CreateBox(BOX_WIDTH, BOX_DEPTH, BOX_HEIGHT);

    const Vec3 LIGHT_DIRECTION = {-1.0F, -2.0F, -3.0F};
    auto light = std::make_shared<renderer::DirectionalLight>(LIGHT_DIRECTION);

    const Vec3 AMBIENT_LIGHT = {0.1F, 0.1F, 0.1F};
    const Vec3 OBJECT_COLOR = {1.0F, 0.5F, 0.31F};

    while (app.window().active()) {
        app.Begin();

        if (app.input_manager().IsKeyDown(renderer::keys::KEY_ESCAPE)) {
            app.window().RequestClose();
            continue;
        }

        auto model_matrix = Mat4::Identity();
        auto normal_matrix = math::inverse(math::transpose(model_matrix));

        program->Bind();
        program->SetMat4("u_model_matrix", model_matrix);
        program->SetMat4("u_normal_matrix", normal_matrix);
        program->SetMat4("u_view_proj_matrix", app.camera().proj_matrix() *
                                                   app.camera().view_matrix());
        program->SetVec3("u_viewer_position", app.camera().position());
        program->SetVec3("u_object_color", OBJECT_COLOR);
        program->SetVec3("u_ambient_light", AMBIENT_LIGHT);
        program->SetVec3("u_dir_light.direction", light->direction);
        program->SetVec3("u_dir_light.color", light->color);
        program->SetFloat("u_dir_light.intensity", light->intensity);
        program->SetInt("u_dir_light.enabled", GL_TRUE);
        program->SetInt("u_point_light.enabled", GL_FALSE);
        program->SetInt("u_spot_light.enabled", GL_FALSE);

        geometry->VAO().Bind();

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLint>(geometry->VAO().index_buffer().count()),
            GL_UNSIGNED_INT, nullptr);

        geometry->VAO().Unbind();
        program->Unbind();

        app.debug_drawer().DrawLine({0.0F, 0.0F, 0.0F}, {5.0F, 0.0F, 0.0F},
                                    {1.0F, 0.0F, 0.0F});
        app.debug_drawer().DrawLine({0.0F, 0.0F, 0.0F}, {0.0F, 5.0F, 0.0F},
                                    {0.0F, 1.0F, 0.0F});
        app.debug_drawer().DrawLine({0.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 5.0F},
                                    {0.0F, 0.0F, 1.0F});

        auto tf = Mat4::Identity();
        tf(0, 3) = 1.0F;
        tf(1, 3) = 1.0F;
        tf(2, 3) = 1.0F;
        app.debug_drawer().DrawBox({1.0F, 1.0F, 1.0F}, tf, {1.0F, 1.0F, 1.0F});
        app.debug_drawer().DrawSphere(0.25F, tf, {1.0F, 0.0F, 1.0F});
        app.debug_drawer().DrawCylinder(0.25F, 0.5F, tf, {1.0F, 1.0F, 0.0F});

        app.Render();
        app.End();
    }
}

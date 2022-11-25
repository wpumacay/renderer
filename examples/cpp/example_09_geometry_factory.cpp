#include <array>
#include <glad/gl.h>

#include <renderer/window/window_t.hpp>
#include <renderer/input/input_manager_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/geometry/geometry_t.hpp>
#include <renderer/geometry/geometry_factory.hpp>

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

    auto program = shader_manager.LoadProgram(
        "basic3d",
        renderer::EXAMPLES_PATH + "/resources/shaders/basic3d_vert.glsl",
        renderer::EXAMPLES_PATH + "/resources/shaders/basic3d_frag.glsl");

    auto geometry = renderer::CreateSphere(1.0F, 15, 15);

    renderer::Camera camera({3.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 0.0F});
    Vec3 color = {0.1F, 0.2F, 0.7F};

    while (window.active()) {
        window.Begin();

        if (input_manager.IsKeyDown(renderer::keys::KEY_ESCAPE)) {
            window.RequestClose();
        }

#if defined(RENDERER_IMGUI)
        ImGui::Begin("Options");
        // Allow the user to control the position of the camera
        static Vec3 s_camera_position = camera.position();
        ImGui::SliderFloat3("camera.position", s_camera_position.data(), -10.0F,
                            10.0F);
        camera.SetPosition(s_camera_position);
        // Allow the user to set wireframe mode
        static bool s_wireframe = false;
        ImGui::Checkbox("wireframe", &s_wireframe);
        if (s_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        // Allow the user to select which geometry to visualize
        std::array<const char*, 4> items_geometries = {"plane", "box", "sphere",
                                                       "ellipsoid"};
        static size_t s_item_current_idx = 0;
        static bool s_change_item = false;
        const char* combo_preview_value =
            items_geometries.at(s_item_current_idx);
        if (ImGui::BeginCombo("Geometries", combo_preview_value)) {
            for (size_t i = 0; i < items_geometries.size(); ++i) {
                const auto IS_SELECTED = (s_item_current_idx == i);
                if (ImGui::Selectable(items_geometries.at(i), IS_SELECTED)) {
                    s_change_item = (s_item_current_idx != i);
                    s_item_current_idx = i;
                }
                if (IS_SELECTED) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        if (s_change_item) {
            switch (s_item_current_idx) {
                case 0:  // plane
                    geometry = renderer::CreatePlane(1.0F, 1.0F,
                                                     renderer::eAxis::AXIS_Z);
                    break;
                case 1:  // box
                    geometry = renderer::CreateBox(1.0F, 1.0F, 1.0F);
                    break;
                case 2:  // sphere
                    geometry = renderer::CreateSphere(1.0F, 20, 20);
                    break;
                case 3:  // ellipsoid
                    geometry =
                        renderer::CreateEllipsoid(0.5F, 2.5F, 4.5F, 20, 20);
                    break;
                default:
                    break;
            }
            s_change_item = false;
        }
        ImGui::End();
#endif  // RENDERER_IMGUI

        program->Bind();
        program->SetMat4("u_model_matrix", Mat4::Identity());
        program->SetMat4("u_view_matrix", camera.view_matrix());
        program->SetMat4("u_proj_matrix", camera.proj_matrix());
        program->SetVec3("u_color", color);

        geometry->VAO().Bind();

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLint>(geometry->VAO().index_buffer().count()),
            GL_UNSIGNED_INT, nullptr);

#if defined(RENDERER_IMGUI)
        if (s_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
#endif  // RENDERER_IMGUI

        geometry->VAO().Unbind();
        program->Unbind();
        window.End();
    }
}

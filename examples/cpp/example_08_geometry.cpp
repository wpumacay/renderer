#include <array>
#include <glad/gl.h>

#include <renderer/window/window_t.hpp>
#include <renderer/input/input_manager_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/geometry/geometry_t.hpp>

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

    // clang-format off
    std::array<float, 12> positions = {
         0.0F, -1.0F, -1.0F,
         0.0F,  1.0F, -1.0F,
         0.0F,  1.0F,  1.0F,
         0.0F, -1.0F,  1.0F,
    };
    std::array<float, 12> normals = {
        1.0F, 0.0F, 0.0F,
        1.0F, 0.0F, 0.0F,
        1.0F, 0.0F, 0.0F,
        1.0F, 0.0F, 0.0F,
    };
    std::array<float, 8> uvs = {
        0.0F, 0.0F,
        1.0F, 0.0F,
        1.0F, 1.0F,
        0.0F, 1.0F,
    };
    std::array<uint32_t, 6> indices = {
        0, 1, 2,
        0, 2, 3,
    };
    // clang-format on

    const size_t N_VERTICES = 4;
    const size_t N_INDICES = 6;

    auto geometry = std::make_unique<renderer::Geometry>(
        positions.data(), normals.data(), uvs.data(), N_VERTICES,
        indices.data(), N_INDICES);

    renderer::Camera camera({3.0F, 0.0F, 0.0F}, {0.0F, 0.0F, 0.0F});
    Vec3 color = {0.1F, 0.2F, 0.7F};

    while (window.active()) {
        window.Begin();

        if (input_manager.IsKeyDown(renderer::keys::KEY_ESCAPE)) {
            window.RequestClose();
        }

        program->Bind();
        geometry->VAO().Bind();

        program->SetMat4("u_model_matrix", Mat4::Identity());
        program->SetMat4("u_view_matrix", camera.view_matrix());
        program->SetMat4("u_proj_matrix", camera.proj_matrix());

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
#endif  // RENDERER_IMGUI
        program->SetVec3("u_color", color);

        glDrawElements(GL_TRIANGLES, N_INDICES, GL_UNSIGNED_INT, nullptr);

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

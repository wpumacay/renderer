#include <array>
#include <glad/gl.h>

#include <renderer/window/window_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/core/vertex_array_t.hpp>
#include <renderer/core/vertex_buffer_t.hpp>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

auto main() -> int {
    renderer::Window window(WINDOW_WIDTH, WINDOW_HEIGHT);
    renderer::ShaderManager shader_manager;

    auto program = shader_manager.LoadProgram(
        "basic3d",
        renderer::EXAMPLES_PATH + "/resources/shaders/basic3d_vert.glsl",
        renderer::EXAMPLES_PATH + "/resources/shaders/basic3d_frag.glsl");

    // clang-format off
    std::array<float, 9> buffer_data = {
        /*|    pos     |*/
        -1.0F, 0.0F, -1.0F,
         1.0F, 0.0F, -1.0F,
         0.0F, 0.0F,  1.0F
    };
    // clang-format on

    renderer::BufferLayout layout = {
        {"position", renderer::eElementType::FLOAT_3, false}};

    auto vbo = std::make_unique<renderer::VertexBuffer>(
        layout, renderer::eBufferUsage::STATIC,
        static_cast<uint32_t>(sizeof(buffer_data)), buffer_data.data());

    auto vao = std::make_unique<renderer::VertexArray>();
    vao->AddVertexBuffer(std::move(vbo));

    renderer::Camera camera({0.0F, -3.0F, 0.0F}, {0.0F, 0.0F, 0.0F});
    Vec3 color = {0.1F, 0.2F, 0.7F};

    while (window.active()) {
        window.Begin();
        program->Bind();
        vao->Bind();

        program->SetMat4("u_model_matrix", Mat4::Identity());
        program->SetMat4("u_view_matrix", camera.view_matrix());
        program->SetMat4("u_proj_matrix", camera.proj_matrix());

#if defined(RENDERER_IMGUI)
        ImGui::Begin("Options");
        {
            // Allow the user to set the color of the triangle :D
            ImGui::ColorEdit3("color", color.data());
            // Allow the user to control the position of the camera
            static Vec3 s_camera_position = camera.position();
            ImGui::SliderFloat3("camera.position", s_camera_position.data(),
                                -10.0F, 10.0F);
            camera.SetPosition(s_camera_position);
        }
#endif  // RENDERER_IMGUI
        program->SetVec3("u_color", color);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        vao->Unbind();
        program->Unbind();
        window.End();
    }
}

#include <glad/gl.h>

#include <renderer/debug/debug_drawer_t.hpp>
#include <renderer/core/vertex_buffer_layout_t.hpp>
#include <renderer/core/vertex_buffer_t.hpp>

#include <utils/logging.hpp>

constexpr const char* DD_VERT_SHADER_WIREFRAME_MODE_SRC = R"(
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;

out vec3 f_color;

void main() {
    gl_Position = u_proj_matrix * u_view_matrix * vec4(position, 1.0);
    f_color = color;
}
)";

constexpr const char* DD_FRAG_SHADER_WIREFRAME_MODE_SRC = R"(
#version 330 core

in vec3 f_color;
out vec3 color;

void main() {
    color = vec4(f_color, 1.0);
}
)";

namespace renderer {

DebugDrawer::DebugDrawer() {
    LOG_CORE_INFO("Engine::DebugDrawer >>> initializing ...");

    m_ProgramModeWireframe = std::make_unique<Program>(
        "dd_wireframe_prog", DD_VERT_SHADER_WIREFRAME_MODE_SRC,
        DD_FRAG_SHADER_WIREFRAME_MODE_SRC);

    BufferLayout buff_positions_layout = {
        {"position", ::renderer::eElementType::FLOAT_3, false}};
    BufferLayout buff_colors_layout = {
        {"colors", ::renderer::eElementType::FLOAT_3, false}};

    constexpr uint32_t POSITIONS_VBO_SIZE = sizeof(Vec3) * 2 * LINES_BATCH_SIZE;
    constexpr uint32_t COLORS_VBO_SIZE = sizeof(Vec3) * LINES_BATCH_SIZE;

    auto lines_positions_vbo = std::make_unique<VertexBuffer>(
        buff_positions_layout, ::renderer::eBufferUsage::DYNAMIC,
        POSITIONS_VBO_SIZE, nullptr);

    auto lines_colors_vbo = std::make_unique<VertexBuffer>(
        buff_colors_layout, ::renderer::eBufferUsage::DYNAMIC, COLORS_VBO_SIZE,
        nullptr);

    m_LinesVAO = std::make_unique<VertexArray>();
    m_LinesVAO->AddVertexBuffer(std::move(lines_positions_vbo));
    m_LinesVAO->AddVertexBuffer(std::move(lines_colors_vbo));

    LOG_CORE_INFO("Engine::DebugDrawer >>> successfully initialized :D");
}

auto DebugDrawer::DrawLine(const Vec3& start, const Vec3& end,
                           const Vec3& color) -> void {
    // Keep the line information for the later render stage
    m_Lines.emplace_back(start, end, color);
}

auto DebugDrawer::Render(const Camera& camera) -> void { _RenderLines(camera); }

auto DebugDrawer::_RenderLines(const Camera& camera) -> void {
    if (m_Lines.empty()) {
        return;
    }

    m_ProgramModeWireframe->Bind();
    m_ProgramModeWireframe->SetMat4("u_proj_matrix", camera.proj_matrix());
    m_ProgramModeWireframe->SetMat4("u_view_matrix", camera.view_matrix());

    auto num_full_batches = m_Lines.size() / LINES_BATCH_SIZE;
    auto num_remaining_lines = m_Lines.size() % LINES_BATCH_SIZE;

    for (size_t i = 0; i < num_full_batches; ++i) {
        _RenderLinesBatch(LINES_BATCH_SIZE);
    }
    if (num_remaining_lines > 0) {
        _RenderLinesBatch(num_remaining_lines);
    }
}

auto DebugDrawer::_RenderLinesBatch(size_t num_lines) -> void {
    m_LinesVAO->Bind();
    const auto POSITIONS_VBO_SIZE = sizeof(Vec3) * 2 * num_lines;
    const auto COLORS_VBO_SIZE = sizeof(Vec3) * num_lines;

    auto& buffers = m_LinesVAO->buffers();
    auto& positions_vbo = buffers[0];
    auto& colors_vbo = buffers[1];

    positions_vbo->UpdateData(static_cast<uint32_t>(POSITIONS_VBO_SIZE),
                              nullptr);
    colors_vbo->UpdateData(static_cast<uint32_t>(COLORS_VBO_SIZE), nullptr);

    m_LinesVAO->Unbind();
}

}  // namespace renderer

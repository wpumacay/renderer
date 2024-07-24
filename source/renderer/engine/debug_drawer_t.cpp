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
out vec4 color;

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

    BufferLayout buff_layout = {{"position", eElementType::FLOAT_3, false},
                                {"color", eElementType::FLOAT_3, false}};

    auto lines_vbo = std::make_unique<VertexBuffer>(
        buff_layout, eBufferUsage::DYNAMIC, LINES_VBO_SIZE, nullptr);

    m_LinesVAO = std::make_unique<VertexArray>();
    m_LinesVAO->AddVertexBuffer(std::move(lines_vbo));

    LOG_CORE_INFO("Engine::DebugDrawer >>> successfully initialized :D");
}

auto DebugDrawer::DrawLine(const Vec3& start, const Vec3& end,
                           const Vec3& color) -> void {
    // Keep the line information for the later render stage
    m_Lines.emplace_back(start, end, color);
}

auto DebugDrawer::DrawBox(const Vec3& size, const Mat4& tf, const Vec3& color)
    -> void {
    // List the box vertices in local-space
    auto half_width = 0.5F * size.x();
    auto half_height = 0.5F * size.y();
    auto half_depth = 0.5F * size.z();
    constexpr size_t NUM_BOX_VERTICES = 8;

    std::array<Vec3, NUM_BOX_VERTICES> vertices = {
        Vec3(half_width, -half_height, -half_depth),
        Vec3(half_width, half_height, -half_depth),
        Vec3(-half_width, half_height, -half_depth),
        Vec3(-half_width, -half_height, -half_depth),
        Vec3(half_width, -half_height, half_depth),
        Vec3(half_width, half_height, half_depth),
        Vec3(-half_width, half_height, half_depth),
        Vec3(-half_width, -half_height, half_depth)};

    //  Transform from local-space to world-space using the world transform
    for (auto& vertex : vertices) {
        vertex = Vec3(tf * Vec4(vertex));
    }

    DrawLine(vertices[0], vertices[1], color);
    DrawLine(vertices[1], vertices[2], color);
    DrawLine(vertices[2], vertices[3], color);
    DrawLine(vertices[3], vertices[0], color);

    DrawLine(vertices[1], vertices[5], color);
    DrawLine(vertices[5], vertices[6], color);
    DrawLine(vertices[6], vertices[2], color);
    DrawLine(vertices[2], vertices[1], color);

    DrawLine(vertices[5], vertices[6], color);
    DrawLine(vertices[6], vertices[7], color);
    DrawLine(vertices[7], vertices[4], color);
    DrawLine(vertices[4], vertices[5], color);

    DrawLine(vertices[4], vertices[7], color);
    DrawLine(vertices[7], vertices[3], color);
    DrawLine(vertices[3], vertices[0], color);
    DrawLine(vertices[0], vertices[4], color);

    DrawLine(vertices[2], vertices[6], color);
    DrawLine(vertices[6], vertices[7], color);
    DrawLine(vertices[7], vertices[3], color);
    DrawLine(vertices[3], vertices[2], color);

    DrawLine(vertices[1], vertices[5], color);
    DrawLine(vertices[5], vertices[4], color);
    DrawLine(vertices[4], vertices[0], color);
    DrawLine(vertices[0], vertices[1], color);
}

auto DebugDrawer::DrawSphere(float radius, const Mat4& tf, const Vec3& color)
    -> void {
    constexpr size_t NUM_CIRCLE_POINTS = 10;
    // Create the vertices for the main circles (xy, yz, xz)
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_xy;
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_yz;
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_xz;
    for (size_t i = 0; i < NUM_CIRCLE_POINTS; ++i) {
        auto theta = 2.0F * PI * static_cast<float>(i) /
                     static_cast<float>(NUM_CIRCLE_POINTS);
        auto r_cos_theta = radius * std::cos(theta);
        auto r_sin_theta = radius * std::sin(theta);
        circle_pts_xy.at(i) = {r_cos_theta, r_sin_theta, 0.0F};
        circle_pts_yz.at(i) = {0.0F, r_cos_theta, r_sin_theta};
        circle_pts_xz.at(i) = {r_sin_theta, 0.0F, r_cos_theta};
    }

    // Transform all points into world space
    for (auto& vertex : circle_pts_xy) {
        vertex = Vec3(tf * Vec4(vertex));
    }
    for (auto& vertex : circle_pts_yz) {
        vertex = Vec3(tf * Vec4(vertex));
    }
    for (auto& vertex : circle_pts_xz) {
        vertex = Vec3(tf * Vec4(vertex));
    }

    // Draw each main circle
    for (size_t i = 0; i < circle_pts_xy.size(); ++i) {
        DrawLine(circle_pts_xy.at(i % circle_pts_xy.size()),
                 circle_pts_xy.at((i + 1) % circle_pts_xy.size()), color);
    }
    for (size_t i = 0; i < circle_pts_yz.size(); ++i) {
        DrawLine(circle_pts_yz.at(i % circle_pts_yz.size()),
                 circle_pts_yz.at((i + 1) % circle_pts_yz.size()), color);
    }
    for (size_t i = 0; i < circle_pts_xz.size(); ++i) {
        DrawLine(circle_pts_xz.at(i % circle_pts_xz.size()),
                 circle_pts_xz.at((i + 1) % circle_pts_xz.size()), color);
    }
}

auto DebugDrawer::DrawCylinder(float radius, float height, const Mat4& tf,
                               const Vec3& color) -> void {
    constexpr size_t NUM_CIRCLE_POINTS = 10;
    // Create first main rectangle
    std::array<Vec3, 4> rect_pts_yz;
    rect_pts_yz.at(0) = Vec3(0.0F, -radius, -0.5F * height);
    rect_pts_yz.at(1) = Vec3(0.0F, radius, -0.5F * height);
    rect_pts_yz.at(2) = Vec3(0.0F, radius, 0.5F * height);
    rect_pts_yz.at(3) = Vec3(0.0F, -radius, 0.5F * height);

    // Create second main rectangle
    std::array<Vec3, 4> rect_pts_xz;
    rect_pts_xz.at(0) = Vec3(-radius, 0.0F, -0.5F * height);
    rect_pts_xz.at(1) = Vec3(radius, 0.0F, -0.5F * height);
    rect_pts_xz.at(2) = Vec3(radius, 0.0F, 0.5F * height);
    rect_pts_xz.at(3) = Vec3(-radius, 0.0F, 0.5F * height);

    // Create top and bottom base circles
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_top;
    std::array<Vec3, NUM_CIRCLE_POINTS> circle_pts_bottom;
    for (size_t i = 0; i < NUM_CIRCLE_POINTS; ++i) {
        auto theta = 2.0F * PI * static_cast<float>(i) /
                     static_cast<float>(NUM_CIRCLE_POINTS);
        auto r_cos_theta = radius * std::cos(theta);
        auto r_sin_theta = radius * std::sin(theta);
        circle_pts_top.at(i) = {r_cos_theta, r_sin_theta, 0.5F * height};
        circle_pts_bottom.at(i) = {r_cos_theta, r_sin_theta, -0.5F * height};
    }

    // Transform all points into world space
    for (auto& vertex : rect_pts_yz) {
        vertex = Vec3(tf * Vec4(vertex));
    }
    for (auto& vertex : rect_pts_xz) {
        vertex = Vec3(tf * Vec4(vertex));
    }
    for (auto& vertex : circle_pts_top) {
        vertex = Vec3(tf * Vec4(vertex));
    }
    for (auto& vertex : circle_pts_bottom) {
        vertex = Vec3(tf * Vec4(vertex));
    }

    // Draw all components of our cylinder
    for (size_t i = 0; i < rect_pts_yz.size(); ++i) {
        DrawLine(rect_pts_yz.at(i % rect_pts_yz.size()),
                 rect_pts_yz.at((i + 1) % rect_pts_yz.size()), color);
    }
    for (size_t i = 0; i < rect_pts_xz.size(); ++i) {
        DrawLine(rect_pts_xz.at(i % rect_pts_xz.size()),
                 rect_pts_xz.at((i + 1) % rect_pts_xz.size()), color);
    }
    for (size_t i = 0; i < circle_pts_top.size(); ++i) {
        DrawLine(circle_pts_top.at(i % circle_pts_top.size()),
                 circle_pts_top.at((i + 1) % circle_pts_top.size()), color);
    }
    for (size_t i = 0; i < circle_pts_bottom.size(); ++i) {
        DrawLine(circle_pts_bottom.at(i % circle_pts_bottom.size()),
                 circle_pts_bottom.at((i + 1) % circle_pts_bottom.size()),
                 color);
    }
}

auto DebugDrawer::Render(const Camera& camera) -> void { _RenderLines(camera); }

auto DebugDrawer::_RenderLines(const Camera& camera) -> void {
    if (m_Lines.empty()) {
        return;
    }

    m_ProgramModeWireframe->Bind();
    m_ProgramModeWireframe->SetMat4("u_proj_matrix", camera.proj_matrix());
    m_ProgramModeWireframe->SetMat4("u_view_matrix", camera.view_matrix());

    auto num_full_batches =
        static_cast<uint32_t>(m_Lines.size() / LINES_BATCH_SIZE);
    auto num_remaining_lines =
        static_cast<uint32_t>(m_Lines.size() % LINES_BATCH_SIZE);

    LinesBuffer lines_buffer{};

    for (uint32_t i = 0; i < num_full_batches; ++i) {
        for (uint32_t j = 0; j < LINES_BATCH_SIZE; ++i) {
            _UpdateLineInBuffer(lines_buffer, m_Lines, i, j);
        }
        _RenderLinesBatch(*m_LinesVAO, LINES_BATCH_SIZE, lines_buffer.data());
    }
    if (num_remaining_lines > 0) {
        for (uint32_t j = 0; j < num_remaining_lines; ++j) {
            _UpdateLineInBuffer(lines_buffer, m_Lines, num_full_batches, j);
        }
        _RenderLinesBatch(*m_LinesVAO, num_remaining_lines,
                          lines_buffer.data());
    }

    m_ProgramModeWireframe->Unbind();
    m_Lines.clear();
}

// NOLINTNEXTLINE
auto DebugDrawer::_RenderLinesBatch(VertexArray& lines_vao,
                                    uint32_t lines_number,
                                    const float32_t* lines_data) -> void {
    auto& lines_vbo = *lines_vao.buffers().front();

    uint32_t vbo_subsize =
        (POSITIONS_PER_LINE + COLORS_PER_LINE) * sizeof(Vec3) * lines_number;

    lines_vao.Bind();
    lines_vbo.UpdateData(vbo_subsize, lines_data);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lines_number * 2));
    lines_vao.Unbind();
}

// NOLINTNEXTLINE
auto DebugDrawer::_UpdateLineInBuffer(LinesBuffer& lines_buffer,
                                      const LinesContainer& lines_container,
                                      uint32_t batch_index, uint32_t line_index)
    -> void {
    const auto& line =
        lines_container[batch_index * LINES_BATCH_SIZE + line_index];
    const auto VERTEX_START_INDEX = FLOATS_PER_VERTEX * line_index;

    lines_buffer.at(VERTEX_START_INDEX + 0) = line.start.x();
    lines_buffer.at(VERTEX_START_INDEX + 1) = line.start.y();
    lines_buffer.at(VERTEX_START_INDEX + 2) = line.start.z();

    lines_buffer.at(VERTEX_START_INDEX + 3) = line.color.x();
    lines_buffer.at(VERTEX_START_INDEX + 4) = line.color.y();
    lines_buffer.at(VERTEX_START_INDEX + 5) = line.color.z();

    lines_buffer.at(VERTEX_START_INDEX + 6) = line.end.x();
    lines_buffer.at(VERTEX_START_INDEX + 7) = line.end.y();
    lines_buffer.at(VERTEX_START_INDEX + 8) = line.end.z();

    lines_buffer.at(VERTEX_START_INDEX + 9) = line.color.x();
    lines_buffer.at(VERTEX_START_INDEX + 10) = line.color.y();
    lines_buffer.at(VERTEX_START_INDEX + 11) = line.color.z();
}

}  // namespace renderer

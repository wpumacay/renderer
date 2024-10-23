#include <string>
#include <utility>

#include <glad/gl.h>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/backend/graphics/opengl/vertex_buffer_opengl_t.hpp>

namespace renderer {
namespace opengl {

auto ToOpenGLEnum(eBufferUsage usage) -> uint32_t {
    switch (usage) {
        case eBufferUsage::STATIC:
            return GL_STATIC_DRAW;
        case eBufferUsage::DYNAMIC:
            return GL_DYNAMIC_DRAW;
        default:
            return GL_STATIC_DRAW;
    }
}

OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLBufferLayout layout,
                                       eBufferUsage usage, uint32_t size,
                                       const float32_t* data)
    : m_Layout(std::move(layout)), m_Usage(usage), m_Size(size) {
    glGenBuffers(1, &m_OpenGLId);
    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLId);
    glBufferData(GL_ARRAY_BUFFER, m_Size, data, ToOpenGLEnum(m_Usage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    if (m_OpenGLId != 0) {
        glDeleteBuffers(1, &m_OpenGLId);
        m_OpenGLId = 0;
    }
}

auto OpenGLVertexBuffer::Resize(uint32_t size) -> void {
    if (m_Size == size) {
        return;  // no need to resize :)
    }

    m_Size = size;
    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLId);
    glBufferData(GL_ARRAY_BUFFER, m_Size, nullptr, ToOpenGLEnum(m_Usage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto OpenGLVertexBuffer::UpdateData(uint32_t size, const float32_t* data)
    -> void {
    if (m_Size != size) {
        Resize(size);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_Size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto OpenGLVertexBuffer::Bind() const -> void {
    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLId);
}

// NOLINTNEXTLINE
auto OpenGLVertexBuffer::Unbind() const -> void {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto OpenGLVertexBuffer::ToString() const -> std::string {
    return fmt::format(
        "<OpenGLVertexBuffer\n"
        "  size: {0}\n"
        "  usage: {1}\n"
        "  opengl-id: {2}\n"
        "?\n",
        m_Size, ::renderer::ToString(m_Usage), m_OpenGLId);
}

}  // namespace opengl

}  // namespace renderer

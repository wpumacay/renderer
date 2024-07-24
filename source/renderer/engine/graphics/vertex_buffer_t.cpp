#include <glad/gl.h>

#include <renderer/core/vertex_buffer_t.hpp>
#include <spdlog/fmt/bundled/format.h>

namespace renderer {

auto ToString(const eBufferUsage& usage) -> std::string {
    switch (usage) {
        case eBufferUsage::STATIC:
            return "Static";
        case eBufferUsage::DYNAMIC:
            return "Dynamic";
        default:
            return "undefined";
    }
}

auto ToOpenGLEnum(const eBufferUsage& usage) -> uint32_t {
    switch (usage) {
        case eBufferUsage::STATIC:
            return GL_STATIC_DRAW;
        case eBufferUsage::DYNAMIC:
            return GL_DYNAMIC_DRAW;
        default:
            return GL_STATIC_DRAW;
    }
}

VertexBuffer::VertexBuffer(BufferLayout layout, const eBufferUsage& usage,
                           uint32_t buffer_size, const float32_t* buffer_data)
    : m_Layout(std::move(layout)), m_Usage(usage), m_Size(buffer_size) {
    glGenBuffers(1, &m_OpenGLId);
    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLId);
    glBufferData(GL_ARRAY_BUFFER, m_Size, buffer_data, ToOpenGLEnum(m_Usage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_OpenGLId); }

auto VertexBuffer::Resize(uint32_t size) -> void {
    if (m_Size == size) {
        return;  // no need to resize :)
    }

    m_Size = size;
    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLId);
    glBufferData(GL_ARRAY_BUFFER, m_Size, nullptr, ToOpenGLEnum(m_Usage));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto VertexBuffer::UpdateData(uint32_t size, const float32_t* data) -> void {
    if (m_Size != size) {
        Resize(size);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLId);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_Size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto VertexBuffer::Bind() const -> void {
    glBindBuffer(GL_ARRAY_BUFFER, m_OpenGLId);
}

// NOLINTNEXTLINE
auto VertexBuffer::Unbind() const -> void { glBindBuffer(GL_ARRAY_BUFFER, 0); }

auto VertexBuffer::ToString() const -> std::string {
    std::string str_repr = "VertexBuffer";
    str_repr += fmt::format("(size={0}, usage={1}, opengl-id={2})", m_Size,
                            renderer::ToString(m_Usage), m_OpenGLId);
    return str_repr;
}

}  // namespace renderer

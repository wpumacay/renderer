
#include <glad/gl.h>

#include <renderer/core/index_buffer_t.hpp>
#include <spdlog/fmt/bundled/format.h>

namespace renderer {

IndexBuffer::IndexBuffer(eBufferUsage usage, uint32_t count,
                         const uint32_t* data)
    : m_Usage(usage), m_Count(count) {
    const uint32_t SIZE = m_Count * sizeof(uint32_t);
    glGenBuffers(1, &m_OpenGLId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGLId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE, data, ToOpenGLEnum(m_Usage));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer() {
    if (m_OpenGLId != 0) {
        glDeleteBuffers(1, &m_OpenGLId);
        m_OpenGLId = 0;
    }
}

auto IndexBuffer::Bind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGLId);
}

// NOLINTNEXTLINE
auto IndexBuffer::Unbind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

auto IndexBuffer::ToString() const -> std::string {
    std::string str_repr = "IndexBuffer";
    str_repr += fmt::format("(count={0}, usage={1}, opengl_id={2})", m_Count,
                            renderer::ToString(m_Usage), m_OpenGLId);
    return str_repr;
}

}  // namespace renderer

#include <string>

#include <glad/gl.h>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/backend/graphics/opengl/index_buffer_opengl_t.hpp>
#include "renderer/backend/graphics/opengl/vertex_buffer_opengl_t.hpp"

namespace renderer {
namespace opengl {

OpenGLIndexBuffer::OpenGLIndexBuffer(eBufferUsage usage, uint32_t count,
                                     const uint32_t* data)
    : m_Usage(usage), m_Count(count) {
    const uint32_t SIZE = m_Count * sizeof(uint32_t);
    glGenBuffers(1, &m_OpenGLId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGLId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, SIZE, data, ToOpenGLEnum(m_Usage));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    if (m_OpenGLId != 0) {
        glDeleteBuffers(1, &m_OpenGLId);
        m_OpenGLId = 0;
    }
}

auto OpenGLIndexBuffer::Bind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OpenGLId);
}

// NOLINTNEXTLINE
auto OpenGLIndexBuffer::Unbind() const -> void {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

auto OpenGLIndexBuffer::ToString() const -> std::string {
    return fmt::format(
        "<OpenGLIndexBuffer\n"
        "  count: {0}\n"
        "  usage: {1}\n"
        "  opengl-id: {2}\n",
        ">\n", m_Count, ::renderer::ToString(m_Usage), m_OpenGLId);
}

}  // namespace opengl

}  // namespace renderer

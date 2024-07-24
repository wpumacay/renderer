#pragma once

#include <vector>
#include <string>
#include <memory>

#include <renderer/common.hpp>
#include <renderer/core/vertex_buffer_t.hpp>
#include <renderer/core/index_buffer_t.hpp>

namespace renderer {

class VertexArray {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(VertexArray)

    DEFINE_SMART_POINTERS(VertexArray)

 public:
    /// Creates a VAO on the GPU
    VertexArray();

    /// Releases the resources allocated  and owned by this VAO
    ~VertexArray();

    /// Adds the given VBO to the group managed by this VAO
    auto AddVertexBuffer(VertexBuffer::ptr buffer, bool is_instanced = false)
        -> void;

    /// Sets the given IBO to the group managed by this VAO
    auto SetIndexBuffer(IndexBuffer::ptr buffer) -> void;

    /// Binds this VAO, setting the OpenGL-FSM to the appropriate state
    auto Bind() const -> void;

    /// Unbinds this VAO from the OpenGL-FSM
    auto Unbind() const -> void;

    /// Returns a mutable reference to the container of our buffers
    auto buffers() -> std::vector<VertexBuffer::ptr>& { return m_Buffers; }

    /// Returns an unmutable reference to the container of our buffers
    auto buffers() const -> const std::vector<VertexBuffer::ptr>& {
        return m_Buffers;
    }

    /// Returns a mutable reference to the index buffer
    auto index_buffer() -> IndexBuffer& { return *m_IndexBuffer; }

    /// Returns an unmutable reference to the index buffer
    auto index_buffer() const -> const IndexBuffer& { return *m_IndexBuffer; }

    /// Returns the id associated with this object on the GPU
    auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    ///  Returns the current number of attributes managed by the VAO
    auto num_attribs() const -> uint32_t { return m_NumAttribIndx; }

    /// Returns the number of buffers associated with this VAO
    auto num_buffers() const -> uint32_t {
        return static_cast<uint32_t>(m_Buffers.size());
    }

    /// Returns a string representation of this VAO
    auto ToString() const -> std::string;

 private:
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId = 0;
    /// Index of the current slot in the attributePointer list of the VAO
    uint32_t m_NumAttribIndx = 0;
    /// Container for the owned VBOs
    std::vector<VertexBuffer::ptr> m_Buffers;
    /// Index Buffer associated with this VAO (if applicable)
    IndexBuffer::ptr m_IndexBuffer;
};

}  // namespace renderer

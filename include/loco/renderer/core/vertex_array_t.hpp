#pragma once

#include <vector>
#include <string>
#include <memory>

#include <loco/renderer/common.hpp>
#include <loco/renderer/core/vertex_buffer_t.hpp>
#include <loco/renderer/core/index_buffer_t.hpp>

namespace loco {
namespace renderer {

class VertexArray {
    // cppcheck-suppress unknownMacro
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(VertexArray)

    LOCO_DEFINE_SMART_POINTERS(VertexArray)

 public:
    /// Creates a VAO on the GPU
    VertexArray();

    /// Releases the resources allocated  and owned by this VAO
    ~VertexArray();

    /// Adds the given VBO to the group managed by this VAO
    auto AddVertexBuffer(VertexBuffer::uptr buffer, bool is_instanced = false)
        -> void;

    /// Sets the given IBO to the group managed by this VAO
    auto SetIndexBuffer(IndexBuffer::uptr buffer) -> void;

    /// Binds this VAO, setting the OpenGL-FSM to the appropriate state
    auto Bind() const -> void;

    /// Unbinds this VAO from the OpenGL-FSM
    auto Unbind() const -> void;

    /// Returns a mutable reference to the container of our buffers
    auto buffers() -> std::vector<VertexBuffer::uptr>& { return m_Buffers; }

    /// Returns an unmutable reference to the container of our buffers
    auto buffers() const -> const std::vector<VertexBuffer::uptr>& {
        return m_Buffers;
    }

    /// Returns the id associated with this object on the GPU
    auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    ///  Returns the current number of attributes managed by the VAO
    auto num_attribs() const -> uint32_t { return m_NumAttribIndx; }

    /// Returns a string representation of this VAO
    auto ToString() const -> std::string;

 private:
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId = 0;
    /// Index of the current slot in the attributePointer list of the VAO
    uint32_t m_NumAttribIndx = 0;
    /// Container for the owned VBOs
    std::vector<VertexBuffer::uptr> m_Buffers;
    /// Index Buffer associated with this VAO (if applicable)
    IndexBuffer::uptr m_IndexBuffer;
};

}  // namespace renderer
}  // namespace loco

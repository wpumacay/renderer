#pragma once

#include <string>

#include <renderer/common.hpp>
#include <renderer/core/vertex_buffer_t.hpp>

namespace renderer {

/// Index Buffer Object (IBO|EBO), used to store indices for primitives
class IndexBuffer {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(IndexBuffer)

    NO_COPY_NO_MOVE_NO_ASSIGN(IndexBuffer)

 public:
    /// Creates  an Index Buffer  given some indices data
    explicit IndexBuffer(eBufferUsage usage, uint32_t count,
                         const uint32_t* data);

    /// Releases the resources allocated by this IBO
    ~IndexBuffer();

    /// Binds this buffer for its usage in the graphics pipeline
    auto Bind() const -> void;

    /// Unbinds this buffer from the graphics pipeline
    auto Unbind() const -> void;

    /// Returns the number of indices contained in this buffer
    auto count() const -> uint32_t { return m_Count; }

    /// Returns the id of the OpenGL resource allocated for this buffer
    auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    /// Returns a string representation of this index buffer
    auto ToString() const -> std::string;

 private:
    /// Type of intended usage for this buffer
    eBufferUsage m_Usage = eBufferUsage::STATIC;
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId = 0;
    /// Number of indices stored in this buffer
    uint32_t m_Count = 0;
};

}  // namespace renderer

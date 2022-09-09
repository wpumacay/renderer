#pragma once

#include <string>

#include <loco/renderer/common.hpp>
#include <loco/renderer/core/vertex_buffer_object_t.hpp>

namespace loco {
namespace renderer {

/// Index Buffer Object (IBO|EBO), used to store indices for primitives
class IndexBuffer {
    // cppcheck-suppress unknownMacro
    LOCO_DEFINE_SMART_POINTERS(IndexBuffer)

    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(IndexBuffer);

 public:
    explicit IndexBuffer(eBufferUsage usage, uint32_t count,
                         const uint32_t* data);

    ~IndexBuffer();

    auto Bind() const -> void;

    auto Unbind() const -> void;

    auto ToString() const -> std::string;

    auto count() const -> uint32_t { return m_Count; }

    auto opengl_id() const -> uint32_t { return m_OpenGLId; }

 private:
    /// Type of intended usage for this buffer
    eBufferUsage m_Usage = eBufferUsage::STATIC;
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId = 0;
    /// Number of indices stored in this buffer
    uint32_t m_Count = 0;
};

}  // namespace renderer
}  // namespace loco

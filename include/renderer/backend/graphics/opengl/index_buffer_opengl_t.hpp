#pragma once

#include <string>

#include <renderer/engine/graphics/enums.hpp>
#include "renderer/common.hpp"

namespace renderer {
namespace opengl {

/// Index Buffer Object (IBO|EBO), used to store indices for primitives
class RENDERER_API OpenGLIndexBuffer {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(OpenGLIndexBuffer)

    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLIndexBuffer)

 public:
    /// Creates  an Index Buffer  given some indices data
    explicit OpenGLIndexBuffer(eBufferUsage usage, uint32_t count,
                               const uint32_t* data);

    /// Releases the resources allocated by this IBO
    ~OpenGLIndexBuffer();

    /// Binds this buffer for its usage in the graphics pipeline
    auto Bind() const -> void;

    /// Unbinds this buffer from the graphics pipeline
    auto Unbind() const -> void;

    /// Returns the usage of this IBO
    RENDERER_NODISCARD auto usage() const -> eBufferUsage { return m_Usage; }

    /// Returns the number of indices contained in this buffer
    RENDERER_NODISCARD auto count() const -> uint32_t { return m_Count; }

    /// Returns the id of the OpenGL resource allocated for this buffer
    RENDERER_NODISCARD auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    /// Returns a string representation of this index buffer
    RENDERER_NODISCARD auto ToString() const -> std::string;

 private:
    /// Type of intended usage for this buffer
    eBufferUsage m_Usage{eBufferUsage::STATIC};
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId{0};
    /// Number of indices stored in this buffer
    uint32_t m_Count{0};
};

}  // namespace opengl

}  // namespace renderer

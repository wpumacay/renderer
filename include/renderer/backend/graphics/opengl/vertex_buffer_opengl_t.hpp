#pragma once

#include <string>

#include <renderer/engine/graphics/enums.hpp>
#include <renderer/backend/graphics/opengl/vertex_buffer_layout_opengl_t.hpp>

namespace renderer {
namespace opengl {

/// Returns the corresponding OpenGL enum for a given buffer usage
RENDERER_API auto ToOpenGLEnum(eBufferUsage usage) -> uint32_t;

/// Vertex Buffer Object (VBO), used to store data on the GPU memory
class RENDERER_API OpenGLVertexBuffer {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(OpenGLVertexBuffer)

    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLVertexBuffer)

 public:
    /// Creates a VBO initialized with the given parameters
    /// \param[in] layout The layout used for the data in the GPU buffer
    /// \param[in] usage The type of usage for this buffer
    /// \param[in] size The size in bytes of the buffer
    /// \param[in] data A pointer to the data in CPU from where to copy the data
    explicit OpenGLVertexBuffer(OpenGLBufferLayout layout, eBufferUsage usage,
                                uint32_t size, const float32_t* data);

    /// Releases the resources allocated by this VBO
    ~OpenGLVertexBuffer();

    /// Resizes the buffer to the requested size (in bytes)
    auto Resize(uint32_t size) -> void;

    /// Updates the chunk of memory associated with this buffer on the GPU
    /// \param size How much data (in bytes) will be updated
    /// \param data A pointer to the data to be transferred
    auto UpdateData(uint32_t size, const float32_t* data) -> void;

    /// Binds the current buffer to the appropriate state of the pipeline
    auto Bind() const -> void;

    /// Unbinds the current buffer from the appropriate state of the pipeline
    auto Unbind() const -> void;

    /// Returns a copy of the internal layout of the memor on the GPUy
    RENDERER_NODISCARD auto layout() const -> OpenGLBufferLayout {
        return m_Layout;
    }

    /// Returns the size (in bytes) of this buffer
    RENDERER_NODISCARD auto size() const -> uint32_t { return m_Size; }

    /// Returns the type of usage of this buffer
    RENDERER_NODISCARD auto usage() const -> eBufferUsage { return m_Usage; }

    /// Returns the OpenGL identifier for this object
    RENDERER_NODISCARD auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    /// Returns a string representation of the main information of this buffer
    RENDERER_NODISCARD auto ToString() const -> std::string;

 private:
    /// Layour representation of the memory on the GPU
    OpenGLBufferLayout m_Layout{};
    /// Usage hint for the type of buffer (either dynamic or static)
    eBufferUsage m_Usage{eBufferUsage::STATIC};
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId{0};
    /// Size (in bytes) of the chunk of memory on the GPU
    uint32_t m_Size{0};
};

}  // namespace opengl

}  // namespace renderer

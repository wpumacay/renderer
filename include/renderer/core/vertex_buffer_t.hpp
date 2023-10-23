#pragma once

#include <string>

#include <renderer/core/vertex_buffer_layout_t.hpp>

namespace renderer {

/// Available modes in which a VBO can be used
enum class eBufferUsage {
    STATIC,  //< A chunk of GPU memory that won't change during execution
    DYNAMIC  //< A chunk of GPU memory that could change during execution
};

/// Returns the string representation of the given buffer usage
auto ToString(const eBufferUsage& usage) -> std::string;

/// Returns the corresponding OpenGL enum for a given buffer usage
auto ToOpenGLEnum(const eBufferUsage& usage) -> uint32_t;

/// Vertex Buffer Object (VBO), used to store data on the GPU memory
class VertexBuffer {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(VertexBuffer)

    NO_COPY_NO_MOVE_NO_ASSIGN(VertexBuffer)

 public:
    explicit VertexBuffer(BufferLayout layout, const eBufferUsage& usage,
                          uint32_t buffer_size, const float32_t* buffer_data);

    /// Releases the resources allocated by this VBO
    ~VertexBuffer();

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
    auto layout() const -> BufferLayout { return m_Layout; }

    /// Returns the size (in bytes) of this buffer
    auto size() const -> uint32_t { return m_Size; }

    /// Returns the type of usage of this buffer
    auto usage() const -> eBufferUsage { return m_Usage; }

    /// Returns the OpenGL identifier for this object
    auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    /// Returns a string representation of the main information of this buffer
    auto ToString() const -> std::string;

 private:
    /// Layour representation of the memory on the GPU
    BufferLayout m_Layout{};
    /// Usage hint for the type of buffer (either dynamic or static)
    eBufferUsage m_Usage = eBufferUsage::STATIC;
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId = 0;
    /// Size (in bytes) of the chunk of memory on the GPU
    uint32_t m_Size = 0;
};

}  // namespace renderer

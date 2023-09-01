#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <initializer_list>

#include <renderer/common.hpp>

namespace renderer {

/// Type of element used for part (or all) elements in a vertex buffer
enum class eElementType {
    FLOAT_1,  ///< Single float, size 4-bytes
    FLOAT_2,  ///< Two float compound (vec2), size 8-bytes
    FLOAT_3,  ///< Three float compound (vec3), size 12-bytes
    FLOAT_4,  ///< Four float compound (vec4), size 16-bytes
    INT_1,    ///< Single integer, size 4-bytes (=int32)
    INT_2,    ///< Two integer compound (int2), size 8-bytes
    INT_3,    ///< Three integer compound (int3), size 12-bytes
    INT_4,    ///< Four integer compound (int4), size 16-bytes
};

/// Returns the string representation of the given element-type enum
/// \param etype Type of the buffer element
auto ToString(const eElementType& etype) -> std::string;

/// Returns the corresponding OpenGL enum for a given buffer element type
/// \param etype Type of the buffer element
auto ToOpenGLEnum(const eElementType& etype) -> uint32_t;

/// Returns the size (in bytes) of the given buffer element type
/// \param etype Type of the buffer element
auto GetElementSize(const eElementType& etype) -> uint32_t;

/// Returns the number of single components in a given buffer element type
auto GetElementCount(const eElementType& etype) -> uint32_t;

/// Struct describing a given element in a Vertex Buffer
struct BufferElement {
    /// Name of the element in the buffer, mainly for debugging (e.g. position)
    std::string name{};
    /// The type of this element
    eElementType type = eElementType::FLOAT_3;
    /// Number of single-components of the element
    uint32_t count = 3;
    /// Size of the element (in bytes)
    uint32_t size = sizeof(float) * 3;
    /// Offset of the element's position in the buffer (managed by the layout)
    uint32_t offset = 0;
    /// Whether or not we should normalize the element (e.g. when using normals)
    bool normalized = false;

    /// Creates a default vertex Buffer Element
    BufferElement()
        : name("none"), type(eElementType::FLOAT_1), count(0), size(0) {}

    /// Creates a buffer element with the given description
    BufferElement(const char* name, const eElementType& etype, bool normalize);
};

/// Object describing the layout of all (or part) of the data in a vertex buffer
class OpenGLBufferLayout {
 public:
    /// Constructs an empty buffer layout
    OpenGLBufferLayout() = default;

    // cppcheck-suppress noExplicitConstructor
    /// Creates a buffer layout from elements in a initializer list
    OpenGLBufferLayout(const std::initializer_list<BufferElement>& elements);

    /// Creates a buffer layout from elements in a std::vector
    explicit OpenGLBufferLayout(std::vector<BufferElement> elements);

    auto stride() const -> uint32_t { return m_Stride; }

    /// Returns a mutable reference to the elements in the layout
    auto elements() -> std::vector<BufferElement>& { return m_BufferElements; }

    /// Returns an unmutable reference to the elements in the layout
    auto elements() const -> const std::vector<BufferElement>& {
        return m_BufferElements;
    }

    /// Returns a string representation of this layout
    auto ToString() const -> std::string;

 private:
    /// Offset in bytes required to jump between same elements in the buffer
    uint32_t m_Stride = 0;

    /// Container of the elements description stored in this layout
    std::vector<BufferElement> m_BufferElements;
};

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
class OpenGLVertexBuffer {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(OpenGLVertexBuffer)

    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLVertexBuffer);

 public:
    explicit OpenGLVertexBuffer(OpenGLBufferLayout layout,
                                const eBufferUsage& usage, uint32_t buffer_size,
                                const float32_t* buffer_data);

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
    auto layout() const -> OpenGLBufferLayout { return m_Layout; }

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
    OpenGLBufferLayout m_Layout{};
    /// Usage hint for the type of buffer (either dynamic or static)
    eBufferUsage m_Usage = eBufferUsage::STATIC;
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId = 0;
    /// Size (in bytes) of the chunk of memory on the GPU
    uint32_t m_Size = 0;
};

}  // namespace renderer

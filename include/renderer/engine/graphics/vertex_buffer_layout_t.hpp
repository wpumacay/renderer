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
    std::string name{"none"};
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
    BufferElement() = default;

    /// Creates a buffer element with the given description
    BufferElement(const char* e_name, const eElementType& e_type,
                  bool e_normalized)
        : name(e_name),
          type(e_type),
          count(GetElementCount(e_type)),
          size(GetElementSize(e_type)),
          normalized(e_normalized) {}

    /// \brief Returns a string representation of this elements
    auto ToString() const -> std::string;
};

/// Object describing the layout of all (or part) of the data in a vertex buffer
class BufferLayout {
 public:
    /// Constructs an empty buffer layout
    BufferLayout() = default;

    // cppcheck-suppress noExplicitConstructor
    /// Creates a buffer layout from elements in a initializer list
    BufferLayout(const std::initializer_list<BufferElement>& elements);

    /// Creates a buffer layout from elements in a std::vector
    explicit BufferLayout(std::vector<BufferElement> elements);

    /// \brief Adds the given element to this layout
    auto AddElement(BufferElement element) -> void;

    /// \brief Returns a mutable reference to an element given its index
    auto GetElementByIndex(size_t index) -> BufferElement&;

    /// \brief Returns an unmutable reference to an element given its index
    auto GetElementByIndex(size_t index) const -> const BufferElement&;

    /// \brief Returns the number of elements stored by this buffer layout
    auto num_elements() const -> size_t;

    /// \brief Returns the resulting computed stried for this layout
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

}  // namespace renderer

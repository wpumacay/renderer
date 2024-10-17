#pragma once

#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

#include <renderer/engine/graphics/enums.hpp>

namespace renderer {

/// Struct describing a given element in a Vertex Buffer
struct RENDERER_API BufferElement {
    /// Name of the element in the buffer, mainly for debugging (e.g. position)
    std::string name{"none"};
    /// The type of this element
    eElementType type = eElementType::FLOAT_3;
    /// Number of single-components of the element
    uint32_t count = 3;
    /// Size of the element (in bytes)
    uint32_t nbytes = sizeof(float) * 3;
    /// Offset of the element's position in the buffer (managed by the layout)
    uint32_t offset = 0;
    /// Whether or not we should normalize the element (e.g. when using normals)
    bool normalized = false;

    /// Creates a default Vertex Buffer Element
    BufferElement() = default;

    /// Creates a Vertex Buffer Element with the given description
    BufferElement(const char* e_name, eElementType e_type, bool e_normalized)
        : name(e_name),
          type(e_type),
          count(GetElementCount(e_type)),
          nbytes(GetElementSize(e_type)),
          normalized(e_normalized) {}

    /// \brief Returns a string representation of this elements
    RENDERER_NODISCARD auto ToString() const -> std::string;
};

/// Object describing the layout of all (or part) of the data in a Vertex Buffer
class RENDERER_API BufferLayout {
 public:
    /// Constructs an empty buffer layout
    BufferLayout() = default;

    // cppcheck-suppress noExplicitConstructor
    /// Creates a buffer layout from elements in a initializer list
    BufferLayout(const std::initializer_list<BufferElement>& elements);

    /// Creates a buffer layout from elements in a std::vector
    explicit BufferLayout(std::vector<BufferElement> elements);

    /// Adds the given element to this layout
    auto AddElement(BufferElement element) -> void;

    /// Returns a copy of the buffer element at the requested index
    auto operator[](size_t index) const -> BufferElement;

    /// Returns the number of elements stored by this buffer layout
    RENDERER_NODISCARD auto size() const -> size_t {
        return m_BufferElements.size();
    }

    // Returns the resulting computed stried for this layout
    RENDERER_NODISCARD auto stride() const -> uint32_t { return m_Stride; }

    /// Returns a string representation of this layout
    RENDERER_NODISCARD auto ToString() const -> std::string;

 private:
    /// Offset in bytes required to jump between same elements in the buffer
    uint32_t m_Stride = 0;

    /// Container of the elements description stored in this layout
    std::vector<BufferElement> m_BufferElements;
};

}  // namespace renderer

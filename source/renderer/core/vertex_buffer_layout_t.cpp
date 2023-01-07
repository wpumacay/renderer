#include <iostream>
#include <cstdint>

#include <glad/gl.h>

#include <renderer/core/vertex_buffer_layout_t.hpp>
#include <spdlog/fmt/bundled/format.h>

namespace renderer {

auto ToString(const eElementType& etype) -> std::string {
    switch (etype) {
        case eElementType::FLOAT_1:
            return "Float1";
        case eElementType::FLOAT_2:
            return "Float2";
        case eElementType::FLOAT_3:
            return "Float3";
        case eElementType::FLOAT_4:
            return "Float4";
        case eElementType::INT_1:
            return "Int1";
        case eElementType::INT_2:
            return "Int2";
        case eElementType::INT_3:
            return "Int3";
        case eElementType::INT_4:
            return "Int4";
    }
}

auto ToOpenGLEnum(const eElementType& etype) -> uint32_t {
    switch (etype) {
        case eElementType::FLOAT_1:
        case eElementType::FLOAT_2:
        case eElementType::FLOAT_3:
        case eElementType::FLOAT_4:
            return GL_FLOAT;
        case eElementType::INT_1:
        case eElementType::INT_2:
        case eElementType::INT_3:
        case eElementType::INT_4:
            return GL_INT;
    }
}

auto GetElementSize(const eElementType& etype) -> uint32_t {
    switch (etype) {
        case eElementType::FLOAT_1:
            return 4 * 1;
        case eElementType::FLOAT_2:
            return 4 * 2;
        case eElementType::FLOAT_3:
            return 4 * 3;
        case eElementType::FLOAT_4:
            return 4 * 4;
        case eElementType::INT_1:
            return 4 * 1;
        case eElementType::INT_2:
            return 4 * 2;
        case eElementType::INT_3:
            return 4 * 3;
        case eElementType::INT_4:
            return 4 * 4;
    }
}

auto GetElementCount(const eElementType& etype) -> uint32_t {
    switch (etype) {
        case eElementType::FLOAT_1:
            return 1;
        case eElementType::FLOAT_2:
            return 2;
        case eElementType::FLOAT_3:
            return 3;
        case eElementType::FLOAT_4:
            return 4;
        case eElementType::INT_1:
            return 1;
        case eElementType::INT_2:
            return 2;
        case eElementType::INT_3:
            return 3;
        case eElementType::INT_4:
            return 4;
    }
}

BufferElement::BufferElement(const char* name, const eElementType& etype,
                             bool normalize)
    : name(name),
      type(etype),
      count(GetElementCount(etype)),
      size(GetElementSize(etype)),
      normalized(normalize) {}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
    : m_BufferElements(elements) {
    for (auto& element : m_BufferElements) {
        element.offset = m_Stride;
        m_Stride += element.size;
    }
}

BufferLayout::BufferLayout(std::vector<BufferElement> elements)
    : m_BufferElements(std::move(elements)) {
    for (auto& element : m_BufferElements) {
        element.offset = m_Stride;
        m_Stride += element.size;
    }
}

auto BufferLayout::ToString() const -> std::string {
    uint32_t index = 0;
    auto last_index = static_cast<uint32_t>(m_BufferElements.size() - 1);
    std::string str_repr{"BufferLayout({"};
    for (const auto& element : m_BufferElements) {
        str_repr += fmt::format(
            "{0}[name=\"{1}\", type={2}, count={3}, size={4}, offset={5}, "
            "normalized={6}]",
            (index != 0 ? "              " : ""), element.name,
            renderer::ToString(element.type), element.count, element.size,
            element.offset, (element.normalized ? "true" : "false"));
        str_repr += (index != last_index ? "\n" : "");
        index++;
    }
    str_repr += "})";
    return str_repr;
}

}  // namespace renderer

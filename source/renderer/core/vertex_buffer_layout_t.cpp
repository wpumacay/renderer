#include <stdexcept>

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
        default:
            return "undefined";
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
        default:
            return GL_FLOAT;
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
        default:
            return 4;
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
        default:
            return 1;
    }
}

auto BufferElement::ToString() const -> std::string {
    return fmt::format(
        "<BufferElement\n"
        "  name: {0}\n"
        "  type: {1}\n"
        "  count: {2}\n"
        "  size: {3}\n"
        "  offset: {4}\n"
        "  normalized: {5}\n"
        ">\n",
        name, ::renderer::ToString(type), count, size, offset, normalized);
}

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

auto BufferLayout::AddElement(BufferElement element) -> void {
    element.offset = m_Stride;
    m_Stride += element.size;
    m_BufferElements.push_back(std::move(element));
}

auto BufferLayout::GetElementByIndex(size_t index) -> BufferElement& {
    if (index >= m_BufferElements.size()) {
        throw std::runtime_error(
            fmt::format("BufferLayout::GetElementByIndex >> index {0} out of "
                        "range [0-{1})",
                        index, m_BufferElements.size()));
    }
    return m_BufferElements[index];
}

auto BufferLayout::GetElementByIndex(size_t index) const
    -> const BufferElement& {
    if (index >= m_BufferElements.size()) {
        throw std::runtime_error(
            fmt::format("BufferLayout::GetElementByIndex >> index {0} out of "
                        "range [0-{1})",
                        index, m_BufferElements.size()));
    }
    return m_BufferElements[index];
}

auto BufferLayout::num_elements() const -> size_t {
    return m_BufferElements.size();
}

auto BufferLayout::ToString() const -> std::string {
    uint32_t index = 0;
    auto last_index = static_cast<uint32_t>(m_BufferElements.size() - 1);
    std::string str_repr{"<BufferLayout({\n"};
    for (const auto& element : m_BufferElements) {
        str_repr += fmt::format(
            "  [name={0}, type={1}, count={2}, size={3}, offset={4}, "
            "normalized={5}]\n",
            element.name, renderer::ToString(element.type), element.count,
            element.size, element.offset, element.normalized);
        str_repr += (index != last_index ? "\n" : "");
        index++;
    }
    str_repr += "})>\n";
    return str_repr;
}

}  // namespace renderer

#include <stdexcept>
#include <string>
#include <utility>

#include <spdlog/fmt/bundled/format.h>

#include <renderer/engine/graphics/vertex_buffer_layout_t.hpp>

namespace renderer {

auto BufferElement::ToString() const -> std::string {
    return fmt::format(
        "<BufferElement\n"
        "  name: {0}\n"
        "  type: {1}\n"
        "  count: {2}\n"
        "  nbytes: {3}\n"
        "  offset: {4}\n"
        "  normalized: {5}\n"
        ">\n",
        name, ::renderer::ToString(type), count, nbytes, offset, normalized);
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
    : m_BufferElements(elements) {
    for (auto& element : m_BufferElements) {
        element.offset = m_Stride;
        m_Stride += element.nbytes;
    }
}

BufferLayout::BufferLayout(std::vector<BufferElement> elements)
    : m_BufferElements(std::move(elements)) {
    for (auto& element : m_BufferElements) {
        element.offset = m_Stride;
        m_Stride += element.nbytes;
    }
}

auto BufferLayout::AddElement(BufferElement element) -> void {
    element.offset = m_Stride;
    m_Stride += element.nbytes;
    m_BufferElements.push_back(std::move(element));
}

auto BufferLayout::operator[](size_t index) const -> BufferElement {
    if (index >= m_BufferElements.size()) {
        throw std::runtime_error(
            fmt::format("BufferLayout> index {0} out of range [0-{1})", index,
                        m_BufferElements.size()));
    }
    return m_BufferElements[index];
}

auto BufferLayout::ToString() const -> std::string {
    uint32_t index = 0;
    std::string str_repr{"<BufferLayout({\n"};
    for (const auto& element : m_BufferElements) {
        str_repr += fmt::format(
            "  [name={0}, type={1}, count={2}, nbytes={3}, offset={4}, "
            "normalized={5}]\n",
            element.name, renderer::ToString(element.type), element.count,
            element.nbytes, element.offset, element.normalized);
        index++;
    }
    str_repr += "})>\n";
    return str_repr;
}

}  // namespace renderer

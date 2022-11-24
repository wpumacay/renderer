#include <renderer/geometry/geometry_t.hpp>

namespace renderer {

Geometry::Geometry(const float* buff_positions, const float* buff_normals,
                   const float* buff_uvs, size_t n_vertices,
                   const uint32_t* buff_indices, size_t n_indices) {
    SetIndices(buff_indices, n_indices);
    SetAttribute("position", eElementType::FLOAT_3,
                 n_vertices * 3 * sizeof(float), buff_positions, false);
    SetAttribute("normal", eElementType::FLOAT_3,
                 n_vertices * 3 * sizeof(float), buff_normals, true);
    SetAttribute("uvs", eElementType::FLOAT_2, n_vertices * 2 * sizeof(float),
                 buff_uvs, false);
}

auto Geometry::SetIndices(const uint32_t* data, size_t count,
                          const eBufferUsage& usage) -> void {
    auto indices_ibo = std::make_unique<IndexBuffer>(
        usage, static_cast<uint32_t>(count), data);
    m_VAO.SetIndexBuffer(std::move(indices_ibo));
}

auto Geometry::SetAttribute(const std::string& name, const eElementType& etype,
                            size_t size, const float* data, bool normalized,
                            const eBufferUsage& usage) -> void {
    BufferLayout layout = {{name.c_str(), etype, normalized}};
    auto attribute_vbo = std::make_unique<VertexBuffer>(
        layout, usage, static_cast<uint32_t>(size), data);
    m_VAO.AddVertexBuffer(std::move(attribute_vbo));
}

}  // namespace renderer

#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include <renderer/common.hpp>
#include <renderer/core/vertex_array_t.hpp>

namespace renderer {

class Geometry {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Geometry)

    DEFINE_SMART_POINTERS(Geometry)

 public:
    /// Creates an empty geometry without any attributes
    Geometry() = default;

    /// Creates a geometry from the given raw vertex data
    explicit Geometry(const float* buff_positions, const float* buff_normals,
                      const float* buff_uvs, size_t n_vertices,
                      const uint32_t* buff_indices, size_t n_indices);

    /// Creates a geometry from the given vertex data (stored in containers)
    explicit Geometry(const std::vector<Vec3>& positions,
                      const std::vector<Vec3>& normals,
                      const std::vector<Vec2>& uvs,
                      const std::vector<uint32_t>& indices);

    /// Frees/deallocates any internal resources
    ~Geometry() = default;

    /// Sets the current indices to be the ones given by the provided buffer
    /// \param[in] data The buffer containing the indices we want to use
    /// \param[in] count The number of indices stored in the given buffer
    /// \param[in] usage The kind of usage required for the index buffer
    auto SetIndices(const uint32_t* data, size_t count,
                    const eBufferUsage& usage = eBufferUsage::STATIC) -> void;

    /// Sets the appropriate buffer attribute provided some vertex data
    /// \param[in] name The name of the attribute being configured
    /// \param[in] etype The type of element being stored in the given buffer
    /// \param[in] size The size in bytes of the total space used by the buffer
    /// \param[in] data A pointer to the data to be set for this attribute
    /// \param[in] normalized Whether or not the attribute should be normalized
    /// \param[in] usage The kind of usage required for this buffer attribute
    auto SetAttribute(const std::string& name, const eElementType& etype,
                      size_t size, const float* data, bool normalized = false,
                      const eBufferUsage& usage = eBufferUsage::STATIC) -> void;

    /// Returns an unmutable reference to the internal VAO used by this geometry
    auto VAO() const -> const VertexArray& { return m_VAO; }

 private:
    /// Vertex array used to store the vertex data for this geometry
    VertexArray m_VAO;
    // TODO(wilbert): add bounding sphere and bounding box
};

}  // namespace renderer

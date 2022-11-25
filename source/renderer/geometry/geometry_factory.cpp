#include <cstdint>
#include <vector>
#include <glad/gl.h>

#include <renderer/geometry/geometry_factory.hpp>

namespace renderer {

auto CreatePlane(float width, float depth, const eAxis& axis)
    -> Geometry::uptr {
    std::vector<Vec3> positions = {
        _RotateToMatchUpAxis({0.5F * width, -0.5F * depth, 0.0F}, axis),
        _RotateToMatchUpAxis({0.5F * width, 0.5F * depth, 0.0F}, axis),
        _RotateToMatchUpAxis({-0.5F * width, 0.5F * depth, 0.0F}, axis),
        _RotateToMatchUpAxis({-0.5F * width, -0.5F * depth, 0.0F}, axis)};

    std::vector<Vec3> normals = {
        _RotateToMatchUpAxis({0.0F, 0.0F, 1.0F}, axis),
        _RotateToMatchUpAxis({0.0F, 0.0F, 1.0F}, axis),
        _RotateToMatchUpAxis({0.0F, 0.0F, 1.0F}, axis),
        _RotateToMatchUpAxis({0.0F, 0.0F, 1.0F}, axis)};

    std::vector<Vec2> uvs = {{0.0F, 0.0F},
                             {0.5F * width, 0.0F},
                             {0.5F * width, 0.5F * depth},
                             {0.0F, 0.5F * depth}};

    std::vector<uint32_t> indices = {0, 1, 2, 0, 2, 3};

    return std::make_unique<Geometry>(positions, normals, uvs, indices);
}

auto CreateBox(float width, float depth, float height) -> Geometry::uptr {
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;
    std::vector<uint32_t> indices;

    std::vector<Vec3> normals_source = {
        {0.0F, 0.0F, 1.0F},  {0.0F, 0.0F, -1.0F}, {0.0F, 1.0F, 0.0F},
        {0.0F, -1.0F, 0.0F}, {1.0F, 0.0F, 0.0F},  {-1.0F, 0.0F, 0.0F}};

    positions.reserve(4 * normals_source.size());
    normals.reserve(4 * normals_source.size());
    uvs.reserve(4 * normals_source.size());
    indices.reserve(6 * normals_source.size());

    auto scale = Vec3(0.5F * width, 0.5F * depth, 0.5F * height);

    // For each face, compute the vertices that form the face perpendicular to
    // that normal
    for (const auto& n : normals_source) {
        // Add the indices accordingly
        indices.push_back(static_cast<uint32_t>(positions.size()));
        indices.push_back(static_cast<uint32_t>(positions.size() + 1));
        indices.push_back(static_cast<uint32_t>(positions.size() + 2));
        indices.push_back(static_cast<uint32_t>(positions.size()));
        indices.push_back(static_cast<uint32_t>(positions.size() + 2));
        indices.push_back(static_cast<uint32_t>(positions.size() + 3));

        // Form a perpendicular right hand system based on the current normal
        Vec3 s1 = {n.y(), n.z(), n.x()};
        Vec3 s2 = math::cross(n, s1);

        // Generate each vertex of each face according to these vectors
        auto v0 = (n - s1 - s2) * (scale);
        auto v1 = (n + s1 - s2) * (scale);
        auto v2 = (n + s1 + s2) * (scale);
        auto v3 = (n - s1 + s2) * (scale);

        positions.push_back(v0);
        positions.push_back(v1);
        positions.push_back(v2);
        positions.push_back(v3);

        uvs.emplace_back(0.0F, 0.0F);
        uvs.emplace_back(1.0F, 0.0F);
        uvs.emplace_back(1.0F, 1.0F);
        uvs.emplace_back(0.0F, 1.0F);

        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);
    }

    return std::make_unique<Geometry>(positions, normals, uvs, indices);
}

auto _RotateToMatchUpAxis(const Vec3& vec, const eAxis& axis) -> Vec3 {
    switch (axis) {
        case eAxis::AXIS_X:
            return Vec3(vec.y(), vec.z(), vec.x());
        case eAxis::AXIS_Y:
            return Vec3(vec.z(), vec.x(), vec.y());
        case eAxis::AXIS_Z:
            return Vec3(vec.x(), vec.y(), vec.z());
    }
}

}  // namespace renderer

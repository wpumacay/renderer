#include <cstdint>
#include <vector>
#include <glad/gl.h>

#include <renderer/geometry/geometry_factory.hpp>
#include "math/common.hpp"

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

auto CreateSphere(float radius, size_t nDiv1, size_t nDiv2) -> Geometry::uptr {
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;
    std::vector<uint32_t> indices;

    const size_t N_VERTICES = (nDiv1 + 1) * (nDiv2 + 1);
    positions.reserve(N_VERTICES);
    normals.reserve(N_VERTICES);
    uvs.reserve(N_VERTICES);

    const size_t N_INDICES = 3 * (nDiv1 - 1) * nDiv2 + 3 * (nDiv1 - 1) * nDiv2;
    indices.reserve(N_INDICES);

    // Construct all vertices using spherical coordinates
    const auto PI = static_cast<float>(math::PI);
    for (size_t i = 0; i <= nDiv1; ++i) {
        for (size_t j = 0; j <= nDiv2; ++j) {
            // Compute the grid on the second and third spherical coordinates
            const auto I_GRID =
                static_cast<float>(i) / static_cast<float>(nDiv1);
            const auto J_GRID =
                static_cast<float>(j) / static_cast<float>(nDiv2);
            float theta = 2.0F * PI * I_GRID;
            float phi = PI * (J_GRID - 0.5F);

            float cos_theta = std::cos(theta);
            float sin_theta = std::sin(theta);
            float cos_phi = std::cos(phi);
            float sin_phi = std::sin(phi);

            // clang-format off
            Vec3 vertex(radius * cos_theta * cos_phi,
                        radius * sin_theta * cos_phi,
                        radius * sin_phi);
            // clang-format on
            Vec3 normal = math::normalize(vertex);

            positions.push_back(vertex);
            normals.push_back(normal);

            // UV calculations adapted from ThreeJS repo [1]
            float v_offset = 0.0F;
            v_offset = (j == 0) ? (0.5F / static_cast<float>(nDiv1)) : v_offset;
            v_offset =
                (j == nDiv2) ? (-0.5F / static_cast<float>(nDiv1)) : v_offset;

            uvs.emplace_back(I_GRID, J_GRID + v_offset);
        }
    }

    // Compute the indices for this tessellation
    for (size_t i = 0; i < nDiv1; ++i) {
        for (size_t j = 0; j < nDiv2; ++j) {
            const auto IDX_0 =
                static_cast<uint32_t>((i + 0) * (nDiv2 + 1) + (j + 0));
            const auto IDX_1 =
                static_cast<uint32_t>((i + 1) * (nDiv2 + 1) + (j + 0));
            const auto IDX_2 =
                static_cast<uint32_t>((i + 1) * (nDiv2 + 1) + (j + 1));
            const auto IDX_3 =
                static_cast<uint32_t>((i + 0) * (nDiv2 + 1) + (j + 1));

            if (j != 0) {
                indices.push_back(IDX_0);
                indices.push_back(IDX_1);
                indices.push_back(IDX_2);
            }

            if (j != (nDiv2 - 1)) {
                indices.push_back(IDX_0);
                indices.push_back(IDX_2);
                indices.push_back(IDX_3);
            }
        }
    }

    return std::make_unique<Geometry>(positions, normals, uvs, indices);
}

auto CreateEllipsoid(float radius_x, float radius_y, float radius_z,
                     size_t nDiv1, size_t nDiv2) -> Geometry::uptr {
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;
    std::vector<uint32_t> indices;

    const size_t N_VERTICES = (nDiv1 + 1) * (nDiv2 + 1);
    positions.reserve(N_VERTICES);
    normals.reserve(N_VERTICES);
    uvs.reserve(N_VERTICES);

    const size_t N_INDICES = 3 * (nDiv1 - 1) * nDiv2 + 3 * (nDiv1 - 1) * nDiv2;
    indices.reserve(N_INDICES);

    // Construct all vertices using spherical coordinates
    const auto PI = static_cast<float>(math::PI);
    for (size_t i = 0; i <= nDiv1; ++i) {
        for (size_t j = 0; j <= nDiv2; ++j) {
            // Compute the grid on the second and third spherical coordinates
            const auto I_GRID =
                static_cast<float>(i) / static_cast<float>(nDiv1);
            const auto J_GRID =
                static_cast<float>(j) / static_cast<float>(nDiv2);
            float theta = 2.0F * PI * I_GRID;
            float phi = PI * (J_GRID - 0.5F);

            float cos_theta = std::cos(theta);
            float sin_theta = std::sin(theta);
            float cos_phi = std::cos(phi);
            float sin_phi = std::sin(phi);

            // clang-format off
            Vec3 vertex(radius_x * cos_theta * cos_phi,
                        radius_y * sin_theta * cos_phi,
                        radius_z * sin_phi);
            // clang-format on
            Vec3 normal = math::normalize(vertex);

            positions.push_back(vertex);
            normals.push_back(normal);

            // UV calculations adapted from ThreeJS repo [1]
            float v_offset = 0.0F;
            v_offset = (j == 0) ? (0.5F / static_cast<float>(nDiv1)) : v_offset;
            v_offset =
                (j == nDiv2) ? (-0.5F / static_cast<float>(nDiv1)) : v_offset;

            uvs.emplace_back(I_GRID, J_GRID + v_offset);
        }
    }

    // Compute the indices for this tessellation
    for (size_t i = 0; i < nDiv1; ++i) {
        for (size_t j = 0; j < nDiv2; ++j) {
            const auto IDX_0 =
                static_cast<uint32_t>((i + 0) * (nDiv2 + 1) + (j + 0));
            const auto IDX_1 =
                static_cast<uint32_t>((i + 1) * (nDiv2 + 1) + (j + 0));
            const auto IDX_2 =
                static_cast<uint32_t>((i + 1) * (nDiv2 + 1) + (j + 1));
            const auto IDX_3 =
                static_cast<uint32_t>((i + 0) * (nDiv2 + 1) + (j + 1));

            if (j != 0) {
                indices.push_back(IDX_0);
                indices.push_back(IDX_1);
                indices.push_back(IDX_2);
            }

            if (j != (nDiv2 - 1)) {
                indices.push_back(IDX_0);
                indices.push_back(IDX_2);
                indices.push_back(IDX_3);
            }
        }
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

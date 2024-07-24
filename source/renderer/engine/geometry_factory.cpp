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

auto CreateCylinder(float radius, float height, const eAxis& axis, size_t nDiv)
    -> Geometry::uptr {
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;
    std::vector<uint32_t> indices;

    const size_t N_VERTICES = nDiv + 4 * nDiv + nDiv;
    positions.reserve(N_VERTICES);
    normals.reserve(N_VERTICES);
    uvs.reserve(N_VERTICES);

    const size_t N_INDICES = 3 * (nDiv - 2) + 6 * nDiv + 3 * (nDiv - 2);
    indices.reserve(N_INDICES);

    //--------------------------------------------------------------------------
    // Compute the points of the cross-section of the cylinder (we're assumming
    // that the axis of the cylinder is aligned with the z-axis). We'll use
    // these points as the basis for our geometry calculations later
    std::vector<Vec3> section_xy;
    section_xy.reserve(nDiv + 1);
    for (size_t i = 0; i <= nDiv; ++i) {
        const auto I_GRID = static_cast<float>(i) / static_cast<float>(nDiv);
        const auto THETA = 2.0F * static_cast<float>(math::PI) * I_GRID;
        section_xy.emplace_back(radius * std::cos(THETA),
                                radius * std::sin(THETA), 0.0F);
    }

    //--------------------------------------------------------------------------
    // Construct the geometry of the cylinder
    size_t base_idx = 0;

    //////// Up Base ////////
    for (size_t i = 0; i < nDiv; ++i) {
        positions.push_back(_RotateToMatchUpAxis(
            section_xy[i] + Vec3(0.0F, 0.0F, 0.5F * height), axis));
        normals.push_back(_RotateToMatchUpAxis({0.0F, 0.0F, 1.0F}, axis));
        uvs.emplace_back(0.5F * (1.0F + section_xy[i].x() / radius),
                         0.5F * (1.0F + section_xy[i].y() / radius));
    }

    for (size_t i = 1; i <= nDiv - 2; ++i) {
        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + i));
        indices.push_back(static_cast<uint32_t>(base_idx + i + 1));
    }

    base_idx += positions.size();

    //////// Body Surface ////////
    for (size_t i = 0; i < nDiv; i++) {
        // Quad vertices
        auto p0 = section_xy[i + 0] + Vec3(0.0F, 0.0F, -0.5F * height);
        auto p1 = section_xy[i + 1] + Vec3(0.0F, 0.0F, -0.5F * height);
        auto p2 = section_xy[i + 1] + Vec3(0.0F, 0.0F, 0.5F * height);
        auto p3 = section_xy[i + 0] + Vec3(0.0F, 0.0F, 0.5F * height);

        positions.push_back(_RotateToMatchUpAxis(p0, axis));
        positions.push_back(_RotateToMatchUpAxis(p1, axis));
        positions.push_back(_RotateToMatchUpAxis(p2, axis));
        positions.push_back(_RotateToMatchUpAxis(p3, axis));

        uvs.emplace_back(static_cast<float>(i + 0) / static_cast<float>(nDiv),
                         0.0F);
        uvs.emplace_back(static_cast<float>(i + 1) / static_cast<float>(nDiv),
                         0.0F);
        uvs.emplace_back(static_cast<float>(i + 1) / static_cast<float>(nDiv),
                         1.0F);
        uvs.emplace_back(static_cast<float>(i + 0) / static_cast<float>(nDiv),
                         1.0F);

        auto n_quad1 =
            _RotateToMatchUpAxis(math::normalize(section_xy[i + 0]), axis);
        auto n_quad2 =
            _RotateToMatchUpAxis(math::normalize(section_xy[i + 1]), axis);

        normals.push_back(n_quad1);
        normals.push_back(n_quad2);
        normals.push_back(n_quad2);
        normals.push_back(n_quad1);

        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + 1));
        indices.push_back(static_cast<uint32_t>(base_idx + 2));

        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + 2));
        indices.push_back(static_cast<uint32_t>(base_idx + 3));

        base_idx += 4;
    }

    //////// Down Base ////////
    for (size_t i = 0; i < nDiv; ++i) {
        positions.push_back(_RotateToMatchUpAxis(
            section_xy[i] + Vec3(0.0F, 0.0F, -0.5F * height), axis));
        normals.push_back(_RotateToMatchUpAxis({0.0F, 0.0F, -1.0F}, axis));
        uvs.emplace_back(0.5F * (1.0F + section_xy[i].x() / radius),
                         0.5F * (1.0F + section_xy[i].y() / radius));
    }

    for (size_t i = 1; i <= nDiv - 2; ++i) {
        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + i + 1));
        indices.push_back(static_cast<uint32_t>(base_idx + i));
    }

    return std::make_unique<Geometry>(positions, normals, uvs, indices);
}

// NOLINTNEXTLINE
auto CreateCapsule(float radius, float height, const eAxis& axis, size_t nDiv1,
                   size_t nDiv2) -> Geometry::uptr {
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;
    std::vector<uint32_t> indices;

    // TODO(wilbert): both caps generation produces extra vertices (whole
    // spheres), which shouldn't happen as these are hidden vertices

    /* Capsule format
     *
     *   |-- radius --|-- height --|-- radius --|
     */

    //--------------------------------------------------------------------------
    // Build the top cap of the capsule
    size_t base_idx = 0;
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
            // apply offset to up position
            vertex = vertex +
                     _RotateToMatchUpAxis({0.0F, 0.0F, 0.5F * height}, axis);

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

            indices.push_back(static_cast<uint32_t>(base_idx + IDX_0));
            indices.push_back(static_cast<uint32_t>(base_idx + IDX_1));
            indices.push_back(static_cast<uint32_t>(base_idx + IDX_2));

            if (j != (nDiv2 - 1)) {
                indices.push_back(static_cast<uint32_t>(base_idx + IDX_0));
                indices.push_back(static_cast<uint32_t>(base_idx + IDX_2));
                indices.push_back(static_cast<uint32_t>(base_idx + IDX_3));
            }
        }
    }
    base_idx += positions.size();

    //--------------------------------------------------------------------------
    // Build the cylindrical body of the capsule

    std::vector<Vec3> section_xy;
    section_xy.reserve(nDiv1 + 1);
    for (size_t i = 0; i <= nDiv1; ++i) {
        const auto I_GRID = static_cast<float>(i) / static_cast<float>(nDiv1);
        const auto THETA = 2.0F * PI * I_GRID;
        section_xy.emplace_back(radius * std::cos(THETA),
                                radius * std::sin(THETA), 0.0F);
    }

    for (size_t i = 0; i < nDiv1; i++) {
        // Quad vertices
        auto p0 = section_xy[i + 0] + Vec3(0.0F, 0.0F, -0.5F * height);
        auto p1 = section_xy[i + 1] + Vec3(0.0F, 0.0F, -0.5F * height);
        auto p2 = section_xy[i + 1] + Vec3(0.0F, 0.0F, 0.5F * height);
        auto p3 = section_xy[i + 0] + Vec3(0.0F, 0.0F, 0.5F * height);

        positions.push_back(_RotateToMatchUpAxis(p0, axis));
        positions.push_back(_RotateToMatchUpAxis(p1, axis));
        positions.push_back(_RotateToMatchUpAxis(p2, axis));
        positions.push_back(_RotateToMatchUpAxis(p3, axis));

        uvs.emplace_back(static_cast<float>(i + 0) / static_cast<float>(nDiv1),
                         0.0F);
        uvs.emplace_back(static_cast<float>(i + 1) / static_cast<float>(nDiv1),
                         0.0F);
        uvs.emplace_back(static_cast<float>(i + 1) / static_cast<float>(nDiv1),
                         1.0F);
        uvs.emplace_back(static_cast<float>(i + 0) / static_cast<float>(nDiv1),
                         1.0F);

        auto n_quad1 =
            _RotateToMatchUpAxis(math::normalize(section_xy[i + 0]), axis);
        auto n_quad2 =
            _RotateToMatchUpAxis(math::normalize(section_xy[i + 1]), axis);

        normals.push_back(n_quad1);
        normals.push_back(n_quad2);
        normals.push_back(n_quad2);
        normals.push_back(n_quad1);

        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + 1));
        indices.push_back(static_cast<uint32_t>(base_idx + 2));

        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + 2));
        indices.push_back(static_cast<uint32_t>(base_idx + 3));

        base_idx += 4;
    }

    //--------------------------------------------------------------------------
    // Build the bottom cap of the capsule

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
            // apply offset to up position
            vertex = vertex -
                     _RotateToMatchUpAxis({0.0F, 0.0F, 0.5F * height}, axis);

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

            indices.push_back(static_cast<uint32_t>(base_idx + IDX_0));
            indices.push_back(static_cast<uint32_t>(base_idx + IDX_2));
            indices.push_back(static_cast<uint32_t>(base_idx + IDX_1));

            if (j != (nDiv2 - 1)) {
                indices.push_back(static_cast<uint32_t>(base_idx + IDX_0));
                indices.push_back(static_cast<uint32_t>(base_idx + IDX_3));
                indices.push_back(static_cast<uint32_t>(base_idx + IDX_2));
            }
        }
    }

    return std::make_unique<Geometry>(positions, normals, uvs, indices);
}

auto CreateArrow(float length, const eAxis& axis) -> Geometry::uptr {
    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vec2> uvs;
    std::vector<uint32_t> indices;

    const size_t N_DIV = 10;
    const float RADIUS_CYLINDER = 0.05F * length;
    const float LENGTH_CYLINDER = 0.8F * length;
    const float RADIUS_CONE = 0.075F * length;
    const float LENGTH_CONE = 0.2F * length;

    const size_t N_VERTICES = N_DIV + 4 * N_DIV + 3 * N_DIV + 4 * N_DIV;
    positions.reserve(N_VERTICES);
    normals.reserve(N_VERTICES);
    uvs.reserve(N_VERTICES);

    const size_t N_INDICES =
        3 * (N_DIV - 2) + 6 * N_DIV + 3 * N_DIV + 6 * N_DIV;
    indices.reserve(N_INDICES);

    //--------------------------------------------------------------------------
    // Tessellate the cylindrical part

    // Compute the cross section vertices
    std::vector<Vec3> section_xy;
    section_xy.reserve(N_DIV + 1);
    const auto PI = static_cast<float>(math::PI);
    for (size_t i = 0; i <= N_DIV; ++i) {
        const auto I_GRID = static_cast<float>(i) / static_cast<float>(N_DIV);
        const auto THETA = 2.0F * PI * I_GRID;
        section_xy.emplace_back(RADIUS_CYLINDER * std::cos(THETA),
                                RADIUS_CYLINDER * std::sin(THETA), 0.0F);
    }

    // Build the down base of the cylinder
    size_t base_idx = 0;
    for (size_t i = 0; i < N_DIV; ++i) {
        positions.push_back(_RotateToMatchUpAxis(section_xy[i], axis));
        normals.push_back(_RotateToMatchUpAxis({0.0F, 0.0F, -1.0F}, axis));
        uvs.emplace_back(0.5F * (1.0F + section_xy[i].x() / RADIUS_CYLINDER),
                         0.5F * (1.0F + section_xy[i].y() / RADIUS_CYLINDER));
    }
    for (size_t i = 1; i <= N_DIV - 2; ++i) {
        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + i + 1));
        indices.push_back(static_cast<uint32_t>(base_idx + i));
    }
    base_idx += positions.size();

    // Build the cylindrical surface
    for (size_t i = 0; i < N_DIV; ++i) {
        auto p0 = section_xy[i + 0] + Vec3(0.0F, 0.0F, 0.0F);
        auto p1 = section_xy[i + 1] + Vec3(0.0F, 0.0F, 0.0F);
        auto p2 = section_xy[i + 1] + Vec3(0.0F, 0.0F, LENGTH_CYLINDER);
        auto p3 = section_xy[i + 0] + Vec3(0.0F, 0.0F, LENGTH_CYLINDER);

        positions.push_back(_RotateToMatchUpAxis(p0, axis));
        positions.push_back(_RotateToMatchUpAxis(p1, axis));
        positions.push_back(_RotateToMatchUpAxis(p2, axis));
        positions.push_back(_RotateToMatchUpAxis(p3, axis));

        uvs.emplace_back(static_cast<float>(i + 0) / static_cast<float>(N_DIV),
                         0.0F);
        uvs.emplace_back(static_cast<float>(i + 1) / static_cast<float>(N_DIV),
                         0.0F);
        uvs.emplace_back(static_cast<float>(i + 1) / static_cast<float>(N_DIV),
                         1.0F);
        uvs.emplace_back(static_cast<float>(i + 0) / static_cast<float>(N_DIV),
                         1.0F);

        auto n_quad1 =
            _RotateToMatchUpAxis(math::normalize(section_xy[i + 0]), axis);
        auto n_quad2 =
            _RotateToMatchUpAxis(math::normalize(section_xy[i + 1]), axis);

        normals.push_back(n_quad1);
        normals.push_back(n_quad2);
        normals.push_back(n_quad2);
        normals.push_back(n_quad1);

        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + 1));
        indices.push_back(static_cast<uint32_t>(base_idx + 2));

        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + 2));
        indices.push_back(static_cast<uint32_t>(base_idx + 3));

        base_idx += 4;
    }

    //--------------------------------------------------------------------------
    // Tessellate the cone part

    // Compute the base points
    std::vector<Vec3> section_xy_cone_out;
    std::vector<Vec3> section_xy_cone_in;
    for (size_t i = 0; i < N_DIV; ++i) {
        const auto I_GRID = static_cast<float>(i) / static_cast<float>(N_DIV);
        const auto THETA = 2.0F * PI * I_GRID;
        const auto COS_THETA = std::cos(THETA);
        const auto SIN_THETA = std::sin(THETA);
        section_xy_cone_out.emplace_back(RADIUS_CONE * COS_THETA,
                                         RADIUS_CONE * SIN_THETA, 0.0F);
        section_xy_cone_in.emplace_back(RADIUS_CYLINDER * COS_THETA,
                                        RADIUS_CYLINDER * SIN_THETA, 0.0F);
    }

    // Build cone surface (tessellate using strips of triangles)
    for (size_t i = 0; i < section_xy_cone_out.size(); ++i) {
        indices.push_back(static_cast<uint32_t>(positions.size()));
        indices.push_back(static_cast<uint32_t>(positions.size() + 1));
        indices.push_back(static_cast<uint32_t>(positions.size() + 2));

        auto p0 = _RotateToMatchUpAxis(
            section_xy_cone_out[(i + 0) % section_xy_cone_out.size()] +
                Vec3(0.0F, 0.0F, LENGTH_CYLINDER),
            axis);
        auto p1 = _RotateToMatchUpAxis(
            section_xy_cone_out[(i + 1) % section_xy_cone_out.size()] +
                Vec3(0.0F, 0.0F, LENGTH_CYLINDER),
            axis);
        auto p2 = _RotateToMatchUpAxis(
            {0.0F, 0.0F, LENGTH_CYLINDER + LENGTH_CONE}, axis);

        positions.push_back(p0);
        positions.push_back(p1);
        positions.push_back(p2);

        auto n_quad0 = _RotateToMatchUpAxis(
            section_xy_cone_out[(i + 0) % section_xy_cone_out.size()], axis);
        auto n_quad1 = _RotateToMatchUpAxis(
            section_xy_cone_out[(i + 1) % section_xy_cone_out.size()], axis);
        auto n_quad2 = _RotateToMatchUpAxis({0.0F, 0.0F, 1.0F}, axis);

        normals.push_back(math::normalize(n_quad0));
        normals.push_back(math::normalize(n_quad1));
        normals.push_back(math::normalize(n_quad2));

        uvs.emplace_back((static_cast<float>(i) + 0.0F) /
                             static_cast<float>(section_xy_cone_out.size()),
                         1.0F);
        uvs.emplace_back((static_cast<float>(i) + 1.0F) /
                             static_cast<float>(section_xy_cone_out.size()),
                         1.0F);
        uvs.emplace_back((static_cast<float>(i) + 0.0F) /
                             static_cast<float>(section_xy_cone_out.size()),
                         0.0F);
    }

    // Build bottom base (tessellated using strips of rings)
    base_idx = positions.size();

    for (size_t i = 0; i < section_xy_cone_out.size(); ++i) {
        auto p0 = _RotateToMatchUpAxis(
            section_xy_cone_out[(i + 0) % section_xy_cone_out.size()] +
                Vec3(0.0F, 0.0F, LENGTH_CYLINDER),
            axis);
        auto p1 = _RotateToMatchUpAxis(
            section_xy_cone_in[(i + 0) % section_xy_cone_out.size()] +
                Vec3(0.0F, 0.0F, LENGTH_CYLINDER),
            axis);
        auto p2 = _RotateToMatchUpAxis(
            section_xy_cone_in[(i + 1) % section_xy_cone_out.size()] +
                Vec3(0.0F, 0.0F, LENGTH_CYLINDER),
            axis);
        auto p3 = _RotateToMatchUpAxis(
            section_xy_cone_out[(i + 1) % section_xy_cone_out.size()] +
                Vec3(0.0F, 0.0F, LENGTH_CYLINDER),
            axis);

        positions.push_back(p0);
        positions.push_back(p1);
        positions.push_back(p2);
        positions.push_back(p3);

        auto n = _RotateToMatchUpAxis({0.0F, 0.0F, -1.05F}, axis);

        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);
        normals.push_back(n);

        const auto IDX = i;
        const auto IDX_NEXT = (i + 1) % section_xy_cone_in.size();

        uvs.emplace_back(
            0.5F * (1.0F + section_xy_cone_out[IDX].x() / RADIUS_CONE),
            0.5F * (1.0F + section_xy_cone_out[IDX].y() / RADIUS_CONE));
        uvs.emplace_back(
            0.5F * (1.0F + section_xy_cone_in[IDX].x() / RADIUS_CONE),
            0.5F * (1.0F + section_xy_cone_in[IDX].y() / RADIUS_CONE));
        uvs.emplace_back(
            0.5F * (1.0F + section_xy_cone_in[IDX_NEXT].x() / RADIUS_CONE),
            0.5F * (1.0F + section_xy_cone_in[IDX_NEXT].y() / RADIUS_CONE));
        uvs.emplace_back(
            0.5F * (1.0F + section_xy_cone_out[IDX_NEXT].x() / RADIUS_CONE),
            0.5F * (1.0F + section_xy_cone_out[IDX_NEXT].y() / RADIUS_CONE));

        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + 2));
        indices.push_back(static_cast<uint32_t>(base_idx + 1));

        indices.push_back(static_cast<uint32_t>(base_idx));
        indices.push_back(static_cast<uint32_t>(base_idx + 3));
        indices.push_back(static_cast<uint32_t>(base_idx + 2));

        base_idx += 4;
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
    return Vec3(vec.x(), vec.y(), vec.z());
}

}  // namespace renderer

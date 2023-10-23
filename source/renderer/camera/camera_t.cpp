#include <renderer/camera/camera_t.hpp>

#include <cmath>
#include <utils/logging.hpp>

namespace renderer {

auto ToString(const eProjectionType& proj_type) -> std::string {
    switch (proj_type) {
        case eProjectionType::PERSPECTIVE:
            return "perspective";
        case eProjectionType::ORTHOGRAPHIC:
            return "orthographic";
    }
    return "undefined";
}

Camera::Camera(const Vec3& position, const Vec3& target, const Vec3& world_up,
               const ProjectionData& proj_data)
    : m_Position(position),
      m_Target(target),
      m_WorldUp(world_up),
      m_ProjData(proj_data) {
    ComputeBasisVectors();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

auto Camera::ComputeBasisVectors() -> void {
    // Adapted the look-at function from [0]. Handles corners cases in which the
    // front vector aligns with the world-up vector (just use the world axes)

    // Cache front vector, in case the update cant be applied
    auto old_front = m_Front;
    m_Front = math::normalize<float>(m_Position - m_Target);
    const bool FRONT_ALIGNS_WORLDUP =
        (m_Front == m_WorldUp) || (m_Front == -m_WorldUp);

    if (FRONT_ALIGNS_WORLDUP) {
        // Keep cached value of the front vector
        m_Front = old_front;
        return;
    }

    m_Right = math::normalize(math::cross<float>(m_WorldUp, m_Front));
    m_Up = math::normalize(math::cross<float>(m_Front, m_Right));

    // Get the orientation from the basis vectors (rot-matrix)
    Mat3 rotmat(m_Right, m_Up, m_Front);
    m_Orientation = Quat(rotmat);
}

auto Camera::ComputeBasisVectorsFromOrientation() -> void {
    auto rot_matrix = Mat3(m_Orientation);
    m_Right = math::normalize(rot_matrix[0]);
    m_Up = math::normalize(rot_matrix[1]);
    m_Front = math::normalize(rot_matrix[2]);
    // Recompute the target point, making sure we are at the same distance we
    // were previous to this update
    auto length = math::norm(m_Position - m_Target);
    m_Target = m_Position - static_cast<double>(length) * m_Front;
}

auto Camera::UpdateViewMatrix() -> void {
    // The view matrix is the inverse of the camera pose in world space. We
    // could use the inverse directly, but we have the analytical inverse
    // of this matrix (to avoid extra unnecessary computations)
    //
    //  |                 |   | --- right^T ---  -<right, p>   |
    //  |    R^T   -R^T p |   | ---   up^T  ---  -<up, p>      |
    //  |                 | = | --- front^T ---  -<front, p>   |
    //  | 0   0   0    1  |   |  0     0    0          1       |

    m_ViewMatrix(0, 0) = m_Right.x();
    m_ViewMatrix(1, 0) = m_Up.x();
    m_ViewMatrix(2, 0) = m_Front.x();
    m_ViewMatrix(3, 0) = 0.0F;

    m_ViewMatrix(0, 1) = m_Right.y();
    m_ViewMatrix(1, 1) = m_Up.y();
    m_ViewMatrix(2, 1) = m_Front.y();
    m_ViewMatrix(3, 1) = 0.0F;

    m_ViewMatrix(0, 2) = m_Right.z();
    m_ViewMatrix(1, 2) = m_Up.z();
    m_ViewMatrix(2, 2) = m_Front.z();
    m_ViewMatrix(3, 2) = 0.0F;

    m_ViewMatrix(0, 3) = -math::dot<float>(m_Right, m_Position);
    m_ViewMatrix(1, 3) = -math::dot<float>(m_Up, m_Position);
    m_ViewMatrix(2, 3) = -math::dot<float>(m_Front, m_Position);
    m_ViewMatrix(3, 3) = 1.0F;
}

auto Camera::UpdateProjectionMatrix() -> void {
    // Make sure zoom is within a valid range [1e-3, +Inf]
    if (std::abs(m_Zoom) < 1e-3F) {
        LOG_CORE_WARN(
            "Camera::UpdateProjectionMatrix >>> zoom={0} value is less "
            "than minimum accepted value 1e-3. Won't update projection.",
            m_Zoom);
        return;
    }

    switch (m_ProjData.projection) {
        case eProjectionType::PERSPECTIVE: {
            // Based on ThreeJS implementation [2]
            auto top = m_ProjData.near *
                       std::tan((m_ProjData.fov * 0.5F) * PI / 180.0F) / m_Zoom;
            auto height = 2.0F * top;
            auto width = m_ProjData.aspect * height;
            auto left = -0.5F * width;

            m_ProjMatrix =
                Mat4::Perspective(left, left + width, top, top - height,
                                  m_ProjData.near, m_ProjData.far);
            break;
        }
        case eProjectionType::ORTHOGRAPHIC: {
            m_ProjMatrix = Mat4::Ortho(m_ProjData.width / m_Zoom,
                                       m_ProjData.height / m_Zoom,
                                       m_ProjData.near, m_ProjData.far);
        }
    }
}

auto Camera::SetOrientation(const Quat& quat) -> void {
    m_Orientation = quat;
    ComputeBasisVectorsFromOrientation();
    UpdateViewMatrix();
}

auto Camera::SetPosition(const Vec3& pos) -> void {
    m_Position = pos;
    ComputeBasisVectors();
    UpdateViewMatrix();
}

auto Camera::SetPositionNoUpdate(const Vec3& pos) -> void { m_Position = pos; }

auto Camera::SetTarget(const Vec3& target) -> void {
    m_Target = target;
    ComputeBasisVectors();
    UpdateViewMatrix();
}

auto Camera::SetTargetNoUpdate(const Vec3& target) -> void {
    m_Target = target;
}

auto Camera::SetProjectionData(const ProjectionData& proj_data) -> void {
    m_ProjData = proj_data;
    UpdateProjectionMatrix();
}

auto Camera::SetProjectionType(eProjectionType proj_type) -> void {
    if (m_ProjData.projection == proj_type) {
        return;
    }
    m_ProjData.projection = proj_type;
    UpdateProjectionMatrix();
}

auto Camera::SetFOV(float fov) -> void {
    if (m_ProjData.projection == eProjectionType::PERSPECTIVE) {
        m_ProjData.fov = fov;
        UpdateProjectionMatrix();
    }
}

auto Camera::SetAspectRatio(float aspect) -> void {
    if (m_ProjData.projection == eProjectionType::PERSPECTIVE) {
        m_ProjData.aspect = aspect;
        UpdateProjectionMatrix();
    }
}

auto Camera::SetWidth(float width) -> void {
    if (m_ProjData.projection == eProjectionType::ORTHOGRAPHIC) {
        m_ProjData.width = width;
        UpdateProjectionMatrix();
    }
}

auto Camera::SetHeight(float height) -> void {
    if (m_ProjData.projection == eProjectionType::ORTHOGRAPHIC) {
        m_ProjData.height = height;
        UpdateProjectionMatrix();
    }
}

auto Camera::SetZNear(float near) -> void {
    m_ProjData.near = near;
    UpdateProjectionMatrix();
}

auto Camera::SetZFar(float far) -> void {
    m_ProjData.far = far;
    UpdateProjectionMatrix();
}

auto Camera::SetZoom(float zoom) -> void {
    m_Zoom = zoom;
    UpdateProjectionMatrix();
}

}  // namespace renderer

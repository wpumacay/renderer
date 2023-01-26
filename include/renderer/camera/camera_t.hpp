#pragma once

#include <string>

#include <renderer/common.hpp>
#include <utils/common.hpp>

namespace renderer {

/// Available projection types
enum class eProjectionType {
    PERSPECTIVE,  //< Pin-hole perspective-like camera
    ORTHOGRAPHIC  //< Isometric-like camera
};

/// Returns the string representation of the given projection enumerator
auto ToString(const eProjectionType& proj_type) -> std::string;

/// Parameters required to define the projection appropriately
struct ProjectionData {
    /// Type of projection used by the camera
    eProjectionType projection = eProjectionType::PERSPECTIVE;
    /// Field of view of the camera (for perspective cameras only)
    float fov = 45.0F;
    /// Aspect ratio of the view-frustum (for perspective cameras only)
    float aspect = 1.0F;
    /// Width of view box of the camera (for orthographic cameras only)
    float width = 2.0F;
    /// Height of the view box of the camera (for orthographics cameras only)
    float height = 2.0F;
    /// Distance to the closest plane of the view-volume
    float near = 0.1F;
    /// Distance to the furthest plane of the view-volume
    float far = 1000.0F;
};

/// Camera type
class Camera {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Camera)

    DEFINE_SMART_POINTERS(Camera)

 public:
    /// Creates a camera with the given configuration
    explicit Camera(const Vec3& position, const Vec3& target,
                    const Vec3& world_up = {0.0, 0.0, 1.0},
                    const ProjectionData& proj_data = ProjectionData());

    /// Frees the resources allocated by this camera
    ~Camera() = default;

    /// Sets the position of the camera in world space, and update the state
    auto SetPosition(const Vec3& pos) -> void;

    /// Sets the orientation of the camera's reference frame in world space
    auto SetOrientation(const Quat& quat) -> void;

    /// Sets the target of the camera in world space, and update the state
    auto SetTarget(const Vec3& target) -> void;

    /// Sets the camera's position, but doesn't update the internal camera state
    auto SetPositionNoUpdate(const Vec3& pos) -> void;

    /// Sets the camera's target, but doesn't update the internal camera state
    auto SetTargetNoUpdate(const Vec3& target) -> void;

    /// Sets the configuration for the projection of this camera
    auto SetProjectionData(const ProjectionData& proj_data) -> void;

    /// Changes to the given projection type, and updates internal proj. matrix
    auto SetProjectionType(eProjectionType proj_type) -> void;

    /// For perspective cameras, it sets the field of view (in degrees)
    auto SetFOV(float fov) -> void;

    /// For perspective cameras, it sets the aspect ratio
    auto SetAspectRatio(float aspect) -> void;

    /// For orthographic cameras, it sets the width of the view box
    auto SetWidth(float width) -> void;

    /// For orthographic cameras, it sets the height of the view box
    auto SetHeight(float height) -> void;

    /// Sets the distance to the near plane of the view volume
    auto SetZNear(float near) -> void;

    /// Sets the distance to the far plane of the view volume
    auto SetZFar(float far) -> void;

    /// Sets the zoom and updates the internal state of the camera
    auto SetZoom(float zoom) -> void;

    /// Updates the view matrix from the current state of the camera
    auto UpdateViewMatrix() -> void;

    /// Updates the projection matrix from the current state of the camera
    auto UpdateProjectionMatrix() -> void;

    /// Computes the basis vectors from the current camera state
    auto ComputeBasisVectors() -> void;

    /// Computes the basis vectors from the current orientation
    auto ComputeBasisVectorsFromOrientation() -> void;

 public:
    /// Returns an unmutable reference to the position of this camera
    auto position() const -> const Vec3& { return m_Position; }

    /// Returns an unmutable reference to the orientation of this camera
    auto orientation() const -> const Quat& { return m_Orientation; }

    /// Returns an unmutable reference to the target of this camera
    auto target() const -> const Vec3& { return m_Target; }

    /// Returns an unmutable reference to the projection data of this camera
    auto proj_data() const -> const ProjectionData& { return m_ProjData; }

    /// Returns the current zoom value for this camera
    auto zoom() const -> float { return m_Zoom; }

    /// Returns an unmutable const reference to the internal view matrix
    auto view_matrix() const -> const Mat4& { return m_ViewMatrix; }

    /// Returns an unmutable const reference to the internal projection matrix
    auto proj_matrix() const -> const Mat4& { return m_ProjMatrix; }

    /// Returns the front vector (direction of Z+-axis of the camera frame)
    auto front() const -> const Vec3& { return m_Front; }

    /// Returns the up vector (direction of Y+ axis of the camera frame)
    auto up() const -> const Vec3& { return m_Up; }

    /// Returns the right vector (direction of X+ axis of the camera frame)
    auto right() const -> const Vec3& { return m_Right; }

    /// Returns the reference up direction in world space
    auto world_up() const -> const Vec3& { return m_WorldUp; }

 private:
    /// Position of the camera w.r.t. the world frame
    Vec3 m_Position;
    /// Orientation of the camera w.r.t. the world frame
    Quat m_Orientation;
    /// Target point of the camera w.r.t. the world frame
    Vec3 m_Target;
    /// World-up vector used by this camera setting
    Vec3 m_WorldUp = {0.0, 0.0, 1.0};
    /// Projection data for this camera's configuration
    ProjectionData m_ProjData;
    /// Value of the zoom used for this camera
    float m_Zoom = 1.0F;

    /// Z-axis of the reference frame of this camera
    Vec3 m_Front;
    /// Y-axis of the reference frame of this camera
    Vec3 m_Up;
    /// X-axis of the reference frame of this camera
    Vec3 m_Right;

    /// Internal view matrix (to avoid recomputing at each get call)
    Mat4 m_ViewMatrix = Mat4::Identity();
    /// Internal projection matrix (to avoid recomputing at each get call)
    Mat4 m_ProjMatrix = Mat4::Identity();
};

}  // namespace renderer

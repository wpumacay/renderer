#pragma once

#include <array>
#include <vector>
#include <string>
#include <tuple>

#include <renderer/common.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/shader/program_t.hpp>
#include <renderer/core/vertex_array_t.hpp>

namespace renderer {

/// Helper class that allows to draw primitives in a immediate-like mode
class DebugDrawer {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(DebugDrawer)

    DEFINE_SMART_POINTERS(DebugDrawer)

 public:
    /// Alias for handling a line (start, end, color)
    using Line = std::tuple<Vec3, Vec3, Vec3>;

    /// Number of lines that can be drawn per batch
    static constexpr size_t LINES_BATCH_SIZE = 1024;

    /// All available primitives that this drawer can handle. These can be
    /// rendererd in two modes: solid (draws using instancing), and wireframe
    /// (draws using line-segments via repeated calls to DrawLine)
    enum Primitive {
        /// Box primitive, defined by its width, depth and height
        BOX = 0,
        /// Sphere primitive, defined by its radius
        SPHERE = 1,
        /// Cylinder primitive, defined by its height and base radius. The axis
        /// of this object is aligned to the world X-axis
        CYLINDER_X = 2,
        /// Cylinder primitive, defined by its height and base radius. The axis
        /// of this object is aligned to the world Y-axis
        CYLINDER_Y = 3,
        /// Cylinder primitive, defined by its height and base radius. The axis
        /// of this object is aligned to the world Z-axis
        CYLINDER_Z = 4,
        /// Capsule primitive, defined by its height and top|bottom caps radius.
        /// The axis of this object is aligned to the world X-axis
        CAPSULE_X = 5,
        /// Capsule primitive, defined by its height and top|bottom caps radius.
        /// The axis of this object is aligned to the world Z-axis
        CAPSULE_Y = 6,
        /// Capsule primitive, defined by its height and top|bottom caps radius.
        /// The axis of this object is aligned to the world Z-axis
        CAPSULE_Z = 7,
        /// Arrow primitive, composed of a cylindre and cone merge accordingly.
        /// The primitive is defined by its length (till the tip of the cone),
        /// and its axis is aligned with the world X-axis
        ARROW_X = 8,
        /// Arrow primitive, composed of a cylindre and cone merge accordingly.
        /// The primitive is defined by its length (till the tip of the cone),
        /// and its axis is aligned with the world Y-axis
        ARROW_Y = 9,
        /// Arrow primitive, composed of a cylindre and cone merge accordingly.
        /// The primitive is defined by its length (till the tip of the cone),
        /// and its axis is aligned with the world Z-axis
        ARROW_Z = 10,
    };

    /// Constructs a Debug Drawer and allocates its related resources
    DebugDrawer();

    /// Releases all allocated resources
    ~DebugDrawer() = default;

    /// Requests a line to be drawn with the given properties
    auto DrawLine(const Vec3& start, const Vec3& end, const Vec3& color)
        -> void;

    /// Requests a primitive to be drawn with the given properties
    /// \param[in] type The type of primitive to be rendered
    /// \param[in] size The size parameters
    auto DrawPrimitive(Primitive type, const Vec3& size, const Vec3& position,
                       const Quat& orientation) -> void;

    /// Renders all the primitives currently waiting to be drawn
    auto Render(const Camera& camera) -> void;

 private:
    auto _RenderLines(const Camera& camera) -> void;

    auto _RenderLinesBatch(size_t num_lines) -> void;

 private:
    /// Shader used for rendering in wireframe mode (using just lines)
    Program::uptr m_ProgramModeWireframe = nullptr;
    /// Shader used for rendering solid instanced primitives
    Program::uptr m_ProgramModeSolid = nullptr;
    /// VAO used for handling line drawing
    VertexArray::uptr m_LinesVAO = nullptr;
    /// Container for the positions of both points used for lines
    std::vector<Line> m_Lines;
};
}  // namespace renderer

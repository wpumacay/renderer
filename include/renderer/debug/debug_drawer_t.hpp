#pragma once

#include <array>
#include <vector>
#include <string>
#include <tuple>

#include <renderer/common.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/shader/program_t.hpp>
#include <renderer/core/vertex_array_t.hpp>

/// Number of lines that can be drawn per batch
static constexpr uint32_t LINES_BATCH_SIZE = 1024;
/// Number of vertex positions stored per line
static constexpr uint32_t POSITIONS_PER_LINE = 2;
/// Number of vertex colors stored per line
static constexpr uint32_t COLORS_PER_LINE = 2;
/// Number of floats per vertex in each line
static constexpr uint32_t FLOATS_PER_VERTEX =
    3 * (POSITIONS_PER_LINE + COLORS_PER_LINE);
/// The size of the VBO (in bytes) used to store both positions and colors
static constexpr uint32_t LINES_VBO_SIZE =
    sizeof(Vec3) * (POSITIONS_PER_LINE + COLORS_PER_LINE) * LINES_BATCH_SIZE;

namespace renderer {

/// Helper class that allows to draw primitives in a immediate-like mode
class DebugDrawer {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(DebugDrawer)

    DEFINE_SMART_POINTERS(DebugDrawer)

 public:
    /// Struct used to save the information of the stored lines. Notice that the
    /// color for both vertices of the line is the same for each line
    struct Line {
        Vec3 start;
        Vec3 end;
        Vec3 color;

        explicit Line(const Vec3& start, const Vec3& end, const Vec3& color)
            : start(start), end(end), color(color) {}
    };

    /// Container used for storing line information for render calls (per batch)
    using LinesBuffer = std::array<float, LINES_VBO_SIZE>;

    /// Container used for storing all lines information given by the user
    using LinesContainer = std::vector<Line>;

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
    /// \param[in] size The size parameters for this primitive
    /// \param[in] position The position of the primitive in world space
    /// \param[in] orientation The orientation of the primitive in world space
    auto DrawPrimitive(Primitive type, const Vec3& size, const Vec3& position,
                       const Quat& orientation) -> void;

    /// Renders all lines and primitives with the given scene state
    /// \param[in] camera The camera corresponding to the view we want to render
    auto Render(const Camera& camera) -> void;

 private:
    /// Renders all currently stored lines using the given state of the scene
    /// \param[in] camera The camera corresponding to the view we want to render
    auto _RenderLines(const Camera& camera) -> void;

    /// Updates a single line in the storage used to connect to the lines VBO
    /// \param[out] lines_buffer Storage where to updawte the line information
    /// \param[in] lines_container Vector containing all lines given by the user
    /// \param[in] batch_index The index in the batch dimension
    /// \param[in] line_index The index in the per-line dimension
    auto _UpdateLineInBuffer(LinesBuffer& lines_buffer,
                             const LinesContainer& lines_container,
                             uint32_t batch_index, uint32_t line_index) -> void;

    /// Renders a batch of lines of a given size
    /// \param[out] lines_vao The VAO used for handling GPU data for the lines
    /// \param[in] lines_number The number of lines to be rendered
    /// \param[in] lines_data A pointer to the data of the lines to be rendered
    auto _RenderLinesBatch(VertexArray& lines_vao, uint32_t lines_number,
                           const float32_t* lines_data) -> void;

 private:
    /// Shader used for rendering in wireframe mode (using just lines)
    Program::uptr m_ProgramModeWireframe = nullptr;
    /// Shader used for rendering solid instanced primitives
    Program::uptr m_ProgramModeSolid = nullptr;
    /// VAO used for handling line drawing
    VertexArray::uptr m_LinesVAO = nullptr;
    /// Container for the positions of both points used for lines
    LinesContainer m_Lines;
};
}  // namespace renderer

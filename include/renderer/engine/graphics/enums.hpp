#pragma once

#include <string>

namespace renderer {

/// Available graphics APIs
enum class eGraphicsAPI {
    OPENGL,
    VULKAN,
    DIRECTX11,
    DIRECTX12,
};

/// Retuns the string representation of the given graphics API enum
auto ToString(eGraphicsAPI api) -> std::string;

/// Available windowing backends
enum class eWindowBackend {
    /// None or dummy backend (for testing purposes only)
    TYPE_NONE,
    /// GLFW backend (used for creating a window for OpenGL in any platform)
    TYPE_GLFW,
    /// EGL backend (used for headless rendering mode)
    TYPE_EGL,
};

/// Returns the string representation of the given backend type
auto ToString(eWindowBackend type) -> std::string;

/// Available shader types
enum class eShaderType {
  VERTEX,    //< Associated to a vertex shasder
  FRAGMENT,  //< Associated to a fragment shader
  GEOMETRY,  //< Associated to a geometry shader
  COMPUTE,   //< Associated to a compute shader
};

/// Returns the string representation of the given shader type
auto ToString(eShaderType type) -> std::string;

}  // namespace renderer
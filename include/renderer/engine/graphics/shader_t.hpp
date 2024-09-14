#pragma once

#include <memory>
#include <string>
#include <cstdint>

#include <renderer/common.hpp>
#include <renderer/engine/graphics/enums.hpp>
#include <renderer/engine/graphics/shader_adapter_t.hpp>

namespace renderer {

/// Shader abstraction class, used to handle a shader on GPU
class RENDERER_API Shader : public std::enable_shared_from_this<Shader> {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Shader)

    DEFINE_SMART_POINTERS(Shader)

 public:
    /// Creates a shader from a set of given parameters
    /// \param[in] type Type of the shader to be created on GPU
    /// \param[in] source Source code of the shader to be created on GPU
    /// \param[in] api The Graphics API to be used by this shader
    Shader(eShaderType type, const char* source, eGraphicsAPI api);

    /// Releases the resources allocated by this shader on the GPU
    ~Shader() = default;

    /// Creates a shader from a set of given parameters
    /// \param[in] type Type of the shader to be created on GPU
    /// \param[in] source Source code of the shader to be created on GPU
    /// \param[in] api The Graphics API to be used by this shader
    static auto CreateShader(eShaderType type, const char* source,
                             eGraphicsAPI api) -> std::shared_ptr<Shader>;

    /// Initializes the shader and backend-related resources
    auto Initialize() -> void;

    /// Compiles the associated shader resource
    auto Compile() -> void;

    /// Returns whether or not this shader is valid
    RENDERER_NODISCARD auto IsValid() const -> bool;

    /// Returns the type of this shader
    RENDERER_NODISCARD auto type() const -> eShaderType { return m_Type; }

    /// Returns the source code of this shader
    RENDERER_NODISCARD auto source() const -> std::string { return m_Source; }

 private:
    /// Creates the internal adapter to link to the specific Graphics API
    auto _InitializeBackend() -> void;

 private:
    /// The graphics API to be used by this shader
    eGraphicsAPI m_API;

    /// Type of this shader
    eShaderType m_Type = eShaderType::VERTEX;

    /// Source code attached to this shader
    std::string m_Source{};

    /// Owning reference to a shader adapter for a specific backend
    std::unique_ptr<IShaderAdapter> m_BackendAdapter = nullptr;
};

}  // namespace renderer

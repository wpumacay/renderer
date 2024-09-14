#pragma once

#include <memory>
#include <string>
#include <utility>

#include <renderer/common.hpp>
#include <renderer/engine/graphics/enums.hpp>

namespace renderer {

class Shader;

/// Interface for shader adapters, which link a shader object to its backend
class IShaderAdapter {
    NO_COPY_NO_MOVE_NO_ASSIGN(IShaderAdapter)

    DEFINE_SMART_POINTERS(IShaderAdapter)

   public:
    /// Creates an adapter to a specific backend
    IShaderAdapter() = default;

    /// Releases all resources in the specific backend
    virtual ~IShaderAdapter() = default;

    /// Sets a non-owning reference to the associated shader object
    auto SetShaderHandle(std::shared_ptr<Shader> shader_handle) -> void {
        m_ShaderHandle = shader_handle;
    }

    /// Requests the backend to compile the associated shader
    virtual auto Compile() -> void = 0;

    /// Checks if the associated shader was compile in the corresponding backend
    RENDERER_NODISCARD auto IsValid() const -> bool { return m_IsValid; }

   protected:
    /// Non-owning reference to a shader object
    std::weak_ptr<Shader> m_ShaderHandle;

    /// Whether or not the shader was successfully compiled
    bool m_IsValid = false;
};

} // namespace renderer






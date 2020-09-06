/// @file CProgram.h
/// @brief Shader-class implementation
#pragma once

#include <CCommon.h>
#include <shaders/CShader.h>

// @todo: Abstract opengl-stuff away from this class, as we will need to implement each shader according
// to its supporting backend, either OpenGL, DirectX, etc.

namespace engine
{
    /// \brief Shader-Program abstraction class, used to handle a shader-program
    ///
    /// \details
    /// Defines the functionality of a Shader Program, which is used to interact with a shader-program on GPU.
    /// This abstraction allows the user to update uniforms and other properties of a GPU-linked shader program
    class CProgram
    {
    public :
        /// \brief Creates a shader-program object with given name and source code for vertex and fragment shaders
        ///
        /// \param[in] name Unique identifier of this shader program
        /// \param[in] vertexShaderSource Source code of the vertex-shader
        /// \param[in] fragmentShaderSource Source code of the fragment-shader
        CProgram( const std::string& name,
                  const std::string& vertexShaderSource,
                  const std::string& fragmentShaderSource );

        /// \brief Releases the resources allocated for this Shader Program on GPU
        ~CProgram();

        /// \brief Binds the current shader for usage in the rendering pipeline
        void Bind();

        /// \brief Unbinds the current shader from the rendering pipeline
        void Unbind();

        /// \brief Sets an int32 uniform given its name and desired value
        ///
        /// \param[in] uniform_name Name of the int32 uniform
        /// \param[in] uniform_value Value of the int32 uniform
        void SetInt( const std::string& uniform_name, const int32& uniform_value );

        /// \brief Sets a float32 uniform given its name and desired value
        ///
        /// \param[in] uniform_name Name of the float32 uniform
        /// \param[in] uniform_value Value of the float32 uniform
        void SetFloat( const std::string& uniform_name, const float32& uniform_value );

        /// \brief Sets a vec-2 uniform given its name and desired value
        ///
        /// \param[in] uniform_name Name of the vec-2 uniform
        /// \param[in] uniform_value Value of the vec-2 uniform
        void SetVec2( const std::string& uniform_name, const CVec2& uniform_value );

        /// \brief Sets a vec-3 unbiform given its name and desired value
        ///
        /// \param[in] uniform_name Name of the vec-3 uniform
        /// \param[in] uniform_value Value of the vec-3 uniform
        void SetVec3( const std::string& uniform_name, const CVec3& uniform_value );

        /// \brief Sets a vec-4 unbiform given its name and desired value
        ///
        /// \param[in] uniform_name Name of the vec-4 uniform
        /// \param[in] uniform_value Value of the vec-4 uniform
        void SetVec4( const std::string& uniform_name, const CVec4& uniform_value );

        /// \brief Sets a mat-4 unbiform given its name and desired value
        ///
        /// \param[in] uniform_name Name of the mat-4 uniform
        /// \param[in] uniform_value Value of the mat-4 uniform
        void SetMat4( const std::string& uniform_name, const CMat4& uniform_value );

        /// \brief Returns the shader-source of the vertex-shader associated with this shader-program
        std::string GetVertexShaderSource() const;

        /// \brief Returns the shader-source of the fragment-shader associated with this shader-program
        std::string GetFragmentShaderSource() const;

        /// \brief Returns whether or not this shader-program was successfully linked and therefore can be used
        bool linked() const { return m_OpenglID != 0; }

        /// \brief Returns the name of this shader-program
        std::string name() const { return m_Name; }

        /// \brief Returns the opengl-id of this shader-program
        uint32 opengl_id() const { return m_OpenglID; }

    protected :

        /// Links this shader program based on both its vertex and fragment shaders
        void _LinkShaderProgram( std::unique_ptr<CShader> vertex_shader, std::unique_ptr<CShader> fragment_shader );

        /// Returns the uniform location of the uniform with the given name
        uint32 _GetUniformLocation( const std::string& uniform_name );

    protected :

        /// Unique string-identifier of this shader
        std::string m_Name;
        /// Resource-id of this shader-program on the GPU
        uint32 m_OpenglID = 0;
        /// Vertex Shader object
        CShader::uptr m_VertexShader = nullptr;
        /// Fragment Shader object
        CShader::uptr m_FragmentShader = nullptr;
        /// Map used to link uniforms' names to its respective locations
        std::unordered_map<std::string, uint32> m_UniformsLocations;

        ADD_CLASS_SMART_POINTERS(CProgram);
    };
}
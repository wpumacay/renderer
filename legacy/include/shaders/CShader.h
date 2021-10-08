/// @file CShader.h
/// @brief Shader-class implementation
#pragma once

#include <CCommon.h>

// @todo: Abstract opengl-stuff away from this class, as we will need to implement each shader according
// to its supporting backend, either OpenGL, DirectX, etc.

namespace engine
{
    /// Available shader-types
    enum class eShaderType
    {
        VERTEX,     ///< Vertex shader type
        FRAGMENT    ///< Fragment shader type
    };

    /// \brief Returns the string representation of the given shader type
    ///
    /// \param[in] type Shader type representation
    /// \return String representation of the given shader type
    std::string ToString( const eShaderType& type );

    /// \brief Converts the given shader type enum into its related OpenGL enum
    ///
    /// \param[in] type Shader type representation
    /// \return OpenGL enum for the given shader type
    uint32 ToOpenGLEnum( const eShaderType& type );

    /// \brief Shader abstraction class, used to handle a shader on GPU
    class CShader
    {
    public :
        /// \brief Creates a shader object with given name, type and source code
        ///
        /// \param[in] name Unique identifier of this shader
        /// \param[in] type Type of shader to be created on GPU
        /// \param[in] source Source code of the-shader
        CShader( const std::string& name, const eShaderType& type, const std::string& source );

        /// \brief Releases the resources allocated for this Shader on GPU
        ~CShader();

        /// \brief Returns the name of this shader
        std::string name() const { return m_Name; }

        /// \brief Returns the type of this shader
        eShaderType type() const { return m_Type; }

        /// \brief Returns the source code of this shader
        std::string source() const { return m_Source; }

        /// \brief Returns the opengl-id of this shader
        uint32 opengl_id() const { return m_OpenglID; }

        /// \brief Returns whether or not this shader was successfully compiled
        bool compiled() const { return m_Compiled; }

    protected :

        /// Compiles the given shader code given its source and the type of shader
        void _CompileShaderFromSource();

    protected :

        /// Unique string-identifier of this shader
        std::string m_Name;
        /// The type of this shader (either VERTEX, FRAGMENT)
        eShaderType m_Type;
        /// GLSL source code of this shader
        std::string m_Source;
        /// Resource-id of this shader on the GPU
        uint32 m_OpenglID = 0;
        /// Whether or not this shader is correctly compiled and ready to use
        bool m_Compiled = false;

        ADD_CLASS_SMART_POINTERS(CShader);
    };
}

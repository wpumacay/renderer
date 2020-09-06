/// @file CShaderManager.h
/// @brief Shader Manager class implementation
#pragma once

#include <shaders/CProgram.h>

namespace engine
{
    /// \brief Shader-Manager abstraction class, used to store and manage shader programs
    struct CShaderManager
    {
    public :

        /// \brief Initializes the shader-manager module
        static void Init();

        /// \brief Releases the resources allocated by this module
        static void Release();

        /// \brief Destroy the manager and releases resources
        ~CShaderManager();

        /// \brief Creates a shader program out of given source files
        ///
        /// \param[in] name Unique identifier of the shader program
        /// \param[in] vrtFile Source file for the vertex shader code
        /// \param[in] frgFile Source file for the fragment shader code
        /// \return A shared shader program object
        static CProgram::ptr CreateProgramFromFiles( const std::string& name,
                                                     const std::string& vrtFile,
                                                     const std::string& frgFile );

        /// \brief Creates a shader program out of given source code
        ///
        /// \param[in] name Unique identifier of the shader program
        /// \param[in] vrtFile Source code of the vertex shader
        /// \param[in] frgFile Source code of the fragment shader
        /// \return A shared shader program object
        static CProgram::ptr CreateProgramFromSources( const std::string& name,
                                                       const std::string& vrtSource,
                                                       const std::string& frgSource );

        /// \brief Returns an already loaded shader program given its name
        ///
        /// \param[in] name The unique identifier of the shader program when loaded
        /// \return A shared shader program object
        static CProgram::ptr GetCachedShader( const std::string& name );

    private :

        /// \brief Unique instance of this singleton module
        static std::unique_ptr<CShaderManager> s_Instance;

        /// \brief Creates a shader program out of given source files
        ///
        /// \param[in] name Unique identifier of the shader program
        /// \param[in] vrtFile Source file for the vertex shader code
        /// \param[in] frgFile Source file for the fragment shader code
        /// \return A shared shader program object
        CProgram::ptr _CreateProgramFromFiles( const std::string& name,
                                               const std::string& vrtFilepath,
                                               const std::string& frgFilepath );

        /// \brief Creates a shader program out of given source code
        ///
        /// \param[in] name Unique identifier of the shader program
        /// \param[in] vrtFile Source code of the vertex shader
        /// \param[in] frgFile Source code of the fragment shader
        /// \return A shared shader program object
        CProgram::ptr _CreateProgramFromSources( const std::string& name,
                                                 const std::string& vrtSource,
                                                 const std::string& frgSource );

        /// \brief Returns an already loaded shader program given its name
        ///
        /// \param[in] name The unique identifier of the shader program when loaded
        /// \return A shared shader program object
        CProgram::ptr _GetCachedShader( const std::string& name );

        /// \brief constructs a shader-manager object
        CShaderManager();

        /// \brief Loads all shader programs from disk, which are used internally by the engine itself
        void _LoadEngineShaders();

        /// \brief Returns the contents of a given file
        std::string _GetFileContents( const std::string& filepath );

        /// \brief Cached-container for stored shader programs
        std::unordered_map<std::string, CProgram::ptr> m_Programs;
    };
}
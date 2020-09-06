
#include <shaders/CShaderManager.h>

namespace engine
{
    std::unique_ptr<CShaderManager> CShaderManager::s_Instance = nullptr;

    void CShaderManager::Init()
    {
        if ( CShaderManager::s_Instance )
        {
            ENGINE_CORE_WARN( "CShaderManager::Init >>> Tried to initialize an already intialized shader-manager" );
            return;
        }
        CShaderManager::s_Instance = std::unique_ptr<CShaderManager>( new CShaderManager() );
    }

    void CShaderManager::Release()
    {
        ENGINE_CORE_ASSERT( CShaderManager::s_Instance, "CShaderManager::Release >>> Tried to release an already deleted shader-manager" );
        CShaderManager::s_Instance = nullptr;
    }

    CShaderManager::~CShaderManager()
    {
        // Nothing to release directly
    }

    CProgram::ptr CShaderManager::CreateProgramFromFiles( const std::string& name,
                                                          const std::string& vrtFile,
                                                          const std::string& frgFile )
    {
        ENGINE_CORE_ASSERT( CShaderManager::s_Instance, "CShaderManager::CreateProgramFromFiles >>> Must initialize shader-manager before using it" );
        return CShaderManager::s_Instance->_CreateProgramFromFiles( name, vrtFile, frgFile );
    }

    CProgram::ptr CShaderManager::CreateProgramFromSources( const std::string& name,
                                                            const std::string& vrtSource,
                                                            const std::string& frgSource )
    {
        ENGINE_CORE_ASSERT( CShaderManager::s_Instance, "CShaderManager::CreateProgramFromSources >>> Must initialize shader-manager before using it" );
        return CShaderManager::s_Instance->_CreateProgramFromSources( name, vrtSource, frgSource );
    }

    CProgram::ptr CShaderManager::GetCachedShader( const std::string& name )
    {
        ENGINE_CORE_ASSERT( CShaderManager::s_Instance, "CShaderManager::GetCachedShader >>> Must initialize shader-manager before using it" );
        return CShaderManager::s_Instance->_GetCachedShader( name );
    }

    CProgram::ptr CShaderManager::_CreateProgramFromFiles( const std::string& name,
                                                           const std::string& vrtFilepath,
                                                           const std::string& frgFilepath )
    {
        auto vertShaderSource = _GetFileContents( vrtFilepath );
        auto fragShaderSOurce = _GetFileContents( frgFilepath );
        return _CreateProgramFromSources( name, vertShaderSource, fragShaderSOurce );
    }

    CProgram::ptr CShaderManager::_CreateProgramFromSources( const std::string& name,
                                                             const std::string& vrtSource,
                                                             const std::string& frgSource )
    {
        auto shader_program = std::make_shared<CProgram>( name, vrtSource, frgSource );
        m_Programs[name] = shader_program;
        return shader_program;
    }

    CProgram::ptr CShaderManager::_GetCachedShader( const std::string& name )
    {
        if ( m_Programs.find( name ) == m_Programs.end() )
        {
            ENGINE_CORE_ERROR( "Requested shader with name {0} hasn't been loaded", name );
            return nullptr;
        }
        return m_Programs[name];
    }

    CShaderManager::CShaderManager()
    {
        _LoadEngineShaders();
    }

    void CShaderManager::_LoadEngineShaders()
    {
        // Base-path where to locate our shaders
        std::string shaders_base_folder = engine::RESOURCES_PATH + "shaders/";

        std::vector<std::string> engine_shader_files = {
            "basic2d_no_textures",
            "basic2d_textures",
            "basic3d_no_textures",
            "debug_drawing_3d",
            "phong_no_textures_no_shadows",
            "phong_no_textures_shadows",
            "phong_textures_no_shadows",
            "phong_textures_shadows",
            "shadow_mapping",
            "skybox",
            "engine_no_shadows_no_fog",
            "engine_no_shadows_fog",
            "engine_shadows_no_fog",
            "engine_shadows_fog",
            "engine_shadow_mapping",
            "engine_skybox_fog",
            "engine_skybox_no_fog",
            "engine_render_depth_view",
            "engine_render_semantic_view",
            "engine_debug_drawing_3d_solid_lighting",
            "engine_debug_drawing_3d_solid_no_lighting"
        };

        for ( size_t i = 0; i < engine_shader_files.size(); i++ )
        {
            std::string shader_name = engine_shader_files[i];
            if ( m_Programs.find( shader_name ) != m_Programs.end() )
                continue;

            std::string shader_vert_file = shaders_base_folder + engine_shader_files[i] + "_vs.glsl";
            std::string shader_frag_file = shaders_base_folder + engine_shader_files[i] + "_fs.glsl";
            _CreateProgramFromFiles( shader_name, shader_vert_file, shader_frag_file );
        }
    }

    std::string CShaderManager::_GetFileContents( const std::string& filepath )
    {
        std::string file_contents;
        std::stringstream file_sstream;

        std::ifstream file_handle;
        file_handle.exceptions( std::ifstream::badbit );
        try
        {
            file_handle.open( filepath.c_str() );
            file_sstream << file_handle.rdbuf();
            file_handle.close();

            file_contents = file_sstream.str();
        }
        catch ( ... )
        {
            ENGINE_CORE_ERROR( "CShaderManager::_GetFileContents >>> Could not load contents of shader file: {0}", filepath );
        }
        return file_contents;
    }
}
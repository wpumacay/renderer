
#include <shaders/CShaderManager.h>

namespace engine
{

    std::string toString( const eShaderType& type )
    {
        if ( type == eShaderType::VERTEX ) return "vertex-shader";
        if ( type == eShaderType::FRAGMENT ) return "fragment-shader";

        ENGINE_CORE_ASSERT( false, "Invalid eShaderType enum given" );

        return "undefined";
    }

    uint32 toOpenGLEnum( const eShaderType& type )
    {
        if ( type == eShaderType::VERTEX ) return GL_VERTEX_SHADER;
        if ( type == eShaderType::FRAGMENT ) return GL_FRAGMENT_SHADER;

        ENGINE_CORE_ASSERT( false, "Invalid eShaderType enum given" );

        return 0;
    }

    CShaderManager* CShaderManager::s_instance = nullptr;

    void CShaderManager::Init()
    {
        if ( CShaderManager::s_instance )
        {
            ENGINE_CORE_WARN( "Tried to initialize an already intialized shader-manager" );
            return;
        }

        CShaderManager::s_instance = new CShaderManager();
    }

    void CShaderManager::Release()
    {
        ENGINE_CORE_ASSERT( CShaderManager::s_instance, "Tried to release an already deleted shader-manager" );

        delete CShaderManager::s_instance;
        CShaderManager::s_instance = nullptr;
    }

    CShaderManager::~CShaderManager()
    {
        // nothing to release directly
    }

    CShader* CShaderManager::CreateShaderFromFiles( const std::string& name,
                                                    const std::string& vrtFile,
                                                    const std::string& frgFile )
    {
        ENGINE_CORE_ASSERT( CShaderManager::s_instance, "Must initialize shader-manager before using it" );

        return CShaderManager::s_instance->_createShaderFromFiles( name, vrtFile, frgFile );
    }

    CShader* CShaderManager::CreateShaderFromSources( const std::string& name,
                                                      const std::string& vrtSource,
                                                      const std::string& frgSource )
    {
        ENGINE_CORE_ASSERT( CShaderManager::s_instance, "Must initialize shader-manager before using it" );

        return CShaderManager::s_instance->_createShaderFromSources( name, vrtSource, frgSource );
    }

    CShader* CShaderManager::GetCachedShader( const std::string& name )
    {
        ENGINE_CORE_ASSERT( CShaderManager::s_instance, "Must initialize shader-manager before using it" );

        return CShaderManager::s_instance->_getCachedShader( name );
    }

    CShaderManager::CShaderManager()
    {
        /* Load engine-shaders from disk (resources directory of the engine)*/
        _loadEngineShaders();
    }

    CShader* CShaderManager::_getCachedShader( const std::string& name )
    {
        if ( m_shaders.find( name ) == m_shaders.end() )
        {
            ENGINE_CORE_ERROR( "Requested shader with name {0} hasn't been loaded", name );
            return nullptr;
        }

        return m_shaders[name].get();
    }

    void CShaderManager::_loadEngineShaders()
    {
        // Base-path where to locate our shaders
        std::string _shadersBaseFolder = std::string( ENGINE_RESOURCES_PATH ) + "shaders/";

        std::vector< std::string > _engineShaderFiles = {
            "basic2d_no_textures",
            "basic2d_textures",
            "basic3d_no_textures",
            "debug_drawing_3d",
            "phong_no_textures_no_shadows",
            "phong_no_textures_shadows",
            "phong_textures_no_shadows",
            "phong_textures_shadows",
            "shadow_mapping",
            "skybox"
        };

        for ( size_t i = 0; i < _engineShaderFiles.size(); i++ )
        {
            std::string _shaderName     = _engineShaderFiles[i];
            std::string _shaderVrtFile  = _shadersBaseFolder + _engineShaderFiles[i] + "_vs.glsl";
            std::string _shaderFrgFile  = _shadersBaseFolder + _engineShaderFiles[i] + "_fs.glsl";

            if ( m_shaders.find( _shaderName ) != m_shaders.end() )
                continue;

            _createShaderFromFiles( _shaderName, _shaderVrtFile, _shaderFrgFile );
        }
    }

    CShader* CShaderManager::_createShaderFromSources( const std::string& name,
                                                       const std::string& vrtSource,
                                                       const std::string& frgSource )
    {
        uint32 _vrtShaderOpenglId = _compileShaderFromSource( vrtSource, eShaderType::VERTEX );
        uint32 _frgShaderOpenglId = _compileShaderFromSource( frgSource, eShaderType::FRAGMENT );

        if ( _vrtShaderOpenglId == 0 || _frgShaderOpenglId == 0 )
        {
            ENGINE_CORE_ERROR( "An error occurred while compiling shader with name: {0}", name );
            return nullptr;
        }

        uint32 _programOpenglId = _linkShaderProgram( _vrtShaderOpenglId, _frgShaderOpenglId );

        // keep ownership of the created shaders
        auto _shader = new CShader( name, _programOpenglId );
        std::unique_ptr< CShader > _shaderPtr( _shader );
        m_shaders[name] = std::move( _shaderPtr );

        return _shader;
    }

    CShader* CShaderManager::_createShaderFromFiles( const std::string& name,
                                                     const std::string& vrtFilepath,
                                                     const std::string& frgFilepath )
    {
        uint32 _vrtShaderOpenglId = _compileShaderFromFile( vrtFilepath, eShaderType::VERTEX );
        uint32 _frgShaderOpenglId = _compileShaderFromFile( frgFilepath, eShaderType::FRAGMENT );

        if ( _vrtShaderOpenglId == 0 || _frgShaderOpenglId == 0 )
        {
            ENGINE_CORE_ERROR( "An error occurred while compiling shaders {0} and {1}", vrtFilepath, frgFilepath );
            return nullptr;
        }

        uint32 _programOpenglId = _linkShaderProgram( _vrtShaderOpenglId, _frgShaderOpenglId );

        // keep ownership of the created shaders
        auto _shader = new CShader( name, _programOpenglId );
        std::unique_ptr< CShader > _shaderPtr( _shader );
        m_shaders[name] = std::move( _shaderPtr );

        return _shader;
    }

    uint32 CShaderManager::_compileShaderFromFile( const std::string& filepath, const eShaderType& type )
    {
        /* load file contents ***************************************************/
        std::string _fileContents;
        std::stringstream _fileStream;

        std::ifstream _fileHandle;
        _fileHandle.exceptions( std::ifstream::badbit );

        try 
        {
            _fileHandle.open( filepath.c_str() );
            _fileStream << _fileHandle.rdbuf();
            _fileHandle.close();

            _fileContents = _fileStream.str();
        }
        catch ( ... )
        {
            ENGINE_CORE_ERROR( "Could not load contents of shader file: {0}", filepath );
            return 0;
        }

        /************************************************************************/

        uint32 _shaderOpenglId = _compileShaderFromSource( _fileContents, type );
        if ( _shaderOpenglId == 0 )
        {
            ENGINE_CORE_ERROR( "Error while compiling shader file: {0}", filepath );
            return 0;
        }

        return _shaderOpenglId;
    }

    uint32 CShaderManager::_compileShaderFromSource( const std::string& source, const eShaderType& type )
    {
        int32 _success;
        const char* _shaderSource = source.c_str();

        uint32 _shaderOpenglId = glCreateShader( engine::toOpenGLEnum( type ) );
        glShaderSource( _shaderOpenglId, 1, &_shaderSource, NULL );
        glCompileShader( _shaderOpenglId );
        glGetShaderiv( _shaderOpenglId, GL_COMPILE_STATUS, &_success );

        if ( !_success )
        {
            GLchar _errorLog[1024];
            glGetShaderInfoLog( _shaderOpenglId, 1024, NULL, _errorLog );
            ENGINE_CORE_ERROR( "Couldn't compile shader type {0}: {1}", 
                               engine::toString( type ), std::string( _errorLog ) );
            return 0;
        }

        return _shaderOpenglId;
    }

    uint32 CShaderManager::_linkShaderProgram( uint32 vrtShaderId, uint32 frgShaderId )
    {
        int32 _success;

        uint32 _programOpenglId = glCreateProgram();
        glAttachShader( _programOpenglId, vrtShaderId );
        glAttachShader( _programOpenglId, frgShaderId );
        glLinkProgram( _programOpenglId );

        glDetachShader( _programOpenglId, vrtShaderId );
        glDetachShader( _programOpenglId, frgShaderId );
        glDeleteShader( vrtShaderId );
        glDeleteShader( frgShaderId );
        glGetProgramiv( _programOpenglId, GL_LINK_STATUS, &_success );

        if ( !_success )
        {
            GLchar _errorLog[1024];
            glGetProgramInfoLog( _programOpenglId, 1024, NULL, _errorLog );
            ENGINE_CORE_ERROR( "Couldn't link shader program : {0}", std::string( _errorLog ) );
            return 0;
        }

        return _programOpenglId;
    }

}
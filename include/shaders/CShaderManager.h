#pragma once

#include <CCommon.h>
#include <shaders/CShader.h>
#include <shaders/CShaderUniforms.h>
#include <utils/CLogger.h>

#if defined( __linux__ )
#include <dirent.h>
#endif

namespace engine
{

    enum class eShaderType
    {
        NONE = 0,
        VERTEX, FRAGMENT
    };

    std::string toString( const eShaderType& type );
    uint32 toOpenGLEnum( const eShaderType& type );

    struct CShaderManager
    {

    public :

        static void Init();
        static void Release();

        ~CShaderManager();

        static std::shared_ptr< CShader > CreateShaderFromFiles( const std::string& name,
                                                                 const std::string& vrtFile,
                                                                 const std::string& frgFile );

        static std::shared_ptr< CShader > CreateShaderFromSources( const std::string& name,
                                                                   const std::string& vrtSource,
                                                                   const std::string& frgSource );

        static std::shared_ptr< CShader > GetCachedShader( const std::string& name );

    private :

        static CShaderManager* s_instance;

        CShaderManager();

        void _loadEngineShaders();

        std::shared_ptr< CShader > _getCachedShader( const std::string& name );

        std::shared_ptr< CShader > _createShaderFromSources( const std::string& name,
                                                             const std::string& vrtSource,
                                                             const std::string& frgSource );

        std::shared_ptr< CShader > _createShaderFromFiles( const std::string& name,
                                                           const std::string& vrtFilepath,
                                                           const std::string& frgFilepath );

        uint32 _compileShaderFromFile( const std::string& filepath, const eShaderType& type );
        uint32 _compileShaderFromSource( const std::string& filepath, const eShaderType& type );
        uint32 _linkShaderProgram( uint32 vrtShaderId, uint32 frgShaderId );

        std::map< std::string, std::shared_ptr< CShader > > m_shaders;
    };

}
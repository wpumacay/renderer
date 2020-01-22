#pragma once

#include <CCommon.h>
#include <shaders/CShader.h>
#include <utils/CLogger.h>

#if defined( __linux__ )
#include <dirent.h>
#endif

namespace engine
{

    enum class eShaderType
    {
        NONE = 0,
        VERTEX, 
        FRAGMENT
    };

    std::string toString( const eShaderType& type );
    uint32 toOpenGLEnum( const eShaderType& type );

    struct CShaderManager
    {

    public :

        static void Init();
        static void Release();

        ~CShaderManager();

        static CShader* CreateShaderFromFiles( const std::string& name,
                                               const std::string& vrtFile,
                                               const std::string& frgFile );

        static CShader* CreateShaderFromSources( const std::string& name,
                                                 const std::string& vrtSource,
                                                 const std::string& frgSource );

        static CShader* GetCachedShader( const std::string& name );

    private :

        static CShaderManager* s_instance;

        CShaderManager();

        void _loadEngineShaders();

        CShader* _getCachedShader( const std::string& name );

        CShader* _createShaderFromSources( const std::string& name,
                                           const std::string& vrtSource,
                                           const std::string& frgSource );

        CShader* _createShaderFromFiles( const std::string& name,
                                         const std::string& vrtFilepath,
                                         const std::string& frgFilepath );

        uint32 _compileShaderFromFile( const std::string& filepath, const eShaderType& type );
        uint32 _compileShaderFromSource( const std::string& filepath, const eShaderType& type );
        uint32 _linkShaderProgram( uint32 vrtShaderId, uint32 frgShaderId );

        std::map< std::string, std::unique_ptr< CShader > > m_shaders;
    };

}
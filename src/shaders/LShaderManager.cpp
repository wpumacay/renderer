

#include <shaders/LShaderManager.h>
#include "../Config.h"

// General purpose shaders
#include <shaders/LShaderBasic3d.h>
#include <shaders/LShaderDebug3d.h>
#include <shaders/LShaderFramebufferScreenRender.h>
#include <shaders/LShaderShadowMap.h>

// Special purpose shaders
#include <shaders/LShaderEntitiesLighting.h>
#include <shaders/LShaderEntitiesLightingShadows.h>
#include <shaders/LShaderTerrain1DVoxelsLighting.h>
#include <shaders/LShaderTerrain1DVoxelsLightingShadows.h>
#include <shaders/LShaderTerrainLighting.h>

using namespace std;

namespace engine
{

    LShaderManager* LShaderManager::INSTANCE = NULL;

    LShaderManager::LShaderManager()
    {
        // Load all shaders available

        GLuint _vShader, _fShader, _program;

        cout << "LShaderManager-> using glsl 330 core shaders" << endl;

        // Common shaders **************************************************

        _vShader = createShader( "basic3d_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "basic3d_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["basic3d"] = _program;
        programObjs["basic3d"] = new LShaderBasic3d( _program );

        _vShader = createShader( "debug/debug_3d_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "debug/debug_3d_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["debug3d"] = _program;
        programObjs["debug3d"] = new LShaderDebug3d( _program );

        _vShader = createShader( "postprocessing/framebuffer_screenrender_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "postprocessing/framebuffer_screenrender_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["pp_framebuffer_screenrender"] = _program;
        programObjs["pp_framebuffer_screenrender"] = new LShaderFramebufferScreenRender( _program );

        _vShader = createShader( "postprocessing/framebuffer_screenrender_channel_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "postprocessing/framebuffer_screenrender_channel_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["pp_framebuffer_screenrender_channel"] = _program;
        programObjs["pp_framebuffer_screenrender_channel"] = new LShaderFramebufferScreenRender( _program );

        _vShader = createShader( "shadows/shadow_mapping_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "shadows/shadow_mapping_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["shadow_mapping"] = _program;
        programObjs["shadow_mapping"] = new LShaderShadowMap( _program );

        // *****************************************************************
        // Specific rendering shaders **************************************

        // We have separate entities - terrain shaders in case there is the need ...
        // to do something different in for each case

        _vShader = createShader( "entities/lighting_entities_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "entities/lighting_entities_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["lighting_entities"] = _program;
        programObjs["lighting_entities"] = new LShaderEntitiesLighting( _program );

        _vShader = createShader( "entities/lighting_entities_shadows_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "entities/lighting_entities_shadows_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["lighting_entities_shadows"] = _program;
        programObjs["lighting_entities_shadows"] = new LShaderEntitiesLightingShadows( _program );

        _vShader = createShader( "terrain/terrain1DVoxels_lighting_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "terrain/terrain1DVoxels_lighting_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["terrain1DVoxels_lighting"] = _program;
        programObjs["terrain1DVoxels_lighting"] = new LShaderTerrain1DVoxelsLighting( _program );

        _vShader = createShader( "terrain/terrain1DVoxels_lighting_shadows_vs.glsl", GL_VERTEX_SHADER );
        _fShader = createShader( "terrain/terrain1DVoxels_lighting_shadows_fs.glsl", GL_FRAGMENT_SHADER );
        _program = createProgram( _vShader, _fShader );

        programs["terrain1DVoxels_lighting_shadows"] = _program;
        programObjs["terrain1DVoxels_lighting_shadows"] = new LShaderTerrain1DVoxelsLightingShadows( _program );

        // *****************************************************************
    }

    LShaderManager::~LShaderManager()
    {
        map< string, GLuint >::iterator _it;
        for ( _it = programs.begin(); _it != programs.end(); ++_it )
        {
            glDeleteProgram( _it->second );
        }
    }

    void LShaderManager::release()
    {
        if ( LShaderManager::INSTANCE != NULL )
        {
            delete LShaderManager::INSTANCE;
            LShaderManager::INSTANCE = NULL;
        }
    }

    void LShaderManager::create()
    {
        if ( LShaderManager::INSTANCE != NULL )
        {
            delete LShaderManager::INSTANCE;
        }

        LShaderManager::INSTANCE = new LShaderManager();
    }


    GLuint LShaderManager::createShader( const char* filename, int shaderType )
    {
        // Load the shader code into a string ********************************************
        string _shaderStrCode;
        ifstream _shaderFile;

        string _fullFilePath = ENGINE_RESOURCES_PATH;
		_fullFilePath += "shaders/";
        _fullFilePath += filename;

        cout << "file: " << _fullFilePath << endl;

        _shaderFile.exceptions( ifstream::badbit );

        try 
        {
            _shaderFile.open( _fullFilePath.c_str() );
            stringstream _shaderStream;
            _shaderStream << _shaderFile.rdbuf();

            _shaderFile.close();

            _shaderStrCode = _shaderStream.str();
        }
        catch ( ... )
        {
            cout << "LShader::createFromFile> failed opening the resource file" << endl;
            return 0;
        }

        const char* _shaderCode_cstr = _shaderStrCode.c_str();
        // *******************************************************************************

        GLint _success;
        GLchar _infoLog[1024];

        GLuint _shaderId = glCreateShader( shaderType );
        glShaderSource( _shaderId, 1, &_shaderCode_cstr, NULL );
        glCompileShader( _shaderId );
        glGetShaderiv( _shaderId, GL_COMPILE_STATUS,
                       &_success );

        if ( !_success )
        {
            glGetShaderInfoLog( _shaderId, 1024, 
                                NULL, _infoLog );
            cout << "Shader: " << _fullFilePath << endl;
            cout << "Failed to compile shader" << endl;
            cout << "Error: " << _infoLog << endl;

            return 0;
        }

        return _shaderId;
    }

    GLuint LShaderManager::createProgram( GLuint vShaderId, GLuint fShaderId )
    {
        GLint _success;
        GLchar _infoLog[1024];

        GLuint _programId = glCreateProgram();
        glAttachShader( _programId, vShaderId );
        glAttachShader( _programId, fShaderId );
        glLinkProgram( _programId );

        glDetachShader( _programId, vShaderId );
        glDetachShader( _programId, fShaderId );
        glDeleteShader( vShaderId );
        glDeleteShader( fShaderId );

        glGetProgramiv( _programId, GL_LINK_STATUS, &_success );
        if ( !_success )
        {
            glGetProgramInfoLog( _programId, 1024, NULL, _infoLog );
            cout << "LINKING ERROR: " << _infoLog << endl;

            return 0;
        }

        return _programId;
    }

}

#include "../include/LShaderManager.h"


namespace miniengine
{

    LShaderManager* LShaderManager::instance = NULL;

    LShaderManager::LShaderManager()
    {

    }

    LShaderManager::~LShaderManager()
    {

    }

    GLuint LShaderManager::addProgram( LProgram pProgram )
    {
        m_programs.push_back( pProgram );
        return m_programs.size() - 1;
    }

    LProgram& LShaderManager::getProgram( GLuint pId )
    {
        if ( pId < 0 && pId >= m_programs.size() )
        {
            throw "LShaderManager::getProgram> program not found!!!";
        }
        return m_programs[pId];
    }

    void LShaderManager::initialize()
    {
        cout << "initializing shader manager" << endl;

        // Load base shaders
        GLuint _id = createProgram( "res/shaders/baseVertexShader3d_no_lighting.glsl",
                                    "res/shaders/baseFragmentShader3d_no_lighting.glsl" );

        cout << "finished initializing shader manager" << endl;

        shaderTable[shaders::BASE_SHADER_3D_NO_LIGHTING] = _id;
    }

    void LShaderManager::create()
    {
        if ( LShaderManager::instance != NULL )
        {
            delete LShaderManager::instance;
        }

        LShaderManager::instance = new LShaderManager;
    }


    GLuint LShaderManager::createProgram( const GLchar* pVertexShaderResPath,
                                          const GLchar* pFragmentShaderResPath )
    {
        LProgram _program = LProgram::createProgram( pVertexShaderResPath,
                                                     pFragmentShaderResPath );
        GLuint _resId = addProgram( _program );
        
        return _resId;
    }

}
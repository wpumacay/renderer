
#include <app/COpenGLContext.h>

namespace engine
{

    COpenGLContext* COpenGLContext::s_instance = NULL;

    COpenGLContext* COpenGLContext::GetInstance()
    {
        ENGINE_CORE_ASSERT( COpenGLContext::s_instance, "Should have initialized app's gl-context by now" );

        return COpenGLContext::s_instance;
    }

    COpenGLContext::COpenGLContext( GLFWwindow* glfwWindowPtr )
    {
        m_glfwWindowPtr = glfwWindowPtr;

        ENGINE_CORE_ASSERT( m_glfwWindowPtr, "Must provide valid window handle" );

        COpenGLContext::s_instance = this;
    }

    COpenGLContext::~COpenGLContext()
    {
        m_glfwWindowPtr = NULL;
    }

    void COpenGLContext::init()
    {
        glfwMakeContextCurrent( m_glfwWindowPtr );

        glewExperimental = GL_TRUE;
        ENGINE_CORE_ASSERT( glewInit() == GLEW_OK, "Failed to initialize GLEW" );

        ENGINE_CORE_INFO( "OpenGL Info:" );
        ENGINE_CORE_INFO( "\tVendor     : {0}", glGetString( GL_VENDOR ) );
        ENGINE_CORE_INFO( "\tRenderer   : {0}", glGetString( GL_RENDERER ) );
        ENGINE_CORE_INFO( "\tVersion    : {0}", glGetString( GL_VERSION ) );
    }

    void COpenGLContext::swapBuffers()
    {
        glfwSwapBuffers( m_glfwWindowPtr );
    }

}
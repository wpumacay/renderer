
#include <core/COpenGLContext.h>

namespace engine
{

    COpenGLContext::COpenGLContext( GLFWwindow* windowHandle )
    {
        m_windowHandle = windowHandle;

        ENGINE_CORE_ASSERT( m_windowHandle, "Must provide valid window handle" );
    }

    COpenGLContext::~COpenGLContext()
    {
        m_windowHandle = NULL;
    }

    void COpenGLContext::init()
    {
        glfwMakeContextCurrent( m_windowHandle );

        glewExperimental = GL_TRUE;
        ENGINE_CORE_ASSERT( glewInit() == GLEW_OK, "Failed to initialize GLEW" );

        ENGINE_CORE_INFO( "OpenGL Info:" );
        ENGINE_CORE_INFO( "\tVendor     : {0}", glGetString( GL_VENDOR ) );
        ENGINE_CORE_INFO( "\tRenderer   : {0}", glGetString( GL_RENDERER ) );
        ENGINE_CORE_INFO( "\tVersion    : {0}", glGetString( GL_VERSION ) );
    }

    void COpenGLContext::swapBuffers()
    {
        glfwSwapBuffers( m_windowHandle );
    }

}
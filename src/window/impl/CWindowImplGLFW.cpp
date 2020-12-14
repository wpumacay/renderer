
#include <window/impl/CWindowImplGLFW.h>

namespace engine
{
    CWindowImplGLFW::CWindowImplGLFW( const CWindowProps& properties )
    {
        // Setup GLFW backend **********************************************************************
        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, properties.gl_api_version_major );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, properties.gl_api_version_minor );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    #ifdef __APPLE__
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
    #endif
        glfwWindowHint( GLFW_RESIZABLE, ( properties.resizable ) ? GLFW_TRUE : GLFW_FALSE );

        auto glfw_window = glfwCreateWindow( properties.width,
                                             properties.height,
                                             properties.title.c_str(),
                                             NULL, NULL );

        if ( !glfw_window )
        {
            ENGINE_CORE_ERROR( "CWindowImplGLFW >>> couldn't create glfw window" );
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent( glfw_window );
        ENGINE_CORE_ASSERT( gladLoadGL( glfwGetProcAddress ), "CWindowImplGLFW >>> failed to load GL using GLAD on the current GL context" );
        ENGINE_CORE_INFO( "OpenGL Info:" );
        ENGINE_CORE_INFO( "\tVendor     : {0}", glGetString( GL_VENDOR ) );
        ENGINE_CORE_INFO( "\tRenderer   : {0}", glGetString( GL_RENDERER ) );
        ENGINE_CORE_INFO( "\tVersion    : {0}", glGetString( GL_VERSION ) );

        glfwSetWindowUserPointer( glfw_window, this );

        glfwSetInputMode( glfw_window, GLFW_STICKY_KEYS, GLFW_TRUE );
        int fbuffer_width, fbuffer_height;
        glfwGetFramebufferSize( glfw_window, &fbuffer_width, &fbuffer_height );

        glViewport( 0, 0, fbuffer_width, fbuffer_height );
        glEnable( GL_DEPTH_TEST );
        glClearColor( properties.clearColor.x(), properties.clearColor.y(),
                      properties.clearColor.z(), properties.clearColor.w() );

        // Keep ownership of the GLFW window resource
        m_GlfwWindow = std::unique_ptr<GLFWwindow, GLFWWindowDestructor>( glfw_window );
        // *****************************************************************************************
    }

    CWindowImplGLFW::~CWindowImplGLFW()
    {
        m_GlfwWindow = nullptr;
    }

    void CWindowImplGLFW::_RegisterKeyCallbackInternal( FnPtr_keyboard_callback callback )
    {
        m_CallbackKey = callback;
    }

    void CWindowImplGLFW::_RegisterMouseCallbackInternal( FnPtr_mouse_callback callback )
    {
        m_CallbackMouse = callback;
    }

    void CWindowImplGLFW::_RegisterMouseMoveCallbackInternal( FnPtr_mousemove_callback callback )
    {
        m_CallbackMouseMove = callback;
    }

    void CWindowImplGLFW::_RegisterScrollCallbackInternal( FnPtr_scroll_callback callback )
    {
        m_CallbackScroll = callback;
    }

    void CWindowImplGLFW::_RegisterResizeCallbackInternal( FnPtr_resize_callback callback )
    {
        m_CallbackResize = callback;
    }

    void CWindowImplGLFW::_EnableCursorInternal()
    {
        glfwSetInputMode( m_GlfwWindow.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    }

    void CWindowImplGLFW::_DisableCursorInternal()
    {
        glfwSetInputMode( m_GlfwWindow.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    }

    void CWindowImplGLFW::_BeginInternal()
    {
        glClearColor( m_Properties.clearColor.x(),
                      m_Properties.clearColor.y(),
                      m_Properties.clearColor.z(),
                      m_Properties.clearColor.w() );
        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );
        glfwPollEvents();
    }

    void CWindowImplGLFW::_EndInternal()
    {
        glfwSwapBuffers( m_GlfwWindow.get() );
    }

    bool CWindowImplGLFW::_IsActiveInternal()
    {
        return glfwWindowShouldClose( m_GlfwWindow.get() ) == GL_FALSE;
    }

    void CWindowImplGLFW::_RequestCloseInternal()
    {
        glfwSetWindowShouldClose( m_GlfwWindow.get(), GL_TRUE );
    }
}
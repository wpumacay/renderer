
#include <window/CWindowGLFW.h>

namespace engine
{
    CWindowGLFW::CWindowGLFW( const CWindowProps& properties )
        : CIWindow( properties )

    {
        m_Type = eWindowType::WINDOWED_GLFW;

        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, properties.gl_api_version_major );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, properties.gl_api_version_minor );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    #ifdef __APPLE__
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    #endif
        glfwWindowHint( GLFW_RESIZABLE, ( properties.resizable ) ? GL_TRUE : GL_FALSE );

        m_GlfwWindow = std::unique_ptr<GLFWwindow, GLFWWindowDestructor>( 
                                                        glfwCreateWindow( m_Properties.width,
                                                                          m_Properties.height,
                                                                          m_Properties.title.c_str(),
                                                                          NULL, NULL ) );

        if ( !m_GlfwWindow )
        {
            ENGINE_CORE_ERROR( "CWindowGLFW >>> couldn't create glfw window" );
            glfwTerminate();
            return;
        }

        // Initialize modern-gl context using glew (@todo: should replace for glad?)
        glfwMakeContextCurrent( m_GlfwWindow.get() );
        glewExperimental = GL_TRUE;
        ENGINE_CORE_ASSERT( glewInit() == GLEW_OK, "CWindowGLFW >>> failed to initialize GLEW" );
        ENGINE_CORE_INFO( "OpenGL Info:" );
        ENGINE_CORE_INFO( "\tVendor     : {0}", glGetString( GL_VENDOR ) );
        ENGINE_CORE_INFO( "\tRenderer   : {0}", glGetString( GL_RENDERER ) );
        ENGINE_CORE_INFO( "\tVersion    : {0}", glGetString( GL_VERSION ) );

        glfwSetWindowUserPointer( m_GlfwWindow.get(), &m_Properties );

        glfwSetKeyCallback( m_GlfwWindow.get(), []( GLFWwindow* glfw_window_ptr, int key, int scancode, int action, int mode )
            {
                auto window_props = static_cast<CWindowProps*>( glfwGetWindowUserPointer( glfw_window_ptr ) );

                if ( window_props->callbackKey )
                    window_props->callbackKey( key, action );
            } );

        glfwSetMouseButtonCallback( m_GlfwWindow.get(), []( GLFWwindow* glfw_window_ptr, int button, int action, int mode )
            {
                auto window_props = static_cast<CWindowProps*>( glfwGetWindowUserPointer( glfw_window_ptr ) );

                double _x,_y;
                glfwGetCursorPos( glfw_window_ptr, &_x, &_y );

                if ( window_props->callbackMouse )
                    window_props->callbackMouse( button, action, _x, _y );
            } );

        glfwSetCursorPosCallback( m_GlfwWindow.get(), []( GLFWwindow* glfw_window_ptr, double x, double y )
            {
                auto window_props = static_cast<CWindowProps*>( glfwGetWindowUserPointer( glfw_window_ptr ) );

                if ( window_props->callbackMouseMove )
                    window_props->callbackMouseMove( x, y );
            } );

        glfwSetScrollCallback( m_GlfwWindow.get(), []( GLFWwindow* glfw_window_ptr, double xOff, double yOff )
            {
                auto window_props = static_cast<CWindowProps*>( glfwGetWindowUserPointer( glfw_window_ptr ) );

                if ( window_props->callbackScroll )
                    window_props->callbackScroll( xOff, yOff );
            } );

        glfwSetWindowSizeCallback( m_GlfwWindow.get(), []( GLFWwindow* glfw_window_ptr, int width, int height )
            {
                auto window_props = static_cast<CWindowProps*>( glfwGetWindowUserPointer( glfw_window_ptr ) );
                window_props->width = width;
                window_props->height = height;

                if ( window_props->callbackResize )
                    window_props->callbackResize( width, height );
            } );

        glfwSetInputMode( m_GlfwWindow.get(), GLFW_STICKY_KEYS, 1 );
        int fbuffer_width, fbuffer_height;
        glfwGetFramebufferSize( m_GlfwWindow.get(), &fbuffer_width, &fbuffer_height );
        m_Properties.width = fbuffer_width;
        m_Properties.height = fbuffer_height;

        glViewport( 0, 0, m_Properties.width, m_Properties.height );
        glEnable( GL_DEPTH_TEST );
        glClearColor( m_Properties.clearColor.x(), 
                      m_Properties.clearColor.y(),
                      m_Properties.clearColor.z(),
                      m_Properties.clearColor.w() );
    }

    CWindowGLFW::~CWindowGLFW()
    {
        m_GlfwWindow = nullptr;
        glfwTerminate();
    }

    void CWindowGLFW::_EnableCursorInternal()
    {
        glfwSetInputMode( m_GlfwWindow.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    }

    void CWindowGLFW::_DisableCursorInternal()
    {
        glfwSetInputMode( m_GlfwWindow.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    }

    void CWindowGLFW::_BeginInternal()
    {
        glClearColor( m_Properties.clearColor.x(), 
                      m_Properties.clearColor.y(),
                      m_Properties.clearColor.z(),
                      m_Properties.clearColor.w() );
        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );
        glfwPollEvents();
    }

    void CWindowGLFW::_EndInternal()
    {
        glfwSwapBuffers( m_GlfwWindow.get() );
    }

    bool CWindowGLFW::_ActiveInternal()
    {
        return glfwWindowShouldClose( m_GlfwWindow.get() ) == GL_FALSE;
    }

    void CWindowGLFW::_RequestCloseInternal()
    {
        glfwSetWindowShouldClose( m_GlfwWindow.get(), GL_TRUE ); 
    }
}
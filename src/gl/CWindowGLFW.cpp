
#include <gl/CWindowGLFW.h>

namespace engine
{
    CWindowGLFW::CWindowGLFW( const CWindowProps& properties )
        : CIWindow( properties )

    {
        m_type = eWindowType::GLFW;

        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, properties.gl_api_version_major );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, properties.gl_api_version_minor );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    #ifdef __APPLE__
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    #endif
        glfwWindowHint( GLFW_RESIZABLE, ( properties.resizable ) ? GL_TRUE : GL_FALSE );

        m_glfwWindow = glfwCreateWindow( m_properties.width,
                                         m_properties.height,
                                         m_properties.title.c_str(),
                                         NULL, NULL );

        if ( !m_glfwWindow )
        {
            ENGINE_CORE_ERROR( "CWindowGLFW >>> couldn't create glfw window" );
            glfwTerminate();
            return;
        }

        // Initialize modern-gl context using glew (@todo: should replace for glad?)
        glfwMakeContextCurrent( m_glfwWindow );
        glewExperimental = GL_TRUE;
        ENGINE_CORE_ASSERT( glewInit() == GLEW_OK, "CWindowGLFW >>> Failed to initialize GLEW" );
        ENGINE_CORE_INFO( "OpenGL Info:" );
        ENGINE_CORE_INFO( "\tVendor     : {0}", glGetString( GL_VENDOR ) );
        ENGINE_CORE_INFO( "\tRenderer   : {0}", glGetString( GL_RENDERER ) );
        ENGINE_CORE_INFO( "\tVersion    : {0}", glGetString( GL_VERSION ) );

        glfwSetWindowUserPointer( m_glfwWindow, &m_properties );

        glfwSetKeyCallback( m_glfwWindow, []( GLFWwindow* pWindow, int key, int scancode, int action, int mode )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( _wprops->callbackKey )
                    _wprops->callbackKey( key, action );
            } );

        glfwSetMouseButtonCallback( m_glfwWindow, []( GLFWwindow* pWindow, int button, int action, int mode )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                double _x,_y;
                glfwGetCursorPos( pWindow, &_x, &_y );

                if ( _wprops->callbackMouse )
                    _wprops->callbackMouse( button, action, _x, _y );
            } );

        glfwSetCursorPosCallback( m_glfwWindow, []( GLFWwindow* pWindow, double x, double y )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( _wprops->callbackMouseMove )
                    _wprops->callbackMouseMove( x, y );
            } );

        glfwSetScrollCallback( m_glfwWindow, []( GLFWwindow* pWindow, double xOff, double yOff )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( _wprops->callbackScroll )
                    _wprops->callbackScroll( xOff, yOff );
            } );

        glfwSetWindowSizeCallback( m_glfwWindow, []( GLFWwindow* pWindow, int width, int height )
            {
                auto _wprops = ( CWindowProps* ) glfwGetWindowUserPointer( pWindow );
                _wprops->width = width;
                _wprops->height = height;

                if ( _wprops->callbackResize )
                    _wprops->callbackResize( width, height );
            } );

        glfwSetInputMode( m_glfwWindow, GLFW_STICKY_KEYS, 1 );
        glfwGetFramebufferSize( m_glfwWindow, &m_properties.width, &m_properties.height );

        glViewport( 0, 0, m_properties.width, m_properties.height );
        glEnable( GL_DEPTH_TEST );
        glClearColor( m_properties.clearColor.x(), 
                      m_properties.clearColor.y(),
                      m_properties.clearColor.z(),
                      m_properties.clearColor.w() );
    }

    CWindowGLFW::~CWindowGLFW()
    {
        glfwDestroyWindow( m_glfwWindow );
        glfwTerminate();

        m_glfwWindow = nullptr;
    }

    void CWindowGLFW::_enableCursorInternal()
    {
        glfwSetInputMode( m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    }

    void CWindowGLFW::_disableCursorInternal()
    {
        glfwSetInputMode( m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    }

    void CWindowGLFW::_beginInternal()
    {
        glfwPollEvents();
    }

    void CWindowGLFW::_endInternal()
    {
        glfwSwapBuffers( m_glfwWindow );
    }

    bool CWindowGLFW::_activeInternal()
    {
        return glfwWindowShouldClose( m_glfwWindow ) == 0;
    }

    void CWindowGLFW::_requestCloseInternal()
    {
        glfwSetWindowShouldClose( m_glfwWindow, 1 ); 
    }

}
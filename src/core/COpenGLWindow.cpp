
#include <core/COpenGLWindow.h>

using namespace std;

namespace engine
{

    COpenGLWindow::COpenGLWindow( const CWindowProps& properties )
    {
        m_properties = properties;

        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, properties.gl_api_version_major );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, properties.gl_api_version_minor );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    #ifdef __APPLE__
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    #endif
        glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

        m_glfwWindowPtr = glfwCreateWindow( m_properties.width,
                                            m_properties.height,
                                            m_properties.title.c_str(),
                                            NULL, NULL );

        if ( !m_glfwWindowPtr )
        {
            glfwTerminate();
            ENGINE_CORE_ASSERT( false, "ERROR: Couldnt initialize glfw" );
        }

        m_glContext = new COpenGLContext( m_glfwWindowPtr );
        m_glContext->init();

        glfwSetWindowUserPointer( m_glfwWindowPtr, &m_properties );

        glfwSetKeyCallback( m_glfwWindowPtr, []( GLFWwindow* pWindow, int key, int scancode, int action, int mode )
            {
                CWindowProps _wprops = *( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( !_wprops.callbackKey )
                    return;

                _wprops.callbackKey( key, action );
            } );

        glfwSetMouseButtonCallback( m_glfwWindowPtr, []( GLFWwindow* pWindow, int button, int action, int mode )
            {
                CWindowProps _wprops = *( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( !_wprops.callbackMouse )
                    return;

                double _x, _y;
                glfwGetCursorPos( pWindow, &_x, &_y );

                _wprops.callbackMouse( button, action, _x, _y );
            } );

        glfwSetCursorPosCallback( m_glfwWindowPtr, []( GLFWwindow* pWindow, double x, double y )
            {
                CWindowProps _wprops = *( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( !_wprops.callbackMouse )
                    return;

                _wprops.callbackMouseMove( x, y );
            } );

        glfwSetScrollCallback( m_glfwWindowPtr, []( GLFWwindow* pWindow, double xOff, double yOff )
            {
                CWindowProps _wprops = *( CWindowProps* ) glfwGetWindowUserPointer( pWindow );

                if ( !_wprops.callbackScroll )
                    return;

                _wprops.callbackScroll( xOff, yOff );
            } );

        glfwSetInputMode( m_glfwWindowPtr, GLFW_STICKY_KEYS, 1 );
        glfwGetFramebufferSize( m_glfwWindowPtr, &m_properties.width, &m_properties.height );
        glViewport( 0, 0, m_properties.width, m_properties.height );

        glEnable( GL_DEPTH_TEST );
        glClearColor( CLEAR_COLOR );

        ENGINE_CORE_INFO( "Successfully created opengl-window" );
    }

    COpenGLWindow::~COpenGLWindow()
    {
        if ( m_glContext )
            delete m_glContext;

        m_glfwWindowPtr = NULL;
        m_glContext = NULL;

        glfwTerminate();
    }

    void COpenGLWindow::enableCursor()
    {
        glfwSetInputMode( m_glfwWindowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    }

    void COpenGLWindow::disableCursor()
    {
        glfwSetInputMode( m_glfwWindowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    }

    void COpenGLWindow::begin()
    {
        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );
        glfwPollEvents();
    }

    void COpenGLWindow::end()
    {
        m_glContext->swapBuffers();
    }

    bool COpenGLWindow::isActive()
    { 
        return glfwWindowShouldClose( m_glfwWindowPtr ) == 0;
    }

    void COpenGLWindow::requestClose() 
    { 
        glfwSetWindowShouldClose( m_glfwWindowPtr, 1 ); 
    }

    void COpenGLWindow::registerKeyCallback( FnPtr_keyboard_callback callback )
    {
        m_properties.callbackKey = callback;
    }

    void COpenGLWindow::registerMouseCallback( FnPtr_mouse_callback callback )
    {
        m_properties.callbackMouse = callback;
    }

    void COpenGLWindow::registerMouseMoveCallback( FnPtr_mousemove_callback callback )
    {
        m_properties.callbackMouseMove = callback;
    }

    void COpenGLWindow::registerScrollCallback( FnPtr_scroll_callback callback )
    {
        m_properties.callbackScroll = callback;
    }

}

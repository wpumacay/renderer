

#include <LWindow.h>

#include <iostream>

using namespace std;

namespace engine
{

    LWindow* LWindow::s_instance = NULL;

    LWindow* LWindow::GetInstance()
    {
        ENGINE_CORE_ASSERT( LWindow::s_instance, "Must create a window first." );

        return LWindow::s_instance;
    }

    LWindow::LWindow( const CWindowProps& properties )
    {
        LWindow::s_instance = this;

        m_properties = properties;

        m_keyCallback = NULL;
        m_mouseCallback = NULL;
        m_mouseMoveCallback = NULL;

        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, APP_CONTEXT_VERSION_MAJOR );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, APP_CONTEXT_VERSION_MINOR );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    #ifdef __APPLE__
        glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    #endif
        glfwWindowHint( GLFW_RESIZABLE, APP_RESIZABLE );

        m_windowHandle = glfwCreateWindow( m_properties.width,
                                           m_properties.height,
                                           m_properties.title.c_str(),
                                           NULL, NULL );

        if ( !m_windowHandle )
        {
            glfwTerminate();
            ENGINE_CORE_ASSERT( false, "ERROR: Couldnt initialize glfw" );
        }

        m_glContext = new COpenGLContext( m_windowHandle );
        m_glContext->init();

        glfwSetKeyCallback( m_windowHandle, LWindow::onKeyCallback );
        glfwSetMouseButtonCallback( m_windowHandle, LWindow::onMouseCallback );
        glfwSetCursorPosCallback( m_windowHandle, LWindow::onMouseMoveCallback );

        glfwSetInputMode( m_windowHandle, GLFW_STICKY_KEYS, 1 );

        glfwGetFramebufferSize( m_windowHandle, &m_properties.width, &m_properties.height );
        glViewport( 0, 0, m_properties.width, m_properties.height );

        glEnable( GL_DEPTH_TEST );
        glClearColor( CLEAR_COLOR );
    }

    LWindow::~LWindow()
    {
        m_windowHandle = NULL;
        glfwTerminate();
    }

    void LWindow::enableCursor()
    {
        glfwSetInputMode( m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    }

    void LWindow::disableCursor()
    {
        glfwSetInputMode( m_windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    }

    void LWindow::begin()
    {
        glClearColor( CLEAR_COLOR ); 
        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT ); 

        glfwPollEvents();
    }

    void LWindow::end()
    {
        m_glContext->swapBuffers();
    }

    bool LWindow::isActive()
    { 
        return glfwWindowShouldClose( m_windowHandle ) == 0;
    }

    void LWindow::requestClose() 
    { 
        glfwSetWindowShouldClose( m_windowHandle, 1 ); 
    }

    void LWindow::onKeyCallback( GLFWwindow* pWindow, int key, int scancode, int action, int mode )
    {
        if ( LWindow::s_instance->m_keyCallback == NULL )
            return;

        LWindow::s_instance->m_keyCallback( key, action );
    }

    void LWindow::onMouseCallback( GLFWwindow* pWindow, int button, int action, int mode )
    {
        if ( LWindow::s_instance->m_mouseCallback == NULL )
            return;

        double _x, _y;
        glfwGetCursorPos( pWindow, &_x, &_y );

        LWindow::s_instance->m_mouseCallback( button, action, _x, _y );
    }

    void LWindow::onMouseMoveCallback( GLFWwindow* pWindow, double x, double y )
    {
        if ( LWindow::s_instance->m_mouseMoveCallback == NULL )
            return;

        LWindow::s_instance->m_mouseMoveCallback( ( double ) x, ( double ) y );
    }

    void LWindow::registerKeyCallback( FnPtr_keyboard_callback callback )
    {
        m_keyCallback = callback;
    }

    void LWindow::registerMouseCallback( FnPtr_mouse_callback callback )
    {
        m_mouseCallback = callback;
    }

    void LWindow::registerMouseMoveCallback( FnPtr_mousemove_callback callback )
    {
        m_mouseMoveCallback = callback;
    }

}

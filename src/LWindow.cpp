

#include "LWindow.h"

#include <iostream>

using namespace std;

namespace engine
{

    LWindow* LWindow::INSTANCE = NULL;

    LWindow* LWindow::GetInstance()
    {
        if ( LWindow::INSTANCE == NULL )
        {
            LWindow::INSTANCE = new LWindow();
        }

        return LWindow::INSTANCE;
    }

    LWindow::LWindow()
    {

        LWindow::INSTANCE = this;
        m_keyCallback = NULL;
        m_mouseCallback = NULL;
        m_mouseMoveCallback = NULL;

        glfwInit();
        glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, APP_CONTEXT_VERSION_MAJOR );
        glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, APP_CONTEXT_VERSION_MINOR );
        glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
        glfwWindowHint( GLFW_RESIZABLE, APP_RESIZABLE );

        m_window = glfwCreateWindow( APP_WIDTH,
                                     APP_HEIGHT,
                                     APP_NAME, NULL, NULL );

        if ( m_window == NULL )
        {
            glfwTerminate();
            cout << "ERROR: Couldnt initialize glfw" << endl;
            m_initialized = false;
            return;
        }

        glfwMakeContextCurrent( m_window );

        glewExperimental = GL_TRUE;
        if ( glewInit() != GLEW_OK )
        {
            glfwTerminate();
            m_initialized = false;
            return;
        }

        glfwSetKeyCallback( m_window, LWindow::onKeyCallback );
        glfwSetMouseButtonCallback( m_window, LWindow::onMouseCallback );
        glfwSetCursorPosCallback( m_window, LWindow::onMouseMoveCallback );

        glfwSetInputMode( m_window, GLFW_STICKY_KEYS, 1 );

        glfwGetFramebufferSize( m_window, &m_width, &m_height );
        glViewport( 0, 0, m_width, m_height );

        glEnable( GL_DEPTH_TEST );

        glClearColor( CLEAR_COLOR );

        m_initialized = true;
    }

    LWindow::~LWindow()
    {
        m_window = NULL;
        glfwTerminate();
    }

    void LWindow::enableCursor()
    {
        glfwSetInputMode( m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
    }

    void LWindow::disableCursor()
    {
        glfwSetInputMode( m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    }

    void LWindow::onKeyCallback( GLFWwindow* pWindow, int key,
        int scancode, int action, int mode )
    {
        if ( LWindow::INSTANCE->m_keyCallback == NULL )
        {
            return;
        }

        LWindow::INSTANCE->m_keyCallback( key, action );
    }

    void LWindow::onMouseCallback( GLFWwindow* pWindow, int button,
        int action, int mode )
    {
        if ( LWindow::INSTANCE->m_mouseCallback == NULL )
        {
            return;
        }

        double _x, _y;
        glfwGetCursorPos( pWindow, &_x, &_y );

        LWindow::INSTANCE->m_mouseCallback( button, action, _x, _y );
    }

    void LWindow::onMouseMoveCallback( GLFWwindow* pWindow, double x, double y )
    {
        if ( LWindow::INSTANCE->m_mouseMoveCallback == NULL )
        {
            return;
        }

        LWindow::INSTANCE->m_mouseMoveCallback( ( double ) x, ( double ) y );
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


#include <gl/CIWindow.h>

using namespace std;

namespace engine
{
    std::string toString( const eWindowType& type )
    {
        if ( type == eWindowType::NONE ) return "none";
        else if ( type == eWindowType::GLFW ) return "glfw";
        else if ( type == eWindowType::HEADLESS_EGL ) return "headless_egl";

        return "undefined";
    }

    void CIWindow::enableCursor()
    {
        _enableCursorInternal();
    }

    void CIWindow::disableCursor()
    {
        _disableCursorInternal();
    }

    void CIWindow::begin()
    {
        glClearColor( m_properties.clearColor.x(), 
                      m_properties.clearColor.y(),
                      m_properties.clearColor.z(),
                      m_properties.clearColor.w() );

        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );
        
        _beginInternal();
    }

    void CIWindow::end()
    {
        _endInternal();
    }

    bool CIWindow::active()
    { 
        return _activeInternal();
    }

    void CIWindow::requestClose() 
    { 
        _requestCloseInternal();
    }

    void CIWindow::registerKeyCallback( FnPtr_keyboard_callback callback )
    {
        m_properties.callbackKey = callback;
    }

    void CIWindow::registerMouseCallback( FnPtr_mouse_callback callback )
    {
        m_properties.callbackMouse = callback;
    }

    void CIWindow::registerMouseMoveCallback( FnPtr_mousemove_callback callback )
    {
        m_properties.callbackMouseMove = callback;
    }

    void CIWindow::registerScrollCallback( FnPtr_scroll_callback callback )
    {
        m_properties.callbackScroll = callback;
    }

    void CIWindow::registerResizeCallback( FnPtr_resize_callback callback )
    {
        m_properties.callbackResize = callback;
    }
}
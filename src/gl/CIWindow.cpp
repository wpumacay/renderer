
#include <gl/CIWindow.h>

using namespace std;

namespace engine
{
    std::string ToString( const eWindowType& type )
    {
        /**/ if ( type == eWindowType::WINDOWED_GLFW ) return "windowed_glfw";
        else if ( type == eWindowType::HEADLESS_EGL ) return "headless_egl";

        return "undefined";
    }

    void CIWindow::RegisterKeyCallback( FnPtr_keyboard_callback callback )
    {
        m_Properties.callbackKey = callback;
    }

    void CIWindow::RegisterMouseCallback( FnPtr_mouse_callback callback )
    {
        m_Properties.callbackMouse = callback;
    }

    void CIWindow::RegisterMouseMoveCallback( FnPtr_mousemove_callback callback )
    {
        m_Properties.callbackMouseMove = callback;
    }

    void CIWindow::RegisterScrollCallback( FnPtr_scroll_callback callback )
    {
        m_Properties.callbackScroll = callback;
    }

    void CIWindow::RegisterResizeCallback( FnPtr_resize_callback callback )
    {
        m_Properties.callbackResize = callback;
    }

    void CIWindow::EnableCursor()
    {
        _EnableCursorInternal();
    }

    void CIWindow::DisableCursor()
    {
        _DisableCursorInternal();
    }

    void CIWindow::Begin()
    {
        _BeginInternal();
    }

    void CIWindow::End()
    {
        _EndInternal();
    }

    bool CIWindow::Active()
    { 
        return _ActiveInternal();
    }

    void CIWindow::RequestClose() 
    { 
        _RequestCloseInternal();
    }
}

#include <window/impl/CIWindowImpl.h>

namespace engine
{
    std::string ToString( const eWindowType& type )
    {
        /**/ if ( type == eWindowType::WINDOWED_GLFW ) return "windowed_glfw";
        else if ( type == eWindowType::HEADLESS_EGL ) return "headless_egl";
        else if ( type == eWindowType::HEADLESS_MESA ) return "headless_mesa";

        ENGINE_CORE_CRITICAL( "Undefined window-type enumerator given to ToString function" );
        return "undefined";
    }

    void CIWindowImpl::RegisterKeyCallback( FnPtr_keyboard_callback callback )
    {
        _RegisterKeyCallbackInternal( callback );
    }

    void CIWindowImpl::RegisterMouseCallback( FnPtr_mouse_callback callback )
    {
        _RegisterMouseCallbackInternal( callback );
    }

    void CIWindowImpl::RegisterMouseMoveCallback( FnPtr_mousemove_callback callback )
    {
        _RegisterMouseMoveCallbackInternal( callback );
    }

    void CIWindowImpl::RegisterScrollCallback( FnPtr_scroll_callback callback )
    {
        _RegisterScrollCallbackInternal( callback );
    }

    void CIWindowImpl::RegisterResizeCallback( FnPtr_resize_callback callback )
    {
        _RegisterResizeCallbackInternal( callback );
    }

    void CIWindowImpl::EnableCursor()
    {
        _EnableCursorInternal( callback );
    }

    void CIWindowImpl::DisableCursor()
    {
        _DisableCursorInternal( callback );
    }

    void CIWindowImpl::Begin()
    {
        _BeginInternal();
    }

    void CIWindowImpl::End()
    {
        _EndInternal();
    }

    bool CIWindowImpl::IsActive()
    {
        _IsActiveInternal();
    }

    void CIWindowImpl::RequestClose()
    {
        _RequestCloseInternal();
    }
}


#include <window/CWindow.h>

using namespace std;

namespace engine
{
    CWindow::CWindow( const CWindowProps& properties )
        : m_Properties( properties )
    {
        const auto backend_type = m_Properties.type;
        /**/ if ( backend_type == eWindowType::WINDOWED_GLFW )
        {
        #ifdef ENGINE_GLFW_ENABLED
            m_Impl = std::make_unique<CWindowImplGLFW>( m_Properties );
        #endif /* ENGINE_GLFW_ENABLED */
        }
        else if ( backend_type == eWindowType::HEADLESS_EGL )
        {
        #ifdef ENGINE_EGL_ENABLED
            m_Impl = std::make_unique<CWindowImplEGL>( m_Properties );
        #endif /* ENGINE_EGL_ENABLED */
        }
        else if ( backend_type == eWindowType::HEADLESS_MESA )
        {
        #ifdef ENGINE_MESA_ENABLED
            //// m_Impl = std::make_unique<CWindowImplMESA>( m_Properties );
            ENGINE_CORE_ERROR( "CWindow::CWindow >>> os-mesa backend is still work in progress. Will add support soon" );
        #endif /* ENGINE_MESA_ENABLED */
        }

        ENGINE_CORE_ASSERT( m_Impl, "CWindow::CWindow >>> couldnt create a backend-impl for the given window-type" );
    }

    CWindow::~CWindow()
    {
        // Release backend-specific resources
        m_Impl = nullptr;
    }

    void CWindow::RegisterKeyCallback( FnPtr_keyboard_callback callback )
    {
        m_Impl->RegisterKeyCallback( callback );
    }

    void CWindow::RegisterMouseCallback( FnPtr_mouse_callback callback )
    {
        m_Impl->RegisterMouseCallback( callback );
    }

    void CWindow::RegisterMouseMoveCallback( FnPtr_mousemove_callback callback )
    {
        m_Impl->RegisterMouseMoveCallback( callback );
    }

    void CWindow::RegisterScrollCallback( FnPtr_scroll_callback callback )
    {
        m_Impl->RegisterScrollCallback( callback );
    }

    void CWindow::RegisterResizeCallback( FnPtr_resize_callback callback )
    {
        m_Impl->RegisterResizeCallback( callback );
    }

    void CWindow::EnableCursor()
    {
        m_Impl->EnableCursor();
    }

    void CWindow::DisableCursor()
    {
        m_Impl->DisableCursor();
    }

    void CWindow::Begin()
    {
        m_Impl->Begin();
    }

    void CWindow::End()
    {
        m_Impl->End();
    }

    bool CWindow::IsActive()
    {
        return m_Impl->IsActive();
    }

    void CWindow::RequestClose()
    {
        m_Impl->RequestClose();
    }
}

/// \file CIWindowImpl.h
/// \brief Interface for backend-specific window support
#pragma once

#include <CCommon.h>
#include <input/CInputKey.h>
#include <input/CInputMouse.h>
#include <input/CInputCallbacks.h>

namespace engine
{
    /// Available types of windows (different backends)
    enum eWindowType
    {
        WINDOWED_GLFW,  ///< Window based on GLFW-backend
        HEADLESS_EGL,   ///< Headless window based on EGL backend
        HEADLESS_MESA,  ///< Headless window based on OS-Mesa backend
    };

    /// Returns the string representation of the given window type
    std::string ToString( const eWindowType& type );

    /// Data struct used for building a window with given properties
    struct CWindowProps
    {
        /// Type of windowing backend
        eWindowType type;
        /// Width of the window
        ssize_t width = 1024;
        /// Height of the window
        ssize_t height = 768;
        /// Title of the window
        std::string title = "Application Window";
        /// Color of the background of the window
        CVec4 clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
        /// Whether or not the window is resizable
        bool resizable = false;
        /// Version of the API for the created context (major-id)
        ssize_t gl_api_version_major = 3;
        /// Version of the API for the created context (minor-id)
        ssize_t gl_api_version_minor = 3;
    };

    /// \class CIWindowImpl
    /// \brief Window-backend interface for different windowing backends
    class CIWindowImpl 
    {
    public :

        CIWindowImpl( const CWindowProps& props );

        virtual ~CIWindowImpl();

        /// \brief Requests the backend to register a given callback for keyboard events
        void RegisterKeyCallback( FnPtr_keyboard_callback callback );

        /// \brief Requests the backend to register a given callback for mouse-button events
        void RegisterMouseCallback( FnPtr_mouse_callback callback );

        /// \brief Requests the backend to register a given callback for mouse-move events
        void RegisterMouseMoveCallback( FnPtr_mousemove_callback callback );

        /// \brief Requests the backend to register a given callback for scroll-wheel events
        void RegisterScrollCallback( FnPtr_scroll_callback callback );

        /// \brief Requests the backend to register a given callback for window-resize events
        void RegisterResizeCallback( FnPtr_resize_callback callback );

        /// \brief Requests the backend to enable the mouse-cursor (if applicable)
        void EnableCursor();

        /// \brief Requests the backend to disable the mouse-cursor (if applicable)
        void DisableCursor();

        /// \brief Requests the backend to prepare the window's backend resources before starting the render process
        void Begin();

        /// \brief Requests the backend to clean the window's backend resources after a render process is complete
        void End();

        /// \brief Requests the backend for the state of the associated window
        bool IsActive();

        /// \brief Requests the backend to the closing process of the associated window
        void RequestClose();


    protected :

        /// \brief Internal backend-specific implementation of the register-key-callback method
        virtual void _RegisterKeyCallback( FnPtr_keyboard_callback callback ) = 0;

        /// \brief Internal backend-specific implementation of the register-mouse-callback method
        virtual void _RegisterMouseCallback( FnPtr_mouse_callback callback ) = 0;

        /// \brief Internal backend-specific implementation of the register-mousemove-callback method
        virtual void _RegisterMouseMoveCallback( FnPtr_mousemove_callback callback ) = 0;

        /// \brief Internal backend-specific implementation of the register-scroll-callback method
        virtual void _RegisterScrollCallback( FnPtr_scroll_callback callback ) = 0;

        /// \brief Internal backend-specific implementation of the register-resize-callback method
        virtual void _RegisterResizeCallback( FnPtr_resize_callback callback ) = 0;

        /// \brief Internal backend-specific implementation of the enable-cursor method
        virtual void _EnableCursor() = 0;

        /// \brief Internal backend-specific implementation of the disable-cursor method
        virtual void _DisableCursor() = 0;

        /// \brief Internal backend-specific implementation of the begin method
        virtual void _Begin() = 0;

        /// \brief Internal backend-specific implementation of the end method
        virtual void _End() = 0;

        /// \brief Internal backend-specific implementation of the is-active method
        virtual bool _IsActive() = 0;

        /// \brief Internal backend-specific implementation of the request-close method
        virtual void _RequestClose() = 0;

    protected :

        /// Struct containing all window properties
        CWindowProps m_Properties;
    };
}
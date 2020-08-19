/// @file CIWindow.h
/// @brief Interface for window objects
#pragma once

#include <CCommon.h>
#include <input/CInputKey.h>
#include <input/CInputMouse.h>
#include <input/CInputCallbacks.h>

namespace engine
{
    /// Data struct used for building a window with given properties
    struct CWindowProps
    {
        /// Width of the window
        ssize_t width = 1024;
        /// Height of the window
        ssize_t height = 768;
        /// Title of the window
        std::string title = "OpenGL-window";
        /// Color of the background of the window
        CVec4 clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
        /// Whether or not the window is resizable
        bool resizable = false;
        /// Version of the API for the created context (major-id)
        ssize_t gl_api_version_major = 3;
        /// Version of the API for the created context (minor-id)
        ssize_t gl_api_version_minor = 3;
        /// Callback for keyboard events
        FnPtr_keyboard_callback callbackKey;
        /// Callback for mouse-button events
        FnPtr_mouse_callback callbackMouse;
        /// Callback for mouse-move events
        FnPtr_mousemove_callback callbackMouseMove;
        /// Callback for scroll-wheel events
        FnPtr_scroll_callback callbackScroll;
        /// Callback for window-resize events
        FnPtr_resize_callback callbackResize;
    };

    /// Available types of windows (different backends)
    enum eWindowType
    {
        WINDOWED_GLFW, ///< Window based on GLFW-backend
        HEADLESS_EGL   ///< Headless window based on EGL backend
    };

    /// Returns the string representation of the given window type
    std::string ToString( const eWindowType& type );

    /// \class CIWindow
    /// \brief Interface for all window-objects (backend-dependent)
    class CIWindow
    {
    public :

        /// \brief Constructs a window with the given properties
        CIWindow( const CWindowProps& properties ) : m_Properties( properties ) {}

        /// \brief Releases the resources of this window
        virtual ~CIWindow() {}

        /// \brief Registers a given callback for keyboard events
        void RegisterKeyCallback( FnPtr_keyboard_callback callback );

        /// \brief Registers a given callback for mouse-button events
        void RegisterMouseCallback( FnPtr_mouse_callback callback );

        /// \brief Registers a given callback for mouse-move events
        void RegisterMouseMoveCallback( FnPtr_mousemove_callback callback );

        /// \brief Registers a given callback for scroll-wheel events
        void RegisterScrollCallback( FnPtr_scroll_callback callback );

        /// \brief Registers a given callback for window-resize events
        void RegisterResizeCallback( FnPtr_resize_callback callback );

        /// \brief Enables the mouse-cursor for this window
        void EnableCursor();

        /// \brief Disables the mouse-cursor for this window
        void DisableCursor();

        /// \brief Prepares the window's resources before showing the rendered framebuffer
        void Begin();

        /// \brief Cleans the window's resources after showing the rendered framebuffer
        void End();

        /// \brief Returns whether the window is currently active or not
        bool Active();

        /// \brief Tells the window to start the closing process
        void RequestClose();

        /// \brief Returns the width of the window (in pixels)
        ssize_t width() const { return m_Properties.width; }

        /// \brief Returns the height of the window (in pixels)
        ssize_t height() const { return m_Properties.height; }

        /// \brief Returns the aspect ratio of the window
        float aspect() const { return ((float) m_Properties.width) / m_Properties.height; }

        /// \brief Returns the title of the window
        std::string title() const { return m_Properties.title; }

        /// \brief Returns the backend-type of the window
        eWindowType type() const { return m_Type; }
        
        /// \brief Returns an object with all window properties
        CWindowProps& properties() { return m_Properties; }

        /// \brief Returns an unmutable object with all window properties
        const CWindowProps& properties() const { return m_Properties; }

    protected :

        /// Executes backend-dependent functionality for enabling the cursor
        virtual void _EnableCursorInternal() = 0;

        /// Executes backend-dependent functionality for disabling the cursor
        virtual void _DisableCursorInternal() = 0;

        /// Executes backend-dependent functionality for preparing the window before rendering
        virtual void _BeginInternal() = 0;

        /// Executes backend-dependent functionality for cleaning the window after rendering
        virtual void _EndInternal() = 0;

        /// Executes backend-dependent functionality for querying the active-state of the window
        virtual bool _ActiveInternal() = 0;

        /// Executes backend-dependent functionality for telling the window to start the close process
        virtual void _RequestCloseInternal() = 0;

    protected :

        /// Struct containing all window properties
        CWindowProps m_Properties;
        /// Type of the backend used for the window
        eWindowType m_Type;
    };
}
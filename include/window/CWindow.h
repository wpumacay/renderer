/// @file CWindow.h
/// @brief Window class with support for multiple windowing-backends
#pragma once

#include <window/impl/CWindowImplGLFW.h>
#include <window/impl/CWindowImplEGL.h>
//// #include <window/CWindowImplMESA.h>

namespace engine
{
    class CIWindowImpl;
    class CWindowImplGLFW;
    class CWindowImplEGL;
    //// class CWindowImplMESA;
}

namespace engine
{
    /// \class CWindow
    /// \brief Window abstraction with support for multiple windowing backends
    class CWindow
    {
        friend class CIWindowImpl;
        friend class CWindowImplGLFW;
        friend class CWindowImplEGL;
        //// friend class CWindowImplMESA;

    public :

        /// \brief Constructs a window with the given properties
        CWindow( const CWindowProperties& properties )

        /// \brief Releases the resources of this window
        ~CWindow();

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

        /// \brief Prepares the window's resources before starting a render process
        void Begin();

        /// \brief Cleans the window's resources after a render process is complete
        void End();

        /// \brief Returns whether the window is currently active or not
        bool IsActive();

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
        eWindowType type() const { return m_Properties.type; }

    protected :

        /// Struct containing all window properties
        CWindowProperties m_Properties;
        /// Handle to the implementation in the specific windowing backend
        std::unique_ptr<CIWindowImpl> m_Impl = nullptr;
    };
}
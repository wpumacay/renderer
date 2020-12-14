/// @file CWindowImplGLFW.h
/// @brief Implementation of backend support for glfw-windows
#pragma once

#include <window/impl/CIWindowImpl.h>

#ifdef ENGINE_GLFW_ENABLED

#include <GLFW/glfw3.h>

namespace engine
{
    /// Custom deleter for unique_ptr types of GLFWWindow
    struct GLFWWindowDestructor
    {
        void operator() ( GLFWwindow* glfw_window_ptr )
        {
            if ( glfw_window_ptr )
                glfwDestroyWindow( glfw_window_ptr );
        }
    };

    /// \class CWindowImplGLFW
    /// \brief Window-class definition that uses GLFW as windowing-backend
    class CWindowImplGLFW : public CIWindowImpl
    {
    public :

        /// \brief Constructs a GLFW-backed window given user-properties
        CWindowImplGLFW( const CWindowProps& properties );

        /// \brief Releases the resources of this window backend
        ~CWindowImplGLFW();

        /// Returns a reference to the internal glfw-window resource
        GLFWwindow* glfw_window() { return m_GlfwWindow.get(); }

        /// Returns an unmutable reference to the internal glfw-window resource
        const GLFWwindow* glfw_window() const { return m_GlfwWindow.get(); }

    protected :

        // Documentation inherited
        void _RegisterKeyCallbackInternal( FnPtr_keyboard_callback callback ) override;

        // Documentation inherited
        void _RegisterMouseCallbackInternal( FnPtr_mouse_callback callback ) override;

        // Documentation inherited
        void _RegisterMouseMoveCallbackInternal( FnPtr_mousemove_callback callback ) override;

        // Documentation inherited
        void _RegisterScrollCallbackInternal( FnPtr_scroll_callback callback ) override;

        // Documentation inherited
        void _RegisterResizeCallbackInternal( FnPtr_resize_callback callback ) override;

        // Documentation inherited
        void _EnableCursorInternal() override;

        // Documentation inherited
        void _DisableCursorInternal() override;

        // Documentation inherited
        void _BeginInternal() override;

        // Documentation inherited
        void _EndInternal() override;

        // Documentation inherited
        bool _IsActiveInternal() override;

        // Documentation inherited
        void _RequestCloseInternal() override;

    private :

        /// Handle to glfw-window resource
        std::unique_ptr<GLFWwindow, GLFWWindowDestructor> m_GlfwWindow = nullptr;
        /// Callback for keyboard events
        FnPtr_keyboard_callback m_CallbackKey = nullptr;
        /// Callback for mouse-button events
        FnPtr_mouse_callback m_CallbackMouse = nullptr;
        /// Callback for mouse-move events
        FnPtr_mousemove_callback m_CallbackMouseMove = nullptr;
        /// Callback for scroll-wheel events
        FnPtr_scroll_callback m_CallbackScroll = nullptr;
        /// Callback for window-resize events
        FnPtr_resize_callback m_CallbackResize = nullptr;
    };
}

#endif /* ENGINE_GLFW_ENABLED */
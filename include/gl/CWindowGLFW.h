/// @file CWindowGLFW.h
/// @brief Implementation of the window-class with GLFW backend
#pragma once

#include <gl/CIWindow.h>
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

    /// \class CWindowGLFW
    /// \brief Window-class definition that uses GLFW as windowing-backend
    class CWindowGLFW : public CIWindow
    {
    public :

        /// \brief Constructs a GLFW-backed window given user-properties
        CWindowGLFW( const CWindowProps& properties );

        /// \brief Releases the resources of this window
        ~CWindowGLFW();

        /// Returns a reference to the internal glfw-window resource
        GLFWwindow* glfw_window() { return m_GlfwWindow.get(); }

        /// Returns an unmutable reference to the internal glfw-window resource
        const GLFWwindow* glfw_window() const { return m_GlfwWindow.get(); }

    protected :

        // Documentation inherited
        void _EnableCursorInternal() override;

        // Documentation inherited
        void _DisableCursorInternal() override;

        // Documentation inherited
        void _BeginInternal() override;

        // Documentation inherited
        void _EndInternal() override;

        // Documentation inherited
        bool _ActiveInternal() override;

        // Documentation inherited
        void _RequestCloseInternal() override;

    private :

        /// Handle to glfw-window resource
        std::unique_ptr<GLFWwindow, GLFWWindowDestructor> m_GlfwWindow;

        ADD_CLASS_SMART_POINTERS(CWindowGLFW);
    };
}
#pragma once

#include <gl/CIWindow.h>
#include <GLFW/glfw3.h>

namespace engine
{
    class CWindowGLFW : public CIWindow
    {
    public :

        CWindowGLFW( const CWindowProps& properties );
        ~CWindowGLFW();

        GLFWwindow* glfwWindow() { return m_glfwWindow; }

    protected :

        void _enableCursorInternal() override;
        void _disableCursorInternal() override;
        void _beginInternal() override;
        void _endInternal() override;
        bool _activeInternal() override;
        void _requestCloseInternal() override;

    private :

        GLFWwindow* m_glfwWindow;

    };
}
#pragma once

#include <LCommon.h>
#include <utils/CLogger.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>

namespace engine
{

    class COpenGLContext
    {
    public :
        COpenGLContext( GLFWwindow* windowHandle );
        ~COpenGLContext();

        void init();
        void swapBuffers();

        GLFWwindow* window() { return m_windowHandle; }

    private :
        GLFWwindow* m_windowHandle;

    };

}
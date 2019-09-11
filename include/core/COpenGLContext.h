#pragma once

#include <LCommon.h>
#include <utils/CLogger.h>
#include <GLFW/glfw3.h>

namespace engine
{

    class COpenGLContext
    {
    public :
        static COpenGLContext* GetInstance();

        COpenGLContext( GLFWwindow* glfwWindowPtr );
        ~COpenGLContext();

        void init();
        void swapBuffers();

        GLFWwindow* window() { return m_glfwWindowPtr; }

    private :
        // single app-context support for now (at least our app, as imgui can have its own context)
        static COpenGLContext* s_instance;

        GLFWwindow* m_glfwWindowPtr;

    };

}
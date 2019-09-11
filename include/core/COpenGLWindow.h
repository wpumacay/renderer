#pragma once

#include "../../Config.h"

#include <LCommon.h>
#include <GLFW/glfw3.h>

#include <input/CInputKey.h>
#include <input/CInputMouse.h>

#include <core/COpenGLContext.h>
#include <utils/CLogger.h>

#define CLEAR_COLOR 0.6f, 0.659f, 0.690f, 1.0f

namespace engine
{

    struct CWindowProps
    {
        int width = 1024;
        int height = 768;
        std::string title;

        FnPtr_keyboard_callback callbackKey;
        FnPtr_mouse_callback callbackMouse;
        FnPtr_mousemove_callback callbackMouseMove;

        CWindowProps()
        {
            width = 1024;
            height = 768;
            title = "OpenGL-Window";

            callbackKey = nullptr;
            callbackMouse = nullptr;
            callbackMouseMove = nullptr;
        }
    };

    class COpenGLWindow
    {
    public :

        COpenGLWindow( const CWindowProps& properties );
        ~COpenGLWindow();

        void registerKeyCallback( FnPtr_keyboard_callback callback );
        void registerMouseCallback( FnPtr_mouse_callback callback );
        void registerMouseMoveCallback( FnPtr_mousemove_callback callback );

        void enableCursor();
        void disableCursor();

        void begin();
        void end();

        bool isActive();
        void requestClose();

        int width() { return m_properties.width; }
        int height() { return m_properties.height; }
        std::string title() { return m_properties.title; }
        
        CWindowProps properties() { return m_properties; }
        COpenGLContext* context() { return m_glContext; }
        GLFWwindow* glfwWindow() { return m_glfwWindowPtr; }

    private :

        GLFWwindow* m_glfwWindowPtr;

        CWindowProps m_properties;

        COpenGLContext* m_glContext;
    };

}
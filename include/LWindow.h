

#pragma once

#include "../Config.h"
#include "../Common.h"

#include <LCommon.h>
#include <GLFW/glfw3.h>
#include <core/COpenGLContext.h>

#define CLEAR_COLOR 0.6f, 0.659f, 0.690f, 1.0f

namespace engine
{

    struct CWindowProps
    {
        int width;
        int height;
        std::string title;
    };

    class LWindow
    {
    public :

        static LWindow* GetInstance();

        LWindow( const CWindowProps& properties );
        ~LWindow();

        static void onKeyCallback( GLFWwindow* pWindow, int key, int scancode, int action, int mode );
        static void onMouseCallback( GLFWwindow* pWindow, int button, int action, int mode );
        static void onMouseMoveCallback( GLFWwindow* pWindow, double x, double y );

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
        GLFWwindow* glfwWindow() { return m_windowHandle; }

    private :

        static LWindow* s_instance;

        GLFWwindow* m_windowHandle;

        CWindowProps m_properties;

        COpenGLContext* m_glContext;

        FnPtr_keyboard_callback m_keyCallback;
        FnPtr_mouse_callback m_mouseCallback;
        FnPtr_mousemove_callback m_mouseMoveCallback;
    };

}
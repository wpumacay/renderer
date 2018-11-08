

#pragma once

#include "../Config.h"
#include "../Common.h"

#include <LCommon.h>
#include <GLFW/glfw3.h>

#define CLEAR_COLOR 0.6f, 0.659f, 0.690f, 1.0f

namespace engine
{

    class LWindow
    {


        private :

        static LWindow* INSTANCE;

        GLFWwindow* m_window;
        LWindow();

        int m_width;
        int m_height;

        FnPtr_keyboard_callback m_keyCallback;
        FnPtr_mouse_callback m_mouseCallback;
        FnPtr_mousemove_callback m_mouseMoveCallback;

        bool m_initialized;

        public :

        static LWindow* GetInstance();
        ~LWindow();

        static void onKeyCallback( GLFWwindow* pWindow, int key, 
                                   int scancode, int action, int mode );

        static void onMouseCallback( GLFWwindow* pWindow, int button, 
                                     int action, int mode );
        static void onMouseMoveCallback( GLFWwindow* pWindow, double x, double y );

        void registerKeyCallback( FnPtr_keyboard_callback callback );
        void registerMouseCallback( FnPtr_mouse_callback callback );
        void registerMouseMoveCallback( FnPtr_mousemove_callback callback );

        void enableCursor();
        void disableCursor();

        void clear() 
        {
            glClearColor( CLEAR_COLOR ); 
            glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT ); 
        }

        void pollEvents() { glfwPollEvents(); }
        void swapBuffers() { glfwSwapBuffers( m_window ); }

        bool isActive() { return glfwWindowShouldClose( m_window ) == 0; }
        bool isInitialized() { return m_initialized; }

        void requestClose() { glfwSetWindowShouldClose( m_window, 1 ); }

        int width() { return m_width; }
        int height() { return m_height; }

        GLFWwindow* getGLFWwindow() { return m_window; }

    };










}
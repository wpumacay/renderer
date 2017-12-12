

#pragma once

#include "../LConfig.h"
#include "../LCommon.h"

#include <GLFW/glfw3.h>

#define CLEAR_COLOR 0.529f, 0.807f, 0.92f, 1.0f

namespace engine
{

    class LWindow
    {

        private :

        static LWindow* INSTANCE;

        GLFWwindow* m_window;

        int m_width;
        int m_height;

        FnPtr_keyboard_callback m_keyCallback;
        FnPtr_mouse_callback m_mouseCallback;

        bool m_initialized;

        public :

        LWindow();
        ~LWindow();
        void release();

        static void onKeyCallback( GLFWwindow* pWindow, int key, 
                                   int scancode, int action, int mode );

        static void onMouseCallback( GLFWwindow* pWindow, int button, 
                                     int action, int mode );

        void registerKeyCallback( FnPtr_keyboard_callback callback );
        void registerMouseCallback( FnPtr_mouse_callback callback );

        void clear() { glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT ); }

        void pollEvents() 
        { 
            glfwPollEvents();
        }

        void swapBuffers() 
        { 
            glfwSwapBuffers( m_window ); 
        }

        bool isActive() 
        { 
            return glfwWindowShouldClose( m_window ) == 0; 
        }

        bool isInitialized() { return m_initialized; }

        int width() { return m_width; }
        int height() { return m_height; }

    };










}


#pragma once

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../LConfig.h"
#include "../LCommon.h"



namespace engine
{

    namespace core
    {

        class LBaseApp
        {

            protected :

            GLFWwindow* m_window;

            int m_width;
            int m_height;

            float m_timeBef;
            float m_timeNow;
            float m_timeDelta;

            bool m_initialized;

            LBaseApp();

            public :

            static LBaseApp* instance;
            static void create();
            static void destroy();

            ~LBaseApp();

            bool init();
            void loop();
            void finalize();

            virtual void onStartup()
            {
                // Override this
            }

            virtual void render() 
            {
                // Override this
            }

            int width() { return m_width; }
            int height() { return m_height; }
            bool hasInitialized() { return m_initialized; }

            virtual void onKeyCallback( int pKey, int pScancode, 
                                        int pAction, int pMode )
            {
                // Override this
            }

            virtual void onMouseButtonCallback( int pButton, 
                                                int pAction, 
                                                int pMods )
            {
                // Override this
            }

            virtual void onCursorCallback( double x, double y )
            {
                // Override this
            }

            virtual void onMouseScrollCallback( double xOff, double yOff )
            {
                // Override this
            }

            static void onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
                                    int pAction, int pMode );

            static void onMouseEvent( GLFWwindow* pWindow, int pButton, 
                                      int pAction, int pMods );

            static void onCursorPosEvent( GLFWwindow* pWindow, double x, double y );

            static void onScrollEvent( GLFWwindow* pWindow, double xOff, double yOff );
        };

    }


}


#pragma once

#include <iostream>

#include "../LConfig.h"

#define GLEW_STATIC
#include <GL/glew.h>
#ifdef USE_GLFW
#include <GLFW/glfw3.h>
#endif

#include "../LCommon.h"

#ifdef USE_GLUT

#define KEY_UP 0
#define KEY_DOWN 1

    #ifdef __APPLE__

    #include "GLUT/glut.h"

    #else

    #include <GL/glut.h>

    #endif

#endif


namespace engine
{

    namespace core
    {

        class LBaseApp
        {

            protected :
#ifdef USE_GLFW
            GLFWwindow* m_window;
#endif
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

#ifdef USE_GLFW

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

#else

            virtual void onKeyCallback( int pKey, int pAction )
            {
                // Override this
            }

            virtual void onMouseButtonCallback( int pButton, 
                                                int pAction, 
                                                int x, int y )
            {
                // Override this
            }

            virtual void onCursorCallback( int x, int y )
            {
                // Override this
            }

            static void onKeyDownEvent( unsigned char pKey, int x, int y );

            static void onKeyUpEvent( unsigned char pKey, int x, int y );

            static void onMouseEvent( int pButton, 
                                      int pAction, 
                                      int x, int y );

            static void onCursorPosEvent( int x, int y );



            static void onDisplayEvent();

            static void onIdleEvent();

#endif


        };

    }


}
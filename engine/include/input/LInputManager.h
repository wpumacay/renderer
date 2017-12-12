
#pragma once

#include "../LCommon.h"
#include "../LKeyDefinitions.h"

#ifdef USE_GLFW
#include <GLFW/glfw3.h>
#endif

namespace engine { namespace core {


    class LInputManager
    {

        private :

        int m_keysStates[L_MAX_KEYS];
        int m_buttonsStates[L_MAX_BUTTONS];

        float m_cursorX;
        float m_cursorY;

        LInputManager();

        public :

        static LInputManager* instance;

        static void create();
        static void destroy();
        ~LInputManager();

        void update();

        /************************************************
        * Public accessors
        *************************************************/
        bool isKeyPressed( int key ) { return m_keysStates[key] == 1; }
        bool isMouseButtonClicked( int button ) { return m_buttonsStates[button] == 1; }
        float getCursorX() { return m_cursorX; }
        float getCursorY() { return m_cursorY; }

        /************************************************
        * Input callbacks
        *************************************************/
#ifdef USE_GLFW
        static void onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
                                int pAction, int pMode );

        static void onMouseEvent( GLFWwindow* pWindow, int pButton, 
                                  int pAction, int pMods );

        static void onCursorPosEvent( GLFWwindow* pWindow, double x, double y );

        static void onScrollEvent( GLFWwindow* pWindow, double xOff, double yOff );
#else
        // TODO: define glut support
#endif
    };

} }


#pragma once

#include <LCommon.h>
#include <utils/CLogger.h>
#include <input/CInputKey.h>
#include <input/CInputMouse.h>
#include <input/CInputCallbacks.h>

using namespace std;

namespace engine
{
    class LInputHandler
    {
    public :

        static void Init();
        static void Release();

        static LInputHandler* GetInstance();

        ~LInputHandler();

        static void callback_key( int key, int action );
        static void callback_mouse( int button, int action, double x, double y );
        static void callback_mouseMove( double x, double y );
        static void callback_scroll( double xOff, double yOff );

        bool isKeyDown( int key );
        bool checkSingleKeyPress( int key );
        bool isMouseDown( int button );
        LVec2 getCursorPosition();
        float getScrollOffX();
        float getScrollOffY();
        float getScrollAccumValueX();
        float getScrollAccumValueY();

    private :
        static LInputHandler* s_instance;
        LInputHandler();

        int m_keys[ENGINE_MAX_KEYS];
        bool m_singleKeys[ENGINE_MAX_KEYS];
        int m_buttons[ENGINE_MAX_BUTTONS];

        float m_mouseX;
        float m_mouseY;

        float m_scrollOffX;
        float m_scrollOffY;

        float m_scrollAccumValueX;
        float m_scrollAccumValueY;
    };
}

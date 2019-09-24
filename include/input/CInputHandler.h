#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <input/CInputKey.h>
#include <input/CInputMouse.h>
#include <input/CInputCallbacks.h>

namespace engine
{
    class CInputHandler
    {

    public :

        static void Init();
        static void Release();

        ~CInputHandler();

        static void callback_key( int key, int action );
        static void callback_mouse( int button, int action, double x, double y );
        static void callback_mouseMove( double x, double y );
        static void callback_scroll( double xOff, double yOff );

        static bool IsKeyDown( int key );
        static bool CheckSingleKeyPress( int key );
        static bool IsMouseDown( int button );
        static CVec2 GetCursorPosition();
        static float GetScrollOffX();
        static float GetScrollOffY();
        static float GetScrollAccumValueX();
        static float GetScrollAccumValueY();

    private :

        static CInputHandler* s_instance;

        CInputHandler();

        bool _isKeyDown( int key );
        bool _checkSingleKeyPress( int key );
        bool _isMouseDown( int button );

        CVec2 _getCursorPosition() const { return m_cursor; }
        float _getScrollOffX() const { return m_scrollOff.x; }
        float _getScrollOffY() const { return m_scrollOff.y; }
        float _getScrollAccumValueX() const { return m_scrollAccumValue.x; }
        float _getScrollAccumValueY() const { return m_scrollAccumValue.y; }

        int m_keys[ENGINE_MAX_KEYS];
        int m_singleKeys[ENGINE_MAX_KEYS];
        int m_buttons[ENGINE_MAX_BUTTONS];

        CVec2 m_cursor;
        CVec2 m_scrollOff;
        CVec2 m_scrollAccumValue;
    };
}

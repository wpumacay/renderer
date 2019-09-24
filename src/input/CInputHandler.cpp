
#include <input/CInputHandler.h>

namespace engine
{

    CInputHandler* CInputHandler::s_instance = NULL;

    CInputHandler::CInputHandler()
    {
        for ( int q = 0; q < ENGINE_MAX_KEYS; q++ )
            m_keys[q] = ENGINE_KEY_RELEASED;

        for ( int q = 0; q < ENGINE_MAX_KEYS; q++ )
            m_singleKeys[q] = ENGINE_KEY_RELEASED;

        for ( int q = 0; q < ENGINE_MAX_BUTTONS; q++ )
            m_buttons[q] = 0;

        m_cursor            = { 0.0f, 0.0f };
        m_scrollOff         = { 0.0f, 0.0f };
        m_scrollAccumValue  = { 0.0f, 0.0f };
    }

    CInputHandler::~CInputHandler()
    {
        // nothing for now, as "Release" clears the instance
    }

    void CInputHandler::Init()
    {
        if ( CInputHandler::s_instance )
        {
            ENGINE_CORE_WARN( "Attempting to initiliaze input-handler twice" );
            return;
        }

        CInputHandler::s_instance = new CInputHandler();
    }

    void CInputHandler::Release()
    {
        ENGINE_CORE_ASSERT( CInputHandler::s_instance, "Tried to release input-handler more than once" );

        delete CInputHandler::s_instance;
        CInputHandler::s_instance = NULL;
    }

    void CInputHandler::callback_key( int key, int action )
    {
        if ( !CInputHandler::s_instance )
            return;

        CInputHandler::s_instance->m_keys[key] = action;
    }

    void CInputHandler::callback_mouse( int button, int action, double x, double y )
    {
        if ( !CInputHandler::s_instance )
            return;

        CInputHandler::s_instance->m_buttons[button] = action;
        CInputHandler::s_instance->m_cursor.x = x;
        CInputHandler::s_instance->m_cursor.y = y;
    }

    void CInputHandler::callback_mouseMove( double x, double y )
    {
        if ( !CInputHandler::s_instance )
            return;

        CInputHandler::s_instance->m_cursor.x = x;
        CInputHandler::s_instance->m_cursor.y = y;
    }

    void CInputHandler::callback_scroll( double xOff, double yOff )
    {
        if ( !CInputHandler::s_instance )
            return;

        CInputHandler::s_instance->m_scrollOff.x = xOff;
        CInputHandler::s_instance->m_scrollOff.y = yOff;

        CInputHandler::s_instance->m_scrollAccumValue.x += xOff;
        CInputHandler::s_instance->m_scrollAccumValue.y += yOff;
    }

    bool CInputHandler::IsKeyDown( int key )
    {
        ENGINE_CORE_ASSERT( CInputHandler::s_instance, "Must initialize input-handler before using it" );

        return CInputHandler::s_instance->_isKeyDown( key );
    }

    bool CInputHandler::CheckSingleKeyPress( int key )
    {
        ENGINE_CORE_ASSERT( CInputHandler::s_instance, "Must initialize input-handler before using it" );

        return CInputHandler::s_instance->_checkSingleKeyPress( key );
    }

    bool CInputHandler::IsMouseDown( int button )
    {
        ENGINE_CORE_ASSERT( CInputHandler::s_instance, "Must initialize input-handler before using it" );

        return CInputHandler::s_instance->_isMouseDown( button );
    }

    CVec2 CInputHandler::GetCursorPosition()
    {
        ENGINE_CORE_ASSERT( CInputHandler::s_instance, "Must initialize input-handler before using it" );

        return CInputHandler::s_instance->_getCursorPosition();
    }

    float CInputHandler::GetScrollOffX()
    {
        ENGINE_CORE_ASSERT( CInputHandler::s_instance, "Must initialize input-handler before using it" );

        return CInputHandler::s_instance->_getScrollOffX();
    }

    float CInputHandler::GetScrollOffY()
    {
        ENGINE_CORE_ASSERT( CInputHandler::s_instance, "Must initialize input-handler before using it" );

        return CInputHandler::s_instance->_getScrollOffY();
    }

    float CInputHandler::GetScrollAccumValueX()
    {
        ENGINE_CORE_ASSERT( CInputHandler::s_instance, "Must initialize input-handler before using it" );

        return CInputHandler::s_instance->_getScrollAccumValueX();
    }

    float CInputHandler::GetScrollAccumValueY()
    {
        ENGINE_CORE_ASSERT( CInputHandler::s_instance, "Must initialize input-handler before using it" );

        return CInputHandler::s_instance->_getScrollAccumValueY();
    }

    bool CInputHandler::_isKeyDown( int key )
    {
        if ( key < 0 || key >= ENGINE_MAX_KEYS )
        {
            ENGINE_CORE_ERROR( "Wrong key requested for input checking: {0}", key );
            return false;
        }

        return m_keys[key] == ENGINE_KEY_PRESSED ||
               m_keys[key] == ENGINE_KEY_REPEAT;
    }

    bool CInputHandler::_checkSingleKeyPress( int key )
    {
        if ( !_isKeyDown( key ) )
        {
            m_singleKeys[key] = ENGINE_KEY_RELEASED;
            return false;
        }

        // Xor between the saved single state and the handler state
        bool _singleState = ( m_singleKeys[key] == ENGINE_KEY_PRESSED );
        bool _handlerState = _isKeyDown( key );
        bool _res = ( _singleState && !_handlerState ) ||
                    ( !_singleState && _handlerState );

        // update the state of the singlekey
        m_singleKeys[key] = ( _handlerState ) ? ENGINE_KEY_PRESSED : ENGINE_KEY_RELEASED;

        // finally, return the previous result
        return _res;
    }

    bool CInputHandler::_isMouseDown( int button )
    {
        if ( button < 0 || button >= ENGINE_MAX_BUTTONS )
        {
            ENGINE_CORE_ERROR( "Wrong button requested for input checking: {0}", button );
            return false;
        }

        return m_buttons[button] == ENGINE_MOUSE_BUTTON_PRESSED;
    }

}
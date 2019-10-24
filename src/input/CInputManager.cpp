
#include <input/CInputManager.h>

namespace engine
{

    CInputManager* CInputManager::s_instance = NULL;

    CInputManager::CInputManager()
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

    CInputManager::~CInputManager()
    {
        // nothing for now, as "Release" clears the instance
    }

    void CInputManager::Init()
    {
        if ( CInputManager::s_instance )
        {
            ENGINE_CORE_WARN( "Attempting to initiliaze input-handler twice" );
            return;
        }

        CInputManager::s_instance = new CInputManager();
    }

    void CInputManager::Release()
    {
        ENGINE_CORE_ASSERT( CInputManager::s_instance, "Tried to release input-handler more than once" );

        delete CInputManager::s_instance;
        CInputManager::s_instance = NULL;
    }

    void CInputManager::Callback_key( int key, int action )
    {
        if ( !CInputManager::s_instance )
            return;

        CInputManager::s_instance->m_keys[key] = action;
    }

    void CInputManager::Callback_mouse( int button, int action, double x, double y )
    {
        if ( !CInputManager::s_instance )
            return;

        CInputManager::s_instance->m_buttons[button] = action;
        CInputManager::s_instance->m_cursor.x = x;
        CInputManager::s_instance->m_cursor.y = y;
    }

    void CInputManager::Callback_mouseMove( double x, double y )
    {
        if ( !CInputManager::s_instance )
            return;

        CInputManager::s_instance->m_cursor.x = x;
        CInputManager::s_instance->m_cursor.y = y;
    }

    void CInputManager::Callback_scroll( double xOff, double yOff )
    {
        if ( !CInputManager::s_instance )
            return;

        CInputManager::s_instance->m_scrollOff.x = xOff;
        CInputManager::s_instance->m_scrollOff.y = yOff;

        CInputManager::s_instance->m_scrollAccumValue.x += xOff;
        CInputManager::s_instance->m_scrollAccumValue.y += yOff;
    }

    bool CInputManager::IsKeyDown( int key )
    {
        ENGINE_CORE_ASSERT( CInputManager::s_instance, "Must initialize input-handler before using it" );

        return CInputManager::s_instance->_isKeyDown( key );
    }

    bool CInputManager::CheckSingleKeyPress( int key )
    {
        ENGINE_CORE_ASSERT( CInputManager::s_instance, "Must initialize input-handler before using it" );

        return CInputManager::s_instance->_checkSingleKeyPress( key );
    }

    bool CInputManager::IsMouseDown( int button )
    {
        ENGINE_CORE_ASSERT( CInputManager::s_instance, "Must initialize input-handler before using it" );

        return CInputManager::s_instance->_isMouseDown( button );
    }

    CVec2 CInputManager::GetCursorPosition()
    {
        ENGINE_CORE_ASSERT( CInputManager::s_instance, "Must initialize input-handler before using it" );

        return CInputManager::s_instance->_getCursorPosition();
    }

    float CInputManager::GetScrollOffX()
    {
        ENGINE_CORE_ASSERT( CInputManager::s_instance, "Must initialize input-handler before using it" );

        return CInputManager::s_instance->_getScrollOffX();
    }

    float CInputManager::GetScrollOffY()
    {
        ENGINE_CORE_ASSERT( CInputManager::s_instance, "Must initialize input-handler before using it" );

        return CInputManager::s_instance->_getScrollOffY();
    }

    float CInputManager::GetScrollAccumValueX()
    {
        ENGINE_CORE_ASSERT( CInputManager::s_instance, "Must initialize input-handler before using it" );

        return CInputManager::s_instance->_getScrollAccumValueX();
    }

    float CInputManager::GetScrollAccumValueY()
    {
        ENGINE_CORE_ASSERT( CInputManager::s_instance, "Must initialize input-handler before using it" );

        return CInputManager::s_instance->_getScrollAccumValueY();
    }

    bool CInputManager::_isKeyDown( int key )
    {
        if ( key < 0 || key >= ENGINE_MAX_KEYS )
        {
            ENGINE_CORE_ERROR( "Wrong key requested for input checking: {0}", key );
            return false;
        }

        return m_keys[key] == ENGINE_KEY_PRESSED ||
               m_keys[key] == ENGINE_KEY_REPEAT;
    }

    bool CInputManager::_checkSingleKeyPress( int key )
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

    bool CInputManager::_isMouseDown( int button )
    {
        if ( button < 0 || button >= ENGINE_MAX_BUTTONS )
        {
            ENGINE_CORE_ERROR( "Wrong button requested for input checking: {0}", button );
            return false;
        }

        return m_buttons[button] == ENGINE_MOUSE_BUTTON_PRESSED;
    }

}
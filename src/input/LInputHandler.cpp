
#include <input/LInputHandler.h>

#include <app/COpenGLApp.h>

namespace engine
{

    LInputHandler* LInputHandler::s_instance = NULL;

    LInputHandler::LInputHandler()
    {
        for ( int q = 0; q < ENGINE_MAX_KEYS; q++ )
        {
            m_keys[q] = 0;
            m_singleKeys[q] = false;
        }

        for ( int q = 0; q < ENGINE_MAX_BUTTONS; q++ )
            m_buttons[q] = 0;

        m_mouseX = 0.0f;
        m_mouseY = 0.0f;

        m_scrollOffX = 0.0f;
        m_scrollOffY = 0.0f;
        m_scrollAccumValueX = 0.0f;
        m_scrollAccumValueY = 0.0f;
    }

    LInputHandler::~LInputHandler()
    {
        // nothing for now, as "Release" clears the instance
    }

    void LInputHandler::Init()
    {
        if ( LInputHandler::s_instance )
        {
            ENGINE_CORE_WARN( "Attempting to initiliaze input handle twice" );
            return;
        }

        LInputHandler::s_instance = new LInputHandler();
    }

    void LInputHandler::Release()
    {
        if ( LInputHandler::s_instance )
            delete LInputHandler::s_instance;

        LInputHandler::s_instance = NULL;
    }

    LInputHandler* LInputHandler::GetInstance()
    {
        ENGINE_CORE_ASSERT( LInputHandler::s_instance, "Must have initialized the input handler by now" );

        return LInputHandler::s_instance;
    }

    void LInputHandler::callback_key( int key, int action )
    {
        if ( !LInputHandler::s_instance )
            return;

        LInputHandler::s_instance->m_keys[key] = action;
    }

    void LInputHandler::callback_mouse( int button, int action, double x, double y )
    {
        if ( !LInputHandler::s_instance )
            return;

        LInputHandler::s_instance->m_buttons[button] = action;
        LInputHandler::s_instance->m_mouseX = x;
        LInputHandler::s_instance->m_mouseY = y;
    }

    void LInputHandler::callback_mouseMove( double x, double y )
    {
        if ( !LInputHandler::s_instance )
            return;

        LInputHandler::s_instance->m_mouseX = x;
        LInputHandler::s_instance->m_mouseY = y;
    }

    void LInputHandler::callback_scroll( double xOff, double yOff )
    {
        if ( !LInputHandler::s_instance )
            return;

        LInputHandler::s_instance->m_scrollOffX = xOff;
        LInputHandler::s_instance->m_scrollOffY = yOff;

        LInputHandler::s_instance->m_scrollAccumValueX += xOff;
        LInputHandler::s_instance->m_scrollAccumValueY += yOff;
    }

    bool LInputHandler::isKeyDown( int key )
    {
        if ( key < 0 || key >= ENGINE_MAX_KEYS )
        {
            std::cout << "LInputHandler::isKeyDown> wrong key requested: " << key << std::endl;
            return false;
        }

        return m_keys[key] == ENGINE_KEY_PRESSED ||
               m_keys[key] == ENGINE_KEY_REPEAT;
    }

    bool LInputHandler::checkSingleKeyPress( int key )
    {
        if ( !isKeyDown( key ) )
        {
            m_singleKeys[key] = false;
            return false;
        }

        // Xor between the saved single state and the handler state
        bool _singleState = m_singleKeys[key];
        bool _handlerState = isKeyDown( key );
        bool _res = ( _singleState && !_handlerState ) ||
                    ( !_singleState && _handlerState );

        // update the state of the singlekey
        m_singleKeys[key] = _handlerState;
        // and return the previous result
        return _res;
    }

    bool LInputHandler::isMouseDown( int button )
    {
        if ( button < 0 || button >= ENGINE_MAX_BUTTONS )
        {
            std::cout << "LInputHandler::isMouseDown> wrong button requested: " << button << std::endl;
            return false;
        }

        return m_buttons[button] == ENGINE_MOUSE_BUTTON_PRESSED;
    }

    CVec2 LInputHandler::getCursorPosition()
    {
        return CVec2( m_mouseX, m_mouseY );
    }

    float LInputHandler::getScrollOffX()
    {
        return m_scrollOffX;
    }

    float LInputHandler::getScrollOffY()
    {
        return m_scrollOffY;
    }

    float LInputHandler::getScrollAccumValueX()
    {
        return m_scrollAccumValueX;
    }

    float LInputHandler::getScrollAccumValueY()
    {
        return m_scrollAccumValueY;
    }

}
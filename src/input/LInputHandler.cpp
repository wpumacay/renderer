

#include <input/LInputHandler.h>



namespace engine
{

	LInputHandler* LInputHandler::_INSTANCE = NULL;

	LInputHandler::LInputHandler()
	{
		for ( int q = 0; q < L_MAX_KEYS; q++ )
		{
			m_keys[q] = GLFW_RELEASE;
            m_singleKeys[q] = false;
		}

		for ( int q = 0; q < L_MAX_BUTTONS; q++ )
		{
			m_buttons[q] = GLFW_RELEASE;
		}

		m_mouseX = 0.0f;
		m_mouseY = 0.0f;
	}

	LInputHandler::~LInputHandler()
	{
		if ( m_window != NULL )
		{
			m_window->registerKeyCallback( NULL );
			m_window->registerMouseCallback( NULL );
		}
	}

	LInputHandler* LInputHandler::GetInstance()
	{
		if ( LInputHandler::_INSTANCE == NULL )
		{
            LInputHandler::_INSTANCE = new LInputHandler();

            // get global window reference
            auto _window = LWindow::GetInstance();
            // register callbacks for window
            _window->registerKeyCallback( LInputHandler::callback_key );
            _window->registerMouseCallback( LInputHandler::callback_mouse );
            _window->registerMouseMoveCallback( LInputHandler::callback_mouseMove );
		}

        return LInputHandler::_INSTANCE;
	}

	void LInputHandler::Release()
	{
		if ( LInputHandler::_INSTANCE != NULL )
		{
			delete LInputHandler::_INSTANCE;
			LInputHandler::_INSTANCE = NULL;	
		}
	}


	void LInputHandler::callback_key( int key, int action )
	{
		if ( LInputHandler::_INSTANCE == NULL )
		{
			return;
		}

        auto _userCallbacks = LInputHandler::_INSTANCE->getKeyboardUserCallbacks();

        for ( auto _callback : _userCallbacks )
        {
            _callback( key, action );
        }

        // std::cout << "key: " << key << std::endl;
        // std::cout << "action: " << action << std::endl;

		LInputHandler::_INSTANCE->m_keys[key] = action;
	}

	void LInputHandler::callback_mouse( int button, int action, double x, double y )
	{
		if ( LInputHandler::_INSTANCE == NULL )
		{
			return;
		}

		LInputHandler::_INSTANCE->m_buttons[button] = action;
		LInputHandler::_INSTANCE->m_mouseX = x;
		LInputHandler::_INSTANCE->m_mouseY = y;
	}

    void LInputHandler::callback_mouseMove( double x, double y )
    {
        if ( LInputHandler::_INSTANCE == NULL )
        {
            return;
        }

        LInputHandler::_INSTANCE->m_mouseX = x;
        LInputHandler::_INSTANCE->m_mouseY = y;
    }

	bool LInputHandler::isKeyDown( int key )
	{
		if ( key < 0 || key >= L_MAX_KEYS )
		{
			std::cout << "LInputHandler::isKeyDown> wrong key requested: " << key << std::endl;
			return false;
		}

		return m_keys[key] == GLFW_PRESS ||
			   m_keys[key] == GLFW_REPEAT;
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
		if ( button < 0 || button >= L_MAX_BUTTONS )
		{
			std::cout << "LInputHandler::isMouseDown> wrong button requested: " << button << std::endl;
			return false;
		}

		return m_buttons[button] == GLFW_PRESS;
	}

	LVec2 LInputHandler::getCursorPosition()
	{
		return LVec2( m_mouseX, m_mouseY );
	}

}
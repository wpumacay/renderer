

#pragma once

#include <iostream>
#include <vector>

#include <LCommon.h>
#include <LWindow.h>

using namespace std;

namespace engine
{

	class LInputHandler
	{
		private :

		LWindow* m_window;

		int m_keys[L_MAX_KEYS];
        bool m_singleKeys[L_MAX_KEYS];
		int m_buttons[L_MAX_BUTTONS];
		float m_mouseX;
		float m_mouseY;

        vector< FnPtr_keyboard_callback > m_userCallbacks;

        static LInputHandler* _INSTANCE;
		LInputHandler();

		public :

		static LInputHandler* GetInstance();
		static void Release();
        ~LInputHandler();

		static void callback_key( int key, int action );
        static void callback_keyUp( int key, int action );
		static void callback_mouse( int button, int action, double x, double y );
        static void callback_mouseMove( double x, double y );

        void addKeyboardUserCallback( FnPtr_keyboard_callback userCallback ) { m_userCallbacks.push_back( userCallback ); }
        vector< FnPtr_keyboard_callback > getKeyboardUserCallbacks() { return m_userCallbacks; }

		bool isKeyDown( int key );
        bool checkSingleKeyPress( int key );
		bool isMouseDown( int button );
		LVec2 getCursorPosition();
	};



}




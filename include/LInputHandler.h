

#pragma once

#include "LWindow.h"
#include <iostream>
#include <vector>

using namespace std;

namespace engine
{

	class LInputHandler
	{
		private :

		LWindow* m_window;

		LInputHandler();

		int m_keys[L_MAX_KEYS];
		int m_buttons[L_MAX_BUTTONS];
		float m_mouseX;
		float m_mouseY;

        vector< FnPtr_keyboard_callback > m_userCallbacks;

		public :

		~LInputHandler();

		static LInputHandler* INSTANCE;
		static void create( LWindow* pWindow );
		static void release();

		static void callback_key( int key, int action );
        static void callback_keyUp( int key, int action );
		static void callback_mouse( int button, int action, double x, double y );
        static void callback_mouseMove( double x, double y );

        void addUserCallback( FnPtr_keyboard_callback userCallback ) { m_userCallbacks.push_back( userCallback ); }
        vector< FnPtr_keyboard_callback > getUserCallbacks() { return m_userCallbacks; }

		bool isKeyPressed( int key );
		bool isButtonPressed( int button );
		void getMousePosition( float& x, float& y );

	};



}




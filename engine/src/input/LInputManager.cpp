

#include "../../include/core/LInputManager.h"


namespace engine { namespace core {

    LInputManager* LInputManager::instance = NULL;

    LInputManager::LInputManager()
    {
        for ( int q = 0; q < L_MAX_KEYS; q++ )
        {
            m_keysStates[q] = 0;
        }

        for ( int q = 0; q < L_MAX_BUTTONS; q++ )
        {
            m_buttonsStates[q] = 0;
        }
    }

    void LInputManager::create()
    {
        if ( LInputManager::instance != NULL )
        {
            delete LInputManager::instance;
        }

        LInputManager::instance = new LInputManager();
    }

    LInputManager::~LInputManager()
    {
        // Release some stuff, nothing useful yet
    }

    void LInputManager::destroy()
    {
        if ( LInputManager::instance != NULL )
        {
            delete LInputManager::instance;
            LInputManager::instance = NULL;
        }
        else
        {
            ENGINE_ERROR( "LInputManager::destroy> trying to 
                          free a non-existing object" );
        }
    }


    void LInputManager::update()
    {

    }

    /************************************************
    * Input callbacks
    *************************************************/

    void LInputManager::onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
                                    int pAction, int pMode )
    {
        if ( pAction == GLFW_PRESS )
        {
            LInputManager::instance->m_keysStates[pKey] = 1;
        }
        else
        {
            LInputManager::instance->m_keysStates[pKey] = 0;
        }
    }

    void LInputManager::onMouseEvent( GLFWwindow* pWindow, int pButton, 
                                      int pAction, int pMods )
    {
        double evx, evy;
        glfwGetCursorPos( pWindow, &evx, &evy );

        LInputManager::instance->m_cursorX = x;
        LInputManager::instance->m_cursorY = y;

        if ( pAction == GLFW_PRESS )
        {
            LInputManager::instance->m_buttonsStates[pButton] = 1;
        }
        else
        {
            LInputManager::instance->m_buttonsStates[pButton] = 0;
        }
    }

    void LInputManager::onCursorPosEvent( GLFWwindow* pWindow, double x, double y )
    {
        LInputManager::instance->m_cursorX = x;
        LInputManager::instance->m_cursorY = y;
    }

    void LInputManager::onScrollEvent( GLFWwindow* pWindow, double xOff, double yOff )
    {

    }


} }
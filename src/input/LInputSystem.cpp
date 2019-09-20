
#include <input/LInputSystem.h>

namespace engine { namespace InputSystem {

    void init()
    {
        LInputHandler::Init();
    }

    bool isKeyDown( int key )
    {
        return LInputHandler::GetInstance()->isKeyDown( key );
    }

    bool checkSingleKeyPress( int key )
    {
        return LInputHandler::GetInstance()->checkSingleKeyPress( key );
    }

    bool isMouseDown( int button )
    {
        return LInputHandler::GetInstance()->isMouseDown( button );
    }

    CVec2 getCursorPosition()
    {
        return LInputHandler::GetInstance()->getCursorPosition();
    }

    float getScrollOffX()
    {
        return LInputHandler::GetInstance()->getScrollOffX();
    }

    float getScrollOffY()
    {
        return LInputHandler::GetInstance()->getScrollOffY();
    }

    float getScrollAccumValueX()
    {
        return LInputHandler::GetInstance()->getScrollAccumValueX();
    }

    float getScrollAccumValueY()
    {
        return LInputHandler::GetInstance()->getScrollAccumValueY();
    }

    void release()
    {
        LInputHandler::Release();
    }

}}

#include <input/LInputSystem.h>

namespace engine { namespace InputSystem {

    void init()
    {
        LInputHandler::GetInstance();
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

    LVec2 getCursorPosition()
    {
        return LInputHandler::GetInstance()->getCursorPosition();
    }

    void release()
    {
        LInputHandler::Release();
    }

    

}}
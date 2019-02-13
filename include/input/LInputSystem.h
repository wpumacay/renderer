
#pragma once

#include <LCommon.h>
#include <input/LInputHandler.h>

namespace engine { namespace InputSystem {

    void init();

    // input queries to the internal handler
    bool isKeyDown( int key );
    bool checkSingleKeyPress( int key );
    bool isMouseDown( int button );
    LVec2 getCursorPosition();

    void release();

}}
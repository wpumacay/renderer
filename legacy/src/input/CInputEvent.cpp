
#include <input/CInputEvent.h>

namespace engine
{

    std::string toString( const eEventType& type )
    {
        if ( type == eEventType::KEY_PRESSED ) return "key-pressed";
        if ( type == eEventType::KEY_RELEASED ) return "key-released";
        if ( type == eEventType::MOUSE_PRESSED ) return "mouse-pressed";
        if ( type == eEventType::MOUSE_RELEASED ) return "mouse-released";
        if ( type == eEventType::MOUSE_MOVED ) return "mouse-moved";
        if ( type == eEventType::MOUSE_SCROLLED ) return "mouse-scrolled";
        if ( type == eEventType::RESIZE ) return "resized";

        ENGINE_CORE_ASSERT( false, "Invalid eEventType enum given" );

        return "undefined";
    }

    CInputEvent::CInputEvent( const std::string& name )
    {
        m_name = name;
        m_type = eEventType::NONE;
    }

    std::string toString( const CInputEvent& event )
    {
        std::string _strRep;
        if ( event.type() == eEventType::KEY_PRESSED )
        {
            _strRep += "type        : key-pressed\n\r";
            _strRep += "keyCode     : " + std::to_string( dynamic_cast< const CKeyPressedEvent& >( event ).keyCode() ) + "\n\r";
        }
        else if ( event.type() == eEventType::KEY_RELEASED )
        {
            _strRep += "type        : key-released\n\r";
            _strRep += "keyCode     : " + std::to_string( dynamic_cast< const CKeyReleasedEvent& >( event ).keyCode() ) + "\n\r";
        }
        else if ( event.type() == eEventType::MOUSE_PRESSED )
        {
            _strRep += "type        : mouse-pressed\n\r";
            _strRep += "button      : " + std::to_string( dynamic_cast< const CMousePressedEvent& >( event ).button() ) + "\n\r";
            _strRep += "position    : " + engine::toString( dynamic_cast< const CMousePressedEvent& >( event ).position() ) + "\n\r";
        }
        else if ( event.type() == eEventType::MOUSE_RELEASED )
        {
            _strRep += "type        : mouse-released\n\r";
            _strRep += "button      : " + std::to_string( dynamic_cast< const CMouseReleasedEvent& >( event ).button() ) + "\n\r";
            _strRep += "position    : " + engine::toString( dynamic_cast< const CMouseReleasedEvent& >( event ).position() ) + "\n\r";
        }
        else if ( event.type() == eEventType::MOUSE_MOVED )
        {
            _strRep += "type        : mouse-moved\n\r";
            _strRep += "position    : " + engine::toString( dynamic_cast< const CMouseMoveEvent& >( event ).position() ) + "\n\r";
        }
        else if ( event.type() == eEventType::MOUSE_SCROLLED )
        {
            _strRep += "type        : mouse-scrolled\n\r";
            _strRep += "(xOff,yOff) : " + engine::toString( dynamic_cast< const CMouseScrollEvent& >( event ).scroll() ) + "\n\r";
        }
        else if ( event.type() == eEventType::RESIZE )
        {
            _strRep += "type        : resized\n\r";
            _strRep += "width       : " + std::to_string( dynamic_cast< const CResizeEvent& >( event ).width() ) + "\n\r";
            _strRep += "height      : " + std::to_string( dynamic_cast< const CResizeEvent& >( event ).height() ) + "\n\r";
        }

        return _strRep;
    }
}

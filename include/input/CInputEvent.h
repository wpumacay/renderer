#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <input/CInputKey.h>
#include <input/CInputMouse.h>

namespace engine
{

    enum class eEventType
    {
        NONE = 0,
        KEY_PRESSED, 
        KEY_RELEASED,
        MOUSE_PRESSED, 
        MOUSE_RELEASED, 
        MOUSE_MOVED, 
        MOUSE_SCROLLED,
        RESIZE
    };

    std::string toString( const eEventType& type );

    class CInputEvent
    {

    public :

        CInputEvent( const std::string& name );
        virtual ~CInputEvent() {}

        eEventType type() const { return m_type; }
        std::string name() const { return m_name; }

    protected :

        eEventType  m_type;
        std::string m_name;

    };

    std::string toString( const CInputEvent& event );

    class CKeyEvent : public CInputEvent
    {

    public :

        CKeyEvent( const std::string& name,
                   int keyCode )
            : CInputEvent( name ), m_keyCode( keyCode ) {}

        int keyCode() const { return m_keyCode; }

    private :

        int m_keyCode;

    };

    class CKeyPressedEvent : public CKeyEvent
    {

    public :

        CKeyPressedEvent( const std::string& name,
                          int keyCode )
            : CKeyEvent( name, keyCode )
        {
            m_type = eEventType::KEY_PRESSED;
        }
    };

    class CKeyReleasedEvent : public CKeyEvent
    {

    public :

        CKeyReleasedEvent( const std::string& name,
                          int keyCode )
            : CKeyEvent( name, keyCode )
        {
            m_type = eEventType::KEY_RELEASED;
        }
    };

    class CMouseEvent : public CInputEvent
    {

    public :

        CMouseEvent( const std::string& name,
                     int button, CVec2 position )
            : CInputEvent( name ), 
                m_button( button ), 
                m_position( position ) {}

        int button() const { return m_button; }
        CVec2 position() const { return m_position; }

    private :

        int     m_button;
        CVec2   m_position;
    };

    class CMousePressedEvent : public CMouseEvent
    {

    public :

        CMousePressedEvent( const std::string& name,
                            int button, CVec2 position )
            : CMouseEvent( name, button, position ) 
        {
            m_type = eEventType::MOUSE_PRESSED;
        }

    };

    class CMouseReleasedEvent : public CMouseEvent
    {

    public :

        CMouseReleasedEvent( const std::string& name,
                             int button, CVec2 position )
            : CMouseEvent( name, button, position )
        {
            m_type = eEventType::MOUSE_RELEASED;
        }

    };

    class CMouseMoveEvent : public CMouseEvent
    {

    public :

        CMouseMoveEvent( const std::string& name,
                         int button, CVec2 position )
            : CMouseEvent( name, button, position )
        {
            m_type = eEventType::MOUSE_MOVED;
        }

    };

    class CMouseScrollEvent : public CInputEvent
    {

    public :

        CMouseScrollEvent( const std::string& name,
                           CVec2 scroll )
            : CInputEvent( name ),
                m_scroll( scroll )
        {
            m_type = eEventType::MOUSE_SCROLLED;
        }

        CVec2 scroll() const { return m_scroll; }

    private :

        CVec2 m_scroll;

    };

    class CResizeEvent : public CInputEvent
    {

    public :

        CResizeEvent( const std::string& name,
                      int width, int height )
            : CInputEvent( name ), 
                m_width( width ), 
                m_height( height )
        {
            m_type = eEventType::RESIZE;
        }

        int width() const { return m_width; }
        int height() const { return m_height; }

    private :

        int m_width;
        int m_height;

    };
}
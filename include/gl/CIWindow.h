#pragma once

#include <CCommon.h>
#include <CMath.h>

#include <input/CInputKey.h>
#include <input/CInputMouse.h>
#include <input/CInputCallbacks.h>

namespace engine
{
    struct CWindowProps
    {
        int width;
        int height;
        std::string title;
        CVec4 clearColor;
        bool resizable;

        int gl_api_version_major;
        int gl_api_version_minor;

        FnPtr_keyboard_callback callbackKey;
        FnPtr_mouse_callback callbackMouse;
        FnPtr_mousemove_callback callbackMouseMove;
        FnPtr_scroll_callback callbackScroll;
        FnPtr_resize_callback callbackResize;

        CWindowProps()
        {
            width = 1024;
            height = 768;
            title = "OpenGL-window";
            clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
            resizable = false;

            gl_api_version_major = 3;
            gl_api_version_minor = 3;
        }
    };

    enum eWindowType
    {
        NONE = 0,
        GLFW,
        HEADLESS_EGL
    };

    std::string toString( const eWindowType& type );

    class CIWindow
    {
    public :

        CIWindow( const CWindowProps& properties ) : m_properties( properties ) {}
        virtual ~CIWindow() {}

        void registerKeyCallback( FnPtr_keyboard_callback callback );
        void registerMouseCallback( FnPtr_mouse_callback callback );
        void registerMouseMoveCallback( FnPtr_mousemove_callback callback );
        void registerScrollCallback( FnPtr_scroll_callback callback );
        void registerResizeCallback( FnPtr_resize_callback callback );

        void enableCursor();
        void disableCursor();

        void begin();
        void end();

        bool active();
        void requestClose();

        int width() const { return m_properties.width; }
        int height() const { return m_properties.height; }
        float aspect() const { return ((float) m_properties.width) / m_properties.height; }
        std::string title() const { return m_properties.title; }
        eWindowType type() const { return m_type; }
        
        CWindowProps properties() { return m_properties; }

    protected :

        virtual void _enableCursorInternal() = 0;
        virtual void _disableCursorInternal() = 0;
        virtual void _beginInternal() = 0;
        virtual void _endInternal() = 0;
        virtual bool _activeInternal() = 0;
        virtual void _requestCloseInternal() = 0;

    protected :

        CWindowProps m_properties;
        eWindowType m_type;
    };

}
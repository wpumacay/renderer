#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <input/CInputEvent.h>

#include <imgui.h>

namespace engine
{

    class CImGuiLayer
    {

    public :

        CImGuiLayer( const std::string& name );
        virtual ~CImGuiLayer() {}

        virtual void update();
        virtual void render();
        virtual bool onEvent( const CInputEvent& event );

        void setActive( bool active ) { m_active = active; }
        bool active() const { return m_active; }
        std::string name() const { return m_name; }

    protected :

        bool        m_active;
        std::string m_name;

    };

}
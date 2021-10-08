
#include <gui/CImGuiLayer.h>


namespace engine
{

    CImGuiLayer::CImGuiLayer( const std::string& name )
    {
        m_name = name;
        m_active = true;
    }

    void CImGuiLayer::update()
    {
        // override this
    }

    void CImGuiLayer::render()
    {
        // override this
    }

    bool CImGuiLayer::onEvent( const CInputEvent& event )
    {
        // override this (true: handled|consumed, false: otherwise)
        return false;
    }

}

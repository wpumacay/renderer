
#include <utils/CImguiUiDemo.h>

namespace engine
{

    CImguiUiDemo::CImguiUiDemo( COpenGLContext* context )
        : CImguiUi( context )
    {

    }

    CImguiUiDemo::~CImguiUiDemo()
    {

    }

    void CImguiUiDemo::_initInternal()
    {

    }

    void CImguiUiDemo::_renderInternal()
    {
        ImGui::Begin( "Demo-ui" );

        ImGui::End();
    }

}
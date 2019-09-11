
#include <utils/CImguiUi.h>

namespace engine
{

    CImguiUi::CImguiUi()
    {

    }

    CImguiUi::~CImguiUi()
    {
        
    }

    void CImguiUi::init()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& _io = ImGui::GetIO(); (void) _io;
        _io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui_ImplGlfw_InitForOpenGL( m_windowHandle, true );
    #ifdef __APPLE__
        ImGui_ImplOpenGL3_Init( "#version 150" );
    #else
        ImGui_ImplOpenGL3_Init( "#version 130" );
    #endif
        ImGui::StyleColorsDark();

        _initInternal();
    }

    void CImguiUi::render()
    {
        // prepare frame

        _renderInternal();

        // end frame
    }

}
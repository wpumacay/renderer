
#include <utils/CImguiUi.h>

namespace engine
{

    CImguiUi::CImguiUi( COpenGLContext* context )
    {
        m_windowHandle = context->window();
    }

    CImguiUi::~CImguiUi()
    {
        m_windowHandle = nullptr;
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

        // user can initialize extra stuff here
        _initInternal();
    }

    void CImguiUi::render()
    {
        // prepare frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // render user-specific Ui
        _renderInternal();

        // end frame
        ImGui::Render();
        int _ww, _wh;
        glfwGetFramebufferSize( m_windowHandle, &_ww, &_wh );
        glViewport( 0, 0, _ww, _wh );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
    }

}
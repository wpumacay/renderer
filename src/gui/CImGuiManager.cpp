
#include <gui/CImGuiManager.h>


namespace engine
{

    CImGuiManager::CImGuiManager( GLFWwindow* glfwWindowPtr )
    {
        m_active = true;
        m_glfwWindowPtr = glfwWindowPtr;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& _io = ImGui::GetIO(); (void) _io;
        _io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        _io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui_ImplGlfw_InitForOpenGL( m_glfwWindowPtr, true ); // true: installing callbacks
    #ifdef __APPLE__
        ImGui_ImplOpenGL3_Init( "#version 150" );
    #else
        ImGui_ImplOpenGL3_Init( "#version 130" );
    #endif
        ImGui::StyleColorsDark();
    }

    CImGuiManager::~CImGuiManager()
    {
        m_glfwWindowPtr = nullptr;

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void CImGuiManager::begin()
    {
        if ( !m_active )
            return;

        // prepare imgui resources for rendering
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void CImGuiManager::render()
    {
        if ( !m_active )
            return;

        // render all commands submitted up to this point
        ImGui::Render();
        int _ww, _wh;
        glfwGetFramebufferSize( m_glfwWindowPtr, &_ww, &_wh );
        glViewport( 0, 0, _ww, _wh );
        ImGui_ImplOpenGL3_RenderDrawData( ImGui::GetDrawData() );
    }

}
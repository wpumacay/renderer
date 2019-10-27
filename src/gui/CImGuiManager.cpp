
#include <gui/CImGuiManager.h>


namespace engine
{

    CImGuiManager::CImGuiManager( GLFWwindow* glfwWindowPtr )
    {
        m_active = true;
        m_useDockingSpace = false;
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

        m_dockSpaceFlags = ImGuiDockNodeFlags_None;
        m_dockSpaceWindowFlags = ImGuiWindowFlags_None;

        // base configuration (no passthrough)
        m_dockSpaceFlags |= ImGuiDockNodeFlags_NoDockingInCentralNode;
        m_dockSpaceWindowFlags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        m_dockSpaceWindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
        m_dockSpaceWindowFlags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        m_dockSpaceWindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
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

        //// ENGINE_CORE_TRACE( "dockspace? {0}", m_useDockingSpace );
        //// ENGINE_CORE_TRACE( "transparent? {0}", ( m_dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode ) != 0 );
        //// ENGINE_CORE_TRACE( "dockable-central? {0}", ( m_dockSpaceFlags & ImGuiDockNodeFlags_NoDockingInCentralNode ) != 0 );

        if ( m_useDockingSpace )
            _configureDockingSpace();
    }

    void CImGuiManager::_configureDockingSpace()
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos( viewport->Pos );
        ImGui::SetNextWindowSize( viewport->Size );
        ImGui::SetNextWindowViewport( viewport->ID );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );

        if ( m_dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode )
            m_dockSpaceWindowFlags |= ImGuiWindowFlags_NoBackground;
        else
            m_dockSpaceWindowFlags &= ~ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
        ImGui::Begin( "DockSpace", NULL, m_dockSpaceWindowFlags );
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiID _dockSpaceID = ImGui::GetID( "DockingSpaceID" );
        ImGui::DockSpace( _dockSpaceID, ImVec2( 0.0f, 0.0f ), m_dockSpaceFlags );

        if ( ImGui::BeginMenuBar() )
        {
            if ( ImGui::BeginMenu( "Docking" ) )
            {
                if ( ImGui::MenuItem( "Option: allow transparent docking space", "", ( m_dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode ) != 0 ) )
                    m_dockSpaceFlags ^= ImGuiDockNodeFlags_PassthruCentralNode;

                if ( ImGui::MenuItem( "Option: allow docking in central node", "", ( m_dockSpaceFlags & ImGuiDockNodeFlags_NoDockingInCentralNode ) == 0 ) )
                    m_dockSpaceFlags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    void CImGuiManager::setDockingSpacePassthrough( bool enabled )
    {
        if ( enabled )
            m_dockSpaceFlags |= ImGuiDockNodeFlags_PassthruCentralNode;
        else
            m_dockSpaceFlags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    bool CImGuiManager::usesDockingPassthrough() const
    {
        return ( m_dockSpaceFlags & ImGuiDockNodeFlags_PassthruCentralNode ) != 0;
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
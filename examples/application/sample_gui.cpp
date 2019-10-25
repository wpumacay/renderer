
#include <CEngine.h>

class DockSpaceLayer : public engine::CImGuiLayer
{

public : 
    
    DockSpaceLayer( const std::string& name )
        : engine::CImGuiLayer ( name ) 
    {
        m_dockspaceFlags = ImGuiDockNodeFlags_None;
        m_windowFlags = ImGuiWindowFlags_None;

        m_dockspaceFlags |= ImGuiDockNodeFlags_NoDockingInCentralNode;

        m_windowFlags |= ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        m_windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    void render() override
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos( viewport->Pos );
        ImGui::SetNextWindowSize( viewport->Size );
        ImGui::SetNextWindowViewport( viewport->ID );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowRounding, 0.0f );
        ImGui::PushStyleVar( ImGuiStyleVar_WindowBorderSize, 0.0f );

        ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 0.0f, 0.0f ) );
        ImGui::Begin( "Dockspace-layer", &m_active, m_windowFlags );
        ImGui::PopStyleVar();

        ImGuiID _dockSpaceId = ImGui::GetID( "dockspace_layer" );
        ImGui::DockSpace( _dockSpaceId, ImVec2( 0.0f, 0.0f ), m_dockspaceFlags );

        ImGui::End();
    }

    bool onEvent( const engine::CInputEvent& event ) override
    {
        return false;
    }

private :

    ImGuiWindowFlags    m_windowFlags;
    ImGuiDockNodeFlags  m_dockspaceFlags;

};


class SampleGuiLayer : public engine::CImGuiLayer
{

public :

    SampleGuiLayer( const std::string& name )
        : engine::CImGuiLayer( name ) {}

    void render() override
    {
        ImGui::Begin( "Sample-gui-layer" );

        m_hasFocus = ImGui::IsWindowFocused();
        m_isHovered = ImGui::IsWindowHovered();
        ENGINE_TRACE( "has-focus: {0}", m_hasFocus );
        ENGINE_TRACE( "is-hovered: {0}", m_isHovered );

        ImGui::End();
    }

    bool onEvent( const engine::CInputEvent& event ) override
    {
        ENGINE_INFO( "Event-Information: \n\r{0}", engine::toString( event ) );
        return false;
    }

private :

    bool m_hasFocus;
    bool m_isHovered;
};


int main()
{
    auto _app = new engine::CApplication();
    auto _dock_layer = new DockSpaceLayer( "docking-layer" );
    auto _sample_layer = new SampleGuiLayer( "sample-layer" );
    _app->addGuiLayer( std::unique_ptr< engine::CImGuiLayer >( _dock_layer ) );
    _app->addGuiLayer( std::unique_ptr< engine::CImGuiLayer >( _sample_layer ) );

    while ( _app->active() )
    {
        if ( engine::CInputManager::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _app->update();
        _app->begin();
        _app->render();

        _app->end();
    }

    return 0;
}
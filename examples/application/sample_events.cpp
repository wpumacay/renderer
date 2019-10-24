
#include <CEngine.h>


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
    auto _layer = new SampleGuiLayer( "sample-layer" );
    _app->addGuiLayer( std::unique_ptr< engine::CImGuiLayer >( _layer ) );

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
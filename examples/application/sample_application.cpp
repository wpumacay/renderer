
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

        ImGui::End();
    }

    bool onEvent( const engine::CInputEvent& event ) override
    {
        if ( event.type() == engine::eEventType::MOUSE_PRESSED )
            return m_isHovered;

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

    auto _scene = new engine::CScene();

    auto _pointlight = new engine::CPointLight( "point",
                                                { 0.2f, 0.2f, 0.2f },
                                                { 0.5f, 0.5f, 0.5f },
                                                { 0.8f, 0.8f, 0.8f },
                                                { 0.0f, 3.0f, 0.0f },
                                                1.0f, 0.05f, 0.005f );

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 50.0f;

    auto _orbitCamera = new engine::COrbitCamera( "orbit",
                                                  { 0.0f, 0.0f, 3.0f },
                                                  { 0.0f, 0.0f, 0.0f },
                                                  engine::eAxis::Y,
                                                  _cameraProjData,
                                                  _app->window()->width(),
                                                  _app->window()->height() );

    auto _sphere = engine::CMeshBuilder::createSphere( 1.0f );
    _sphere->position = { 0.0f, 1.0f, 0.0f };

    auto _floor = engine::CMeshBuilder::createPlane( 6.0f, 6.0f, engine::eAxis::Y );

    _scene->addRenderable( std::unique_ptr< engine::CIRenderable >( _sphere ) );
    _scene->addRenderable( std::unique_ptr< engine::CIRenderable >( _floor ) );
    _scene->addLight( std::unique_ptr< engine::CILight >( _pointlight ) );
    _scene->addCamera( std::unique_ptr< engine::CICamera >( _orbitCamera ) );

    _app->setScene( std::unique_ptr< engine::CScene >( _scene ) );

    _app->renderOptions().useShadowMapping = true;
    _app->renderOptions().shadowMapRangeConfig.type = engine::eShadowRangeType::FIXED_USER;
    _app->renderOptions().shadowMapRangeConfig.worldUp = { 0.0f, 1.0f, 0.0f };
    _app->renderOptions().shadowMapRangeConfig.cameraPtr = _orbitCamera;
    _app->renderOptions().shadowMapRangeConfig.pointLightPtr = _pointlight;

    while ( _app->active() )
    {
        if ( engine::CInputManager::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->update();

        _app->begin();
        _app->render();
        _app->end();
    }

    return 0;
}
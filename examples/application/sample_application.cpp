
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
    auto _windowProperties = engine::CWindowProps();
    _windowProperties.width = 1024;
    _windowProperties.height = 768;
    _windowProperties.title = "resizable-application";
    _windowProperties.clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
    _windowProperties.resizable = true;

    auto _app = new engine::CApplication( _windowProperties );
    auto _layer = new SampleGuiLayer( "sample-layer" );
    _app->addGuiLayer( std::unique_ptr< engine::CImGuiLayer >( _layer ) );

    auto _scene = new engine::CScene();

    /* create a simple point-light for the scene **************************************************/
////     auto _pointlight = new engine::CPointLight( "point",
////                                                 { 0.4f, 0.4f, 0.4f },
////                                                 { 0.8f, 0.8f, 0.8f },
////                                                 { 0.8f, 0.8f, 0.8f },
////                                                 { 5.0f, 5.0f, 5.0f },
////                                                 1.0f, 0.0f, 0.0f );
//// 
////     _scene->addLight( std::unique_ptr< engine::CILight >( _pointlight ) );

    auto _dirlight = new engine::CDirectionalLight( "directional",
                                                    { 0.4f, 0.4f, 0.4f },
                                                    { 0.8f, 0.8f, 0.8f },
                                                    { 0.8f, 0.8f, 0.8f },
                                                    { -1.0f, -1.0f, -1.0f } );

    _scene->addLight( std::unique_ptr< engine::CILight >( _dirlight ) );

    /* create a single camera *********************************************************************/
    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.aspect = _app->window()->aspect();

    auto _orbitCamera = new engine::COrbitCamera( "orbit",
                                                  { 0.0f, 0.0f, 3.0f },
                                                  { 0.0f, 0.0f, 0.0f },
                                                  engine::eAxis::Y,
                                                  _cameraProjData,
                                                  _app->window()->width(),
                                                  _app->window()->height() );

    _scene->addCamera( std::unique_ptr< engine::CICamera >( _orbitCamera ) );
    /**********************************************************************************************/

    const int _nWidthSamples = 50;
    const int _nDepthSamples = 50;
    const float _widthExtent = 10.0f;
    const float _depthExtent = 10.0f;
    const float _centerX = _widthExtent / 2.0f;
    const float _centerY = _depthExtent / 2.0f;
    std::vector< float > _heightData;
    for ( size_t i = 0; i < _nWidthSamples; i++ )
    {
        for ( size_t j = 0; j < _nDepthSamples; j++ )
        {
            float _x = _widthExtent * ( ( (float) i ) / _nWidthSamples - 0.5f );
            float _y = _depthExtent * ( ( (float) j ) / _nDepthSamples - 0.5f );
            
            // float _z = 10.0f * ( _x * _x + _y * _y ) / ( _widthExtent * _widthExtent + _depthExtent * _depthExtent );

            float _u = _x * 2.0f;
            float _v = _y * 2.0f;
            float _z = std::cos( std::sqrt( ( _u * _u + _v * _v ) ) );
            // float _z = 1.0f;

            _heightData.push_back( _z );
        }
    }

    auto _patch = engine::CMeshBuilder::createHeightField( _nWidthSamples, _nDepthSamples,
                                                           _widthExtent, _depthExtent,
                                                           _centerX, _centerY,
                                                           _heightData, 1.0f,
                                                           engine::eAxis::Y );
    _patch->material()->ambient = { 0.5f, 0.5f, 0.5f };
    _patch->material()->diffuse = { 0.5f, 0.5f, 0.5f };
    _patch->material()->specular = { 0.5f, 0.5f, 0.5f };
    _patch->material()->shininess = 32.0f;

    //// auto _floor = engine::CMeshBuilder::createPlane( _widthExtent, _depthExtent, engine::eAxis::Y );
    //// _floor->material()->ambient = { 0.3f, 0.5f, 0.8f };
    //// _floor->material()->diffuse = { 0.3f, 0.5f, 0.8f };
    //// _floor->material()->specular = { 0.3f, 0.5f, 0.8f };
    //// _floor->material()->shininess = 32.0f;

    //// auto _texture = engine::CTextureManager::GetCachedTexture( "img_grid" );
    //// auto _texture = engine::CTextureManager::GetCachedTexture( "img_smiley" );
    auto _texture = engine::CTextureManager::GetCachedTexture( "built_in_chessboard" );
    _patch->material()->setAlbedoMap( _texture );
    //// _floor->material()->setAlbedoMap( _texture );

    /* create some renderables in our scene *******************************************************/
    std::string _modelpath = std::string( ENGINE_RESOURCES_PATH ) + "models/pokemons/lizardon/lizardon.obj";
    auto _model = engine::CMeshBuilder::createModelFromFile( _modelpath );
    _model->scale = { 0.1f, 0.1f, 0.1f };
    _model->position = { 0.0f, 2.0f, 0.0f };

    _scene->addRenderable( std::unique_ptr< engine::CIRenderable >( _model ) );
    _scene->addRenderable( std::unique_ptr< engine::CIRenderable >( _patch ) );
    //// _scene->addRenderable( std::unique_ptr< engine::CIRenderable >( _floor ) );
    /**********************************************************************************************/

    _app->setScene( std::unique_ptr< engine::CScene >( _scene ) );

    _app->renderOptions().useShadowMapping = true;
    _app->renderOptions().shadowMapRangeConfig.type = engine::eShadowRangeType::FIXED_USER;
    _app->renderOptions().shadowMapRangeConfig.worldUp = { 0.0f, 1.0f, 0.0f };
    _app->renderOptions().shadowMapRangeConfig.cameraPtr = _orbitCamera;
    _app->renderOptions().shadowMapRangeConfig.clipSpaceWidth   = 40.0f;
    _app->renderOptions().shadowMapRangeConfig.clipSpaceHeight  = 40.0f;
    _app->renderOptions().shadowMapRangeConfig.clipSpaceDepth   = 40.0f;
    //// _app->renderOptions().shadowMapRangeConfig.pointLightPtr = _pointlight;
    _app->renderOptions().shadowMapRangeConfig.dirLightPtr = _dirlight;

    while ( _app->active() )
    {
        if ( engine::CInputManager::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        //// engine::CDebugDrawer::DrawBox( { 0.1f, 0.1f, 0.1f }, engine::CMat4::translation( _pointlight->position ), { 1.0f, 1.0f, 1.0f } );

        //// engine::CDebugDrawer::DrawNormals( _patch, { 1.0f, 0.0f, 1.0f } );

        _app->update();

        _app->begin();
        _app->render();
        _app->end();
    }

    return 0;
}

#include <CEngine.h>

void renderScene( engine::CICamera* cameraRef,
                  engine::CShader* shaderRef,
                  std::vector< std::unique_ptr<engine::CIRenderable> >& renderables );

int main()
{
    auto _app = std::make_unique<engine::CApplication>();

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 50.0f;

    auto _camera = std::make_unique<engine::COrbitCamera>( "orbit",
                                                           engine::CVec3( 0.0f, 0.0f, 3.0f ),
                                                           engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                           engine::eAxis::Y,
                                                           _cameraProjData );

    auto _cameraRef = _app->scene()->AddCamera( std::move( _camera ) );

    auto _plane     = engine::CMeshBuilder::createPlane( 2.0f, 2.0f, engine::eAxis::Y );
    auto _boxy      = engine::CMeshBuilder::createBox( 0.25f, 0.5f, 1.0f );
    auto _sphery    = engine::CMeshBuilder::createSphere( 0.5f );
    auto _ellipsy   = engine::CMeshBuilder::createEllipsoid( 0.2f, 0.4f, 0.6f );
    auto _cylindyX  = engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::X );
    auto _cylindyY  = engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::Y );
    auto _cylindyZ  = engine::CMeshBuilder::createCylinder( 0.25f, 0.5f, engine::eAxis::Z );
    auto _capsulyX  = engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::X );
    auto _capsulyY  = engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::Y );
    auto _capsulyZ  = engine::CMeshBuilder::createCapsule( 0.25f, 0.5f, engine::eAxis::Z );
    auto _arrowyX   = engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::X );
    auto _arrowyY   = engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::Y );
    auto _arrowyZ   = engine::CMeshBuilder::createArrow( 0.5f, engine::eAxis::Z );
    auto _axy       = engine::CMeshBuilder::createAxes( 0.5f );

    std::string _modelpath;
    _modelpath += ENGINE_RESOURCES_PATH;
    _modelpath += "models/chassis.stl";

    auto _model = engine::CMeshBuilder::createModelFromFile( _modelpath );

    _boxy->position      = { -1.0f, 1.0f, -1.0f };
    _sphery->position    = { -1.0f, 1.0f,  1.0f };
    _ellipsy->position   = {  1.0f, 1.0f,  1.0f };
    _cylindyX->position  = {  1.0f, 1.0f, -1.0f };

    _cylindyY->position  = { -1.0f, 2.0f, -1.0f };
    _cylindyZ->position  = { -1.0f, 2.0f,  1.0f };
    _capsulyX->position  = {  1.0f, 2.0f,  1.0f };
    _capsulyY->position  = {  1.0f, 2.0f, -1.0f };

    _capsulyZ->position  = { -1.0f, 3.0f, -1.0f };
    _arrowyX->position   = { -1.0f, 3.0f,  1.0f };
    _arrowyY->position   = {  1.0f, 3.0f,  1.0f };
    _arrowyZ->position   = {  1.0f, 3.0f, -1.0f };

    _axy->position       = { 0.0f, 1.0f, 0.0f };
    _model->position     = { 0.0f, 2.0f, 0.0f };

    _boxy->scale = { 0.2f, 0.2f, 0.2f };

    std::vector< std::unique_ptr<engine::CIRenderable> > _renderables;
    _renderables.push_back( std::move( _plane ) );
    _renderables.push_back( std::move( _boxy ) );
    _renderables.push_back( std::move( _sphery ) );
    _renderables.push_back( std::move( _ellipsy ) );
    _renderables.push_back( std::move( _cylindyX ) );
    _renderables.push_back( std::move( _cylindyY ) );
    _renderables.push_back( std::move( _cylindyZ ) );
    _renderables.push_back( std::move( _capsulyX ) );
    _renderables.push_back( std::move( _capsulyY ) );
    _renderables.push_back( std::move( _capsulyZ ) );
    _renderables.push_back( std::move( _arrowyX ) );
    _renderables.push_back( std::move( _arrowyY ) );
    _renderables.push_back( std::move( _arrowyZ ) );
    _renderables.push_back( std::move( _axy ) );
    _renderables.push_back( std::move( _model ) );

    // give the renderables a little rotation and scale
    std::default_random_engine _randomGenerator;
    std::uniform_real_distribution< float > _randomDistribution( 0.5f, 1.0f );
    for ( auto& renderablePtr : _renderables )
    {
        renderablePtr->rotation = tinymath::rotation( engine::CVec3( _randomDistribution( _randomGenerator ) * (float) ENGINE_PI,
                                                                     _randomDistribution( _randomGenerator ) * (float) ENGINE_PI,
                                                                     _randomDistribution( _randomGenerator ) * (float) ENGINE_PI ) );
        float _scale = _randomDistribution( _randomGenerator );
        renderablePtr->scale = { _scale, _scale, _scale };
    }

    auto _shaderRef = engine::CShaderManager::GetCachedShader( "basic3d_no_textures" );
    ENGINE_ASSERT( _shaderRef, "Could not grab the basic3d shader to render the scene :(" );

    while( _app->active() )
    {
        tinyutils::Clock::Tick();
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_SPACE ) )
            _cameraRef->SetActiveMode( false );
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ENTER ) )
            _cameraRef->SetActiveMode( true );

        if ( _cameraRef->type() == engine::CFpsCamera::GetStaticType() )
        {
            if ( _cameraRef->active() )
                _app->window()->disableCursor();
            else
                _app->window()->enableCursor();
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->update();
        _app->begin();

        /* do some custom rendering here */

        renderScene( _cameraRef, _shaderRef, _renderables );

        /*********************************/

        _app->render();
        _app->end();
        tinyutils::Clock::Tock();
    }

    return 0;
}

void renderScene( engine::CICamera* cameraRef,
                  engine::CShader* shaderRef,
                  std::vector< std::unique_ptr<engine::CIRenderable> >& renderables )
{
    shaderRef->bind();
    shaderRef->setMat4( "u_tView", cameraRef->mat_view() );
    shaderRef->setMat4( "u_tProj", cameraRef->mat_proj() );

    for ( auto& renderablePtr : renderables )
    {
        shaderRef->setVec3( "u_color", renderablePtr->material()->ambient );
        shaderRef->setMat4( "u_tModel", renderablePtr->matModel() );
        renderablePtr->render();

        auto _bbox = renderablePtr->bbox();
        auto _bsphere = renderablePtr->bsphere();

        engine::CDebugDrawer::DrawBox( _bbox.size, _bbox.worldTransform, { 0.2f, 0.2f, 0.8f } );
        engine::CDebugDrawer::DrawSphere( _bsphere.radius, engine::translation( _bsphere.worldPosition ), { 0.2f, 0.8f, 0.2f } );
    }

    shaderRef->unbind();
}

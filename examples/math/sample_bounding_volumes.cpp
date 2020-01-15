
#include <CEngine.h>

void renderScene( engine::CICamera* cameraPtr,
                  engine::CShader* shaderPtr,
                  const std::vector< engine::CIRenderable* >& renderables );

int main()
{
    auto _app = new engine::CApplication();

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 50.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 0.0f, 0.0f, 3.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Y,
                                             _cameraProjData,
                                             _app->window()->width(),
                                             _app->window()->height() );

    _app->scene()->addCamera( std::unique_ptr< engine::CICamera >( _camera ) );

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

    std::vector< engine::CIRenderable* > _renderables = { _plane, _boxy, _sphery, _ellipsy, 
                                                          _cylindyX, _cylindyY, _cylindyZ,
                                                          _capsulyX, _capsulyY, _capsulyZ,
                                                          _arrowyX, _arrowyY, _arrowyZ,
                                                          _axy, _model };

    // give the renderables a little rotation and scale
    std::default_random_engine _randomGenerator;
    std::uniform_real_distribution< float > _randomDistribution( 0.5f, 1.0f );
    for ( size_t i = 0; i < _renderables.size(); i++ )
    {
        _renderables[i]->rotation = tinymath::rotation( engine::CVec3( _randomDistribution( _randomGenerator ) * (float) ENGINE_PI,
                                                                       _randomDistribution( _randomGenerator ) * (float) ENGINE_PI,
                                                                       _randomDistribution( _randomGenerator ) * (float) ENGINE_PI ) );
        float _scale = _randomDistribution( _randomGenerator );
        _renderables[i]->scale = { _scale, _scale, _scale };
    }

    auto _shader = engine::CShaderManager::GetCachedShader( "basic3d_no_textures" );
    ENGINE_ASSERT( _shader, "Could not grab the basic3d shader to render the scene :(" );

    while( _app->active() )
    {
        if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_SPACE ) )
            _camera->setActiveMode( false );
        else if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_ENTER ) )
            _camera->setActiveMode( true );

        if ( _camera->type() == engine::CFpsCamera::GetStaticType() )
        {
            if ( _camera->active() )
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

        renderScene( _camera, _shader, _renderables );

        /*********************************/

        _app->render();
        _app->end();
    }

    return 0;
}

void renderScene( engine::CICamera* cameraPtr,
                  engine::CShader* shaderPtr,
                  const std::vector< engine::CIRenderable* >& renderables )
{
    shaderPtr->bind();
    shaderPtr->setMat4( "u_tView", cameraPtr->matView() );
    shaderPtr->setMat4( "u_tProj", cameraPtr->matProj() );

    for ( auto renderablePtr : renderables )
    {
        shaderPtr->setVec3( "u_color", renderablePtr->material()->ambient );
        shaderPtr->setMat4( "u_tModel", renderablePtr->matModel() );
        renderablePtr->render();

        auto _bbox = renderablePtr->bbox();
        auto _bsphere = renderablePtr->bsphere();

        engine::CDebugDrawer::DrawBox( _bbox.size, _bbox.worldTransform, { 0.2f, 0.2f, 0.8f } );
        engine::CDebugDrawer::DrawSphere( _bsphere.radius, engine::translation( _bsphere.worldPosition ), { 0.2f, 0.8f, 0.2f } );
    }

    shaderPtr->unbind();
}
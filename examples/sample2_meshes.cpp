
#include <iostream>

#include <core/COpenGLApp.h>

#include <LFpsCamera.h>
#include <LFixedCamera3d.h>
#include <LLightDirectional.h>
#include <LMeshBuilder.h>

#include <utils/CImguiUiDemo.h>

class Application : public engine::COpenGLApp
{

public :

    Application() : engine::COpenGLApp() {}
    ~Application() {}

protected :

    void _initUser() override
    {
        ENGINE_TRACE( "Initializing custom ui" );
        m_uiPtr = new engine::CImguiUiDemo( m_windowPtr->context() );
        m_uiPtr->init();
    }

};

int main()
{
    auto _app = new Application();
    _app->init();

    auto _scene = _app->scene();
    
    auto _plane = engine::LMeshBuilder::createPlane( 10.0f, 10.0f, 1.0f, 1.0f );
    auto _chessboardTex = engine::LAssetsManager::getBuiltInTexture( "chessboard" );
    _plane->addTexture( _chessboardTex );
    _plane->getMaterial()->setColor( { 0.2f, 0.3f, 0.4f } );
    _scene->addRenderable( _plane );

    // make a sample mesh just for testing
    // auto _mesh = engine::LMeshBuilder::createBox( 0.5f, 0.5f, 0.5f );
    // auto _mesh = engine::LMeshBuilder::createArrow( 1.0f, "x" );
    // auto _mesh = engine::LMeshBuilder::createArrow( 1.0f, "y" );
    // auto _mesh = engine::LMeshBuilder::createArrow( 1.0f, "z" );
    auto _mesh = engine::LMeshBuilder::createSphere( 0.2f );
    // auto _mesh = engine::LMeshBuilder::createAxes( 1.0f );

    _mesh->getMaterial()->setColor( { 0.7f, 0.5f, 0.3f } );
    _mesh->pos = { 0.0f, 0.0f, 1.0f };

    // make a sample camera
    auto _camera = new engine::LFpsCamera( "fps",
                                           engine::LVec3( 1.0f, 2.0f, 1.0f ),
                                           engine::LVec3( -2.0f, -4.0f, -2.0f ),
                                           engine::LICamera::UP_Z );

    // auto _camera = new engine::LFixedCamera3d( "fixed",
    //                                            engine::LVec3( 1.0f, 2.0f, 1.0f ),
    //                                            engine::LVec3( 0.0f, 0.0f, 0.0f ),
    //                                            engine::LICamera::UP_Z );
    
    // make a sample light source
    auto _light = new engine::LLightDirectional( engine::LVec3( 0.8, 0.8, 0.8 ), engine::LVec3( 0.8, 0.8, 0.8 ),
                                                 engine::LVec3( 0.3, 0.3, 0.3 ), 0, engine::LVec3( 0, 0, -1 ) );
    _light->setVirtualPosition( engine::LVec3( 5.0f, 0.0f, 5.0f ) );

    // add these components to the scene
    _scene->addCamera( _camera );
    _scene->addLight( _light );
    _scene->addRenderable( _mesh );

    float _t = 0.0;

    while( _app->isActive() )
    {
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        // _mesh->rotation = _mesh->rotation * engine::LMat4::rotationZ( 0.01f );
        // _mesh->pos.x = std::sin( _t );

        _t += 0.01f;

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_SPACE ) )
            _scene->getCurrentCamera()->setActiveMode( false );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ENTER ) )
            _scene->getCurrentCamera()->setActiveMode( true );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;
        
        _app->begin();
        _app->update();
        _app->end();
    }

    return 0;
}


#include <iostream>

#include <core/COpenGLApp.h>
#include <graphics/CMeshBuilder.h>

#include <LFpsCamera.h>
#include <LFixedCamera3d.h>
#include <LLightDirectional.h>
#include <LSkybox.h>

#ifndef ENGINE_RESOURCES_PATH
    #define ENGINE_RESOURCES_PATH "../res/"
#endif

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    auto _scene = _app->scene();

    // make a sample mesh just for testing
    std::string _modelpath;
    _modelpath += ENGINE_RESOURCES_PATH;
    _modelpath += "models/pokemons/lizardon.obj";

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

            _heightData.push_back( _z );
        }
    }

    auto _patch = engine::CMeshBuilder::createHeightField( _nWidthSamples, _nDepthSamples,
                                                           _widthExtent, _depthExtent,
                                                           _centerX, _centerY,
                                                           _heightData, 1.0f );
    _patch->getMaterial()->setColor( { 0.5f, 0.5f, 0.5f } );

    auto _model = engine::CMeshBuilder::createModelFromFile( _modelpath, "lizardon" );
    _model->scale = { 0.05f, 0.05f, 0.05f };
    _model->pos = { 0.0f, 0.0f, 3.0f };

    // make a sample camera
    auto _camera = new engine::LFpsCamera( "fixed",
                                           engine::LVec3( 1.0f, 2.0f, -1.0f ),
                                           engine::LVec3( -2.0f, -4.0f, -2.0f ),
                                           engine::LICamera::UP_Z );
    _camera->setPosition( { -1.77f, -4.12f, 4.37f } );

    // make a sample light source
    auto _light = new engine::LLightDirectional( engine::LVec3( 0.2, 0.2, 0.2 ), engine::LVec3( 0.5, 0.5, 0.5 ),
                                                 engine::LVec3( 0.05, 0.05, 0.05 ), 0, engine::LVec3( 0, 0, -1 ) );
    _light->setVirtualPosition( engine::LVec3( 5.0f, 0.0f, 5.0f ) );

    // make a simple skybox
    auto _skybox = new engine::LSkybox( "starfield" );
    // add these components to the scene
    _scene->addCamera( _camera );
    _scene->addLight( _light );
    _scene->addRenderable( _model );
    _scene->addRenderable( _patch );
    _scene->addSkybox( _skybox );

    while( _app->isActive() )
    {
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _model->rotation = _model->rotation * engine::LMat4::rotationX( 0.01f );

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_SPACE ) )
            _scene->getCurrentCamera()->setActiveMode( false );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ENTER ) )
            _scene->getCurrentCamera()->setActiveMode( true );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_L ) )
            _patch->setWireframeMode( true );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_N ) )
            _patch->setWireframeMode( false );
        else if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _app->begin();
        _app->update();
        _app->end();

        // std::cout << "pos: " << _camera->getPosition().toString() << std::endl;
    }

    return 0;
}

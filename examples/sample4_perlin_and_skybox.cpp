
#include <iostream>

#include <LApp.h>
#include <LFpsCamera.h>
#include <LFixedCamera3d.h>
#include <LLightDirectional.h>
#include <LMeshBuilder.h>
#include <LSkybox.h>

#ifndef ENGINE_RESOURCES_PATH
    #define ENGINE_RESOURCES_PATH "../res/"
#endif

int main()
{
    auto _app = engine::LApp::GetInstance();
    auto _scene = _app->scene();
    
    // make a sample mesh just for testing
    std::string _modelpath;
    _modelpath += ENGINE_RESOURCES_PATH;
    _modelpath += "models/pokemons/lizardon.obj";

    auto _patch = engine::LMeshBuilder::createPerlinPatch( 20.0f, 20.0f, 50 );
    _patch->getMaterial()->setColor( { 0.5f, 0.5f, 0.5f } );

    auto _model = engine::LMeshBuilder::createModelFromFile( _modelpath, "lizardon" );
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

        _app->begin();
        _app->update();
        _app->end();

        // std::cout << "pos: " << _camera->getPosition().toString() << std::endl;
    }

    return 0;
}

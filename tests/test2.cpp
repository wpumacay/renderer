
#include <iostream>

#include <LApp.h>
#include <LFpsCamera.h>
#include <LFixedCamera3d.h>
#include <LLightDirectional.h>
#include <LMeshBuilder.h>


int main()
{
    auto _app = engine::LApp::GetInstance();
    auto _scene = _app->scene();
    
    // make a sample mesh just for testing
    // auto _mesh = engine::LMeshBuilder::createBox( 0.5f, 0.5f, 0.5f );
    // auto _mesh1 = engine::LMeshBuilder::createArrow( 1.0f, "x" );
    // auto _mesh2 = engine::LMeshBuilder::createArrow( 1.0f, "y" );
    // auto _mesh3 = engine::LMeshBuilder::createArrow( 1.0f, "z" );
    // auto _mesh4 = engine::LMeshBuilder::createSphere( 0.2f );
    auto _axes = engine::LMeshBuilder::createAxes( 1.0f );

    // make a sample camera
    auto _camera = new engine::LFpsCamera( "fixed",
                                           engine::LVec3( 1.0f, 2.0f, -1.0f ),
                                           engine::LVec3( -2.0f, -4.0f, -2.0f ),
                                           engine::LICamera::UP_Z );

    // auto _camera = new engine::LFixedCamera3d( "fixed",
    //                                            engine::LVec3( 1.0f, 2.0f, 1.0f ),
    //                                            engine::LVec3( 0.0f, 0.0f, 0.0f ),
    //                                            engine::LICamera::UP_Z );
    // make a sample light source
    auto _light = new engine::LLightDirectional( engine::LVec3( 0.2, 0.2, 0.2 ), engine::LVec3( 0.8, 0.8, 0.8 ),
                                                 engine::LVec3( 0.05, 0.05, 0.05 ), 0, engine::LVec3( -1, -1, 0 ) );

    // add these components to the scene
    _scene->addCamera( _camera );
    _scene->addLight( _light );

    // _scene->addRenderable( _mesh1 );
    // _scene->addRenderable( _mesh2 );
    // _scene->addRenderable( _mesh3 );
    // _scene->addRenderable( _mesh4 );

    // _mesh1->getMaterial()->ambient  = { 1.0, 0.0, 0.0 };
    // _mesh1->getMaterial()->diffuse  = { 1.0, 0.0, 0.0 };
    // _mesh1->getMaterial()->specular = { 1.0, 0.0, 0.0 };

    // _mesh2->getMaterial()->ambient  = { 0.0, 1.0, 0.0 };
    // _mesh2->getMaterial()->diffuse  = { 0.0, 1.0, 0.0 };
    // _mesh2->getMaterial()->specular = { 0.0, 1.0, 0.0 };

    // _mesh3->getMaterial()->ambient  = { 0.0, 0.0, 1.0 };
    // _mesh3->getMaterial()->diffuse  = { 0.0, 0.0, 1.0 };
    // _mesh3->getMaterial()->specular = { 0.0, 0.0, 1.0 };

    // _mesh4->getMaterial()->ambient  = { 0.3, 0.3, 0.3 };
    // _mesh4->getMaterial()->diffuse  = { 0.3, 0.3, 0.3 };
    // _mesh4->getMaterial()->specular = { 0.3, 0.3, 0.3 };

    _scene->addRenderable( _axes );

    float _t = 0.0;

    while( _app->isActive() )
    {
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->update();

        _axes->rotation = _axes->rotation * engine::LMat4::rotationZ( 0.01f );
        _axes->pos.x = std::sin( _t );

        _t += 0.01f;
    }

    return 0;
}

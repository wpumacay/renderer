

#include <window/LWindow.h>
#include <shader/LShaderManager.h>
#include <resource/LAssetsManager.h>
#include <graphics/LMeshBuilder.h>
#include <camera/LFixedCamera3d.h>
#include <graphics/LSkybox.h>

using namespace std;

int main()
{
    engine::LWindow _window;



    engine::LShaderManager::create();
    engine::LAssetsManager::create();

    auto _camera = new engine::LFixedCamera3d( engine::LVec3( 1.0f, 1.0f, 1.0f ),
                                               engine::LVec3( -1.0f, -1.0f, -1.0f ),
                                               engine::LVec3( 0.0f, 1.0f, 0.0f ) );

            
    auto _projMatrix = glm::perspective( glm::radians( 45.0f ),
                                        ( float ) ENGINE_APP_WIDTH / ENGINE_APP_HEIGHT,
                                        0.1f, 100.0f );

    auto _cubeMesh = engine::LMeshBuilder::createBox( 0.5, 0.5, 0.5,
                                                      false, true );

    auto _textureData = engine::LAssetsManager::INSTANCE->texturesData["brickwall"];

    auto _texture = new engine::LTexture();
    _texture->setData( _textureData->data, GL_RGB,
                       _textureData->width, _textureData->height,
                       0 );

    auto _cubeMaterial = _cubeMesh->getMaterial();
    _cubeMaterial->disableLighting();
    _cubeMaterial->addTexture( _texture );

    auto _skybox = new engine::LSkybox( "space" );

    engine::LShaderManager::INSTANCE->currentShader = engine::LShaderManager::INSTANCE->programs["basic3d_texture"];

    while ( _window.isActive() )
    {
        _window.clear();
        _window.pollEvents();

        GLuint _shader = engine::LShaderManager::INSTANCE->currentShader;

        auto _cpos = _camera->getPosition();

        //_cpos.x -= 0.03;

        _camera->setPosition( _cpos );

        glUseProgram( _shader );

        GLuint _uniformProj = glGetUniformLocation( _shader, "u_tProj" );
        GLuint _uniformView = glGetUniformLocation( _shader, "u_tView" );
        GLuint _uniformModel = glGetUniformLocation( _shader, "u_tModel" );

        glUniformMatrix4fv( _uniformProj, 1, GL_FALSE, glm::value_ptr( _projMatrix ) );
        glUniformMatrix4fv( _uniformView, 1, GL_FALSE, glm::value_ptr( _camera->getViewMatrix() ) );
        glUniformMatrix4fv( _uniformModel, 1, GL_FALSE, glm::value_ptr( _cubeMesh->getModelMatrix() ) );

        _cubeMesh->render();

        glUseProgram( 0 );

        _shader = engine::LShaderManager::INSTANCE->programs["basic3d_skybox"];

        glDepthFunc( GL_LEQUAL );
        glUseProgram( _shader );

        auto _sview = glm::mat4( glm::mat3( _camera->getViewMatrix() ) );
        _uniformProj = glGetUniformLocation( _shader, "u_tProj" );
        _uniformView = glGetUniformLocation( _shader, "u_tView" );

        glUniformMatrix4fv( _uniformProj, 1, GL_FALSE, glm::value_ptr( _projMatrix ) );
        glUniformMatrix4fv( _uniformView, 1, GL_FALSE, glm::value_ptr( _sview ) );

        _skybox->render();

        glUseProgram( 0 );
        glDepthFunc( GL_LESS );

        _window.swapBuffers();
    }


    return 0;
}
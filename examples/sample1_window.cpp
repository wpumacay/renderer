
#include <CEngine.h>

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    auto _shader = engine::LShaderManager::getShader( "basic2d" );

    std::vector< engine::CVec2 > _vertices = { { -0.5f, -0.5f },
                                               {  0.5f, -0.5f },
                                               {  0.5f,  0.5f },
                                               { -0.5f,  0.5f } };

    std::vector< engine::CInd3 > _indices = { { 0, 1, 2 }, { 0, 2, 3 } };

    std::vector< engine::CVec3 > _colors = { { 1.0f, 0.0f, 0.0f },
                                             { 0.0f, 1.0f, 0.0f },
                                             { 0.0f, 0.0f, 1.0f },
                                             { 1.0f, 1.0f, 1.0f } };

    std::cout << "sizeof( CVec2 ): " << sizeof( engine::CVec2 ) << std::endl;
    std::cout << "sizeof( CVec3 ): " << sizeof( engine::CVec3 ) << std::endl;
    std::cout << "sizeof( CInd3 ): " << sizeof( engine::CInd3 ) << std::endl;

    auto _vbufferPos = new engine::LVertexBuffer();
    _vbufferPos->setData( sizeof( engine::CVec2 ) * _vertices.size(), 2, (GLfloat*) _vertices.data() );

    auto _vbufferCol = new engine::LVertexBuffer();
    _vbufferCol->setData( sizeof( engine::CVec3 ) * _colors.size(), 3, (GLfloat*) _colors.data() );

    auto _ibuffer = new engine::LIndexBuffer();
    _ibuffer->setData( sizeof( engine::CInd3 ) * _indices.size(), 3 * _indices.size(), (GLuint*) _indices.data() );

    auto _varray = new engine::LVertexArray();
    _varray->addBuffer( _vbufferPos, 0 );
    _varray->addBuffer( _vbufferCol, 1 );

    while( _app->isActive() )
    {
        _app->begin();

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _shader->bind();

        _varray->bind();
        _ibuffer->bind();

        glDrawElements( GL_TRIANGLES, _ibuffer->getCount(), GL_UNSIGNED_INT, 0 );

        _ibuffer->unbind();
        _varray->unbind();

        _shader->unbind();

        _app->end();
    }

    return 0;
}

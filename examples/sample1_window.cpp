
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

    auto _vbufferPos = new engine::CVertexBuffer( { { "pos", engine::eElementType::Float2, false } },
                                                  engine::eBufferUsage::STATIC,
                                                  sizeof( engine::CVec2 ) * _vertices.size(),
                                                  (engine::float32*) _vertices.data() );

    auto _vbufferCol = new engine::CVertexBuffer( { { "color", engine::eElementType::Float3, false } },
                                                  engine::eBufferUsage::STATIC,
                                                  sizeof( engine::CVec3 ) * _colors.size(),
                                                  (engine::float32*) _colors.data() );

    auto _ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                              3 * _indices.size(),
                                              (engine::uint32*) _indices.data() );

    auto _varray = new engine::CVertexArray();
    _varray->addVertexBuffer( _vbufferPos );
    _varray->addVertexBuffer( _vbufferCol );
    _varray->setIndexBuffer( _ibuffer );

    while( _app->isActive() )
    {
        _app->begin();

        if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _shader->bind();
        _varray->bind();

        glDrawElements( GL_TRIANGLES, _varray->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );

        _varray->unbind();
        _shader->unbind();

        _app->end();
    }

    return 0;
}

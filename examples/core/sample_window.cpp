
#include <CEngine.h>

engine::CVertexArray* createGeometryVer1()
{
    float _bufferData[] = {
    /*|      pos     |     color      |*/
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 1.0f, 1.0f, 1.0f
    };

    engine::CVertexBufferLayout _layout = { { "pos", engine::eElementType::Float2, false },
                                            { "color", engine::eElementType::Float3, false } };

    auto _vbuffer = new engine::CVertexBuffer( _layout, 
                                               engine::eBufferUsage::STATIC, 
                                               sizeof( _bufferData ), 
                                               _bufferData );

    auto _varray = new engine::CVertexArray();
    _varray->addVertexBuffer( _vbuffer );

    return _varray;
}

engine::CVertexArray* createGeometryVer2()
{
    float _bufferPosData[] = {
    /*|      pos     |*/
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f 
    };

    float _bufferColData[] = {
    /*|      color      |*/
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    engine::CVertexBufferLayout _layoutPos = { { "pos", engine::eElementType::Float2, false } };
    engine::CVertexBufferLayout _layoutCol = { { "color", engine::eElementType::Float3, false } };

    auto _vbufferPos = new engine::CVertexBuffer( _layoutPos,
                                                  engine::eBufferUsage::STATIC,
                                                  sizeof( _bufferPosData ),
                                                  _bufferPosData );

    auto _vbufferCol = new engine::CVertexBuffer( _layoutCol,
                                                  engine::eBufferUsage::STATIC,
                                                  sizeof( _bufferColData ),
                                                  _bufferColData );

    auto _varray = new engine::CVertexArray();
    _varray->addVertexBuffer( _vbufferPos );
    _varray->addVertexBuffer( _vbufferCol );

    return _varray;
}

engine::CVertexArray* createGeometryVer1Indices()
{
    float _bufferData[] = {
    /*|      pos     |     color      |*/
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 1.0f, 1.0f, 1.0f
    };

    engine::uint32 _indices[] = { 
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    engine::CVertexBufferLayout _layout = { { "pos", engine::eElementType::Float2, false },
                                            { "color", engine::eElementType::Float3, false } };

    auto _vbuffer = new engine::CVertexBuffer( _layout, 
                                               engine::eBufferUsage::STATIC, 
                                               sizeof( _bufferData ), 
                                               _bufferData );

    auto _ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                              6, _indices );

    auto _varray = new engine::CVertexArray();
    _varray->addVertexBuffer( _vbuffer );
    _varray->setIndexBuffer( _ibuffer );

    return _varray;
}

engine::CVertexArray* createGeometryVer2Indices()
{
    float _bufferPosData[] = {
    /*|      pos     |*/
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f 
    };

    float _bufferColData[] = {
    /*|      color      |*/
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    };

    engine::uint32 _indices[] = { 
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    engine::CVertexBufferLayout _layoutPos = { { "pos", engine::eElementType::Float2, false } };
    engine::CVertexBufferLayout _layoutCol = { { "color", engine::eElementType::Float3, false } };

    auto _vbufferPos = new engine::CVertexBuffer( _layoutPos,
                                                  engine::eBufferUsage::STATIC,
                                                  sizeof( _bufferPosData ),
                                                  _bufferPosData );

    auto _vbufferCol = new engine::CVertexBuffer( _layoutCol,
                                                  engine::eBufferUsage::STATIC,
                                                  sizeof( _bufferColData ),
                                                  _bufferColData );

    auto _ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                              6, _indices );

    auto _varray = new engine::CVertexArray();
    _varray->addVertexBuffer( _vbufferPos );
    _varray->addVertexBuffer( _vbufferCol );
    _varray->setIndexBuffer( _ibuffer );

    return _varray;
}

int main()
{
    engine::CLogger::Init();
    engine::CInputManager::Init();

    auto _window = std::unique_ptr< engine::COpenGLWindow >( new engine::COpenGLWindow( engine::CWindowProps() ) );
    _window->registerKeyCallback( engine::CInputManager::Callback_key );
    _window->registerMouseCallback( engine::CInputManager::Callback_mouse );
    _window->registerMouseMoveCallback( engine::CInputManager::Callback_mouseMove );
    _window->registerScrollCallback( engine::CInputManager::Callback_scroll );

    engine::CShaderManager::Init();
    auto _shader = engine::CShaderManager::GetCachedShader( "basic2d_no_textures" );

    std::cout << "sizeof( CVec2 ): " << sizeof( engine::CVec2 ) << std::endl;
    std::cout << "sizeof( CVec3 ): " << sizeof( engine::CVec3 ) << std::endl;
    std::cout << "sizeof( CInd3 ): " << sizeof( engine::CInd3 ) << std::endl;

    // auto _geometryVAO = createGeometryVer1();
    // auto _geometryVAO = createGeometryVer2();
    // auto _geometryVAO = createGeometryVer1Indices();
    auto _geometryVAO = createGeometryVer2Indices();

    while( _window->active() )
    {
        _window->begin();

        if ( engine::CInputManager::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _shader->bind();

        _geometryVAO->bind();

        if ( _geometryVAO->indexBuffer() )
            glDrawElements( GL_TRIANGLES, _geometryVAO->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );
        else
            glDrawArrays( GL_TRIANGLES, 0, 6 );

        _geometryVAO->unbind();

        _shader->unbind();

        _window->end();
    }

    return 0;
}

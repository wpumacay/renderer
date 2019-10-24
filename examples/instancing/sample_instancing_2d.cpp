
#include <CEngine.h>

float g_margin_x = 0.1f;
float g_margin_y = 0.1f;
int g_num_rows = 10;
int g_num_cols = 10;

bool g_useInstancing = true;

engine::CVertexArray* g_quadVAO_NoInstancing = nullptr;
engine::CVertexArray* g_quadVAO_Instancing = nullptr;
engine::CVertexBuffer* g_quadVAO_positionsBuffer = nullptr;

engine::CVertexArray* createQuad_noInstancing();
engine::CVertexArray* createQuad_Instancing();

void drawGrid_noInstancing( engine::CShader* shader,
                            engine::CVertexArray* vao );
void drawGrid_Instancing( engine::CShader* shader,
                          engine::CVertexArray* vao,
                          engine::CVec2* bufferPositions );

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    /* load the shader used for instancing-2d */
    std::string _baseNameInstancing2d = std::string( ENGINE_EXAMPLES_PATH ) + "instancing/shaders/instancing_2d";
    auto _shaderInstancing2d = engine::CShaderManager::CreateShaderFromFiles( "instancing_2d",
                                                                  _baseNameInstancing2d + "_vs.glsl",
                                                                  _baseNameInstancing2d + "_fs.glsl" );
    ENGINE_ASSERT( _shaderInstancing2d, "Couldn't load instancing-2d shader for our demo T_T" );

    std::string _baseNameNoInstancing2d = std::string( ENGINE_EXAMPLES_PATH ) + "instancing/shaders/no_instancing_2d";
    auto _shaderNoInstancing2d = engine::CShaderManager::CreateShaderFromFiles( "no_instancing_2d",
                                                                                _baseNameNoInstancing2d + "_vs.glsl",
                                                                                _baseNameNoInstancing2d + "_fs.glsl" );
    ENGINE_ASSERT( _shaderNoInstancing2d, "Couldn't load no-instancing-2d shader for our demo T_T" );

    g_quadVAO_NoInstancing = createQuad_noInstancing();
    g_quadVAO_Instancing = createQuad_Instancing();

    auto _bufferPositions = new engine::CVec2[g_num_rows * g_num_cols];

    while( _app->active() )
    {
        if ( engine::CInputManager::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _app->beginRendering();

        if ( g_useInstancing )
            drawGrid_Instancing( _shaderInstancing2d, g_quadVAO_Instancing, _bufferPositions );
        else
            drawGrid_noInstancing( _shaderNoInstancing2d, g_quadVAO_NoInstancing );

        _app->endRendering();
    }

    auto _vbos_noInstancing = g_quadVAO_NoInstancing->vertexBuffers();
    auto _ibo_noInstancing = g_quadVAO_NoInstancing->indexBuffer();
    for ( auto _vbo : _vbos_noInstancing )
        delete _vbo;
    delete _ibo_noInstancing;
    delete g_quadVAO_NoInstancing;

    auto _vbos_Instancing = g_quadVAO_Instancing->vertexBuffers();
    auto _ibo_Instancing = g_quadVAO_Instancing->indexBuffer();
    for ( auto _vbo : _vbos_Instancing )
        delete _vbo;
    delete _ibo_Instancing;
    delete g_quadVAO_Instancing;

    return 0;
}

engine::CVertexArray* createQuad_noInstancing()
{
    float _bufferData[] = {
    /*|      pos     |     color      |*/
        -0.05f, -0.05f, 1.0f, 0.0f, 0.0f,
         0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
         0.05f,  0.05f, 0.0f, 0.0f, 1.0f,
        -0.05f,  0.05f, 1.0f, 1.0f, 1.0f
    };

    engine::uint32 _indices[] = { 
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    engine::CVertexBufferLayout _layoutPerVertex = { { "pos", engine::eElementType::Float2, false },
                                                     { "color", engine::eElementType::Float3, false } };

    auto _vbuffer = new engine::CVertexBuffer( _layoutPerVertex,
                                               engine::eBufferUsage::STATIC,
                                               sizeof( _bufferData ),
                                               _bufferData );

    auto _ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                              6, _indices );

    auto _quadVAO = new engine::CVertexArray();
    _quadVAO->addVertexBuffer( _vbuffer );
    _quadVAO->setIndexBuffer( _ibuffer );

    return _quadVAO;
}

engine::CVertexArray* createQuad_Instancing()
{
    float _bufferData[] = {
    /*|      pos     |     color      |*/
        -0.05f, -0.05f, 1.0f, 0.0f, 0.0f,
         0.05f, -0.05f, 0.0f, 1.0f, 0.0f,
         0.05f,  0.05f, 0.0f, 0.0f, 1.0f,
        -0.05f,  0.05f, 1.0f, 1.0f, 1.0f
    };

    engine::uint32 _indices[] = { 
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    engine::CVertexBufferLayout _layoutPerVertex = { { "pos", engine::eElementType::Float2, false },
                                                     { "color", engine::eElementType::Float3, false } };

    auto _vbuffer = new engine::CVertexBuffer( _layoutPerVertex,
                                               engine::eBufferUsage::STATIC,
                                               sizeof( _bufferData ),
                                               _bufferData );

    auto _ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                              6, _indices );

    auto _vpositionsBuffer = new engine::CVertexBuffer( { { "screenPosition", engine::eElementType::Float2, false } },
                                                        engine::eBufferUsage::DYNAMIC,
                                                        g_num_rows * g_num_cols * engine::sizeOfElement( engine::eElementType::Float2 ),
                                                        NULL );

    auto _quadVAO = new engine::CVertexArray();
    _quadVAO->addVertexBuffer( _vbuffer );
    _quadVAO->addVertexBuffer( _vpositionsBuffer, true ); // tell the vao that this has to be instanced
    _quadVAO->setIndexBuffer( _ibuffer );

    g_quadVAO_positionsBuffer = _vpositionsBuffer; // keep a reference to update the positions

    return _quadVAO;
}

void drawGrid_noInstancing( engine::CShader* shader,
                            engine::CVertexArray* vao )
{
    shader->bind();
    vao->bind();

    for ( size_t i = 0; i < g_num_rows; i++ )
    {
        for ( size_t j = 0; j < g_num_cols; j++ )
        {
            float _x = 2.0f * ( ( (float)j ) / ( g_num_cols - 1 ) - 0.5f ); // range [-1, 1]
            float _y = 2.0f * ( ( (float)i ) / ( g_num_rows - 1 ) - 0.5f ); // range [-1, 1]
            _x = ( 1.0f - g_margin_x ) * _x; // range [-1 + margin, 1 - margin]
            _y = ( 1.0f - g_margin_y ) * _y; // range [-1 + margin, 1 - margin]
            shader->setVec2( "u_screenPosition", { _x, _y } );
            glDrawElements( GL_TRIANGLES, vao->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );
        }
    }

    vao->unbind();
    shader->unbind();
}

void drawGrid_Instancing( engine::CShader* shader,
                          engine::CVertexArray* vao,
                          engine::CVec2* bufferPositions )
{
    shader->bind();
    vao->bind();

    for ( size_t i = 0; i < g_num_rows; i++ )
    {
        for ( size_t j = 0; j < g_num_cols; j++ )
        {
            float _x = 2.0f * ( ( (float)j ) / ( g_num_cols - 1 ) - 0.5f ); // range [-1, 1]
            float _y = 2.0f * ( ( (float)i ) / ( g_num_rows - 1 ) - 0.5f ); // range [-1, 1]
            _x = ( 1.0f - g_margin_x ) * _x; // range [-1 + margin, 1 - margin]
            _y = ( 1.0f - g_margin_y ) * _y; // range [-1 + margin, 1 - margin]
            bufferPositions[j + i * g_num_cols] = { _x, _y };
        }
    }

    // update our buffer data
    g_quadVAO_positionsBuffer->updateData( g_num_rows * g_num_cols * engine::sizeOfElement( engine::eElementType::Float2 ),
                                           (engine::float32*) bufferPositions );

    // do the render call (instanced)
    glDrawElementsInstanced( GL_TRIANGLES, vao->indexBuffer()->count(), GL_UNSIGNED_INT, 0, g_num_rows * g_num_cols );

    vao->unbind();
    shader->unbind();
}
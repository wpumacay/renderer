
#include <CEngine.h>

int g_num_rows = 30;
int g_num_cols = 30;
int g_num_floors = 30;

bool g_useInstancing = true;

engine::CVertexArray* g_cubeVAO_NoInstancing = nullptr;
engine::CVertexArray* g_cubeVAO_Instancing = nullptr;
engine::CVertexBuffer* g_cubeVAO_modelMatricesBuffer = nullptr;

engine::CVertexArray* createCube_noInstancing();
engine::CVertexArray* createCube_Instancing();

void drawGrid_noInstancing( engine::CShader* shader,
                            engine::CICamera* camera,
                            engine::CVertexArray* vao );
void drawGrid_Instancing( engine::CShader* shader,
                          engine::CICamera* camera,
                          engine::CVertexArray* vao,
                          std::vector< engine::CMat4 >& bufferModelMatrices );

int main()
{
    auto _app = new engine::CApplication();

    /* load the shader used for instancing-3d */
    std::string _baseNameInstancing3d = std::string( ENGINE_EXAMPLES_PATH ) + "instancing/shaders/instancing_3d";
    auto _shaderInstancing3d = engine::CShaderManager::CreateShaderFromFiles( "instancing_3d",
                                                                  _baseNameInstancing3d + "_vs.glsl",
                                                                  _baseNameInstancing3d + "_fs.glsl" );
    ENGINE_ASSERT( _shaderInstancing3d, "Couldn't load instancing-3d shader for our demo T_T" );

    std::string _baseNameNoInstancing3d = std::string( ENGINE_EXAMPLES_PATH ) + "instancing/shaders/no_instancing_3d";
    auto _shaderNoInstancing3d = engine::CShaderManager::CreateShaderFromFiles( "no_instancing_3d",
                                                                                _baseNameNoInstancing3d + "_vs.glsl",
                                                                                _baseNameNoInstancing3d + "_fs.glsl" );
    ENGINE_ASSERT( _shaderNoInstancing3d, "Couldn't load no-instancing-3d shader for our demo T_T" );

    g_cubeVAO_NoInstancing = createCube_noInstancing();
    g_cubeVAO_Instancing = createCube_Instancing();

    auto _bufferModelMatrices = std::vector< engine::CMat4 >( g_num_rows * g_num_cols * g_num_floors, engine::CMat4() );

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 0.0f, 0.0f, 3.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Y,
                                             _cameraProjData,
                                             _app->window()->width(),
                                             _app->window()->height() );

    _app->scene()->addCamera( std::unique_ptr< engine::CICamera >( _camera ) );

    while( _app->active() )
    {
        if ( engine::CInputManager::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( ENGINE_KEY_I ) )
        {
            g_useInstancing = !g_useInstancing;
            std::cout << std::string( g_useInstancing ? "using instancing" : "not using instancing" ) << std::endl;
        }

        _app->update();
        _app->begin();

        if ( g_useInstancing )
            drawGrid_Instancing( _shaderInstancing3d, _camera, g_cubeVAO_Instancing, _bufferModelMatrices );
        else
            drawGrid_noInstancing( _shaderNoInstancing3d, _camera, g_cubeVAO_NoInstancing );

        _app->render();
        _app->end();
    }

    auto _vbos_noInstancing = g_cubeVAO_NoInstancing->vertexBuffers();
    auto _ibo_noInstancing = g_cubeVAO_NoInstancing->indexBuffer();
    for ( auto _vbo : _vbos_noInstancing )
        delete _vbo;
    delete _ibo_noInstancing;
    delete g_cubeVAO_NoInstancing;

    auto _vbos_Instancing = g_cubeVAO_Instancing->vertexBuffers();
    auto _ibo_Instancing = g_cubeVAO_Instancing->indexBuffer();
    for ( auto _vbo : _vbos_Instancing )
        delete _vbo;
    delete _ibo_Instancing;
    delete g_cubeVAO_Instancing;

    return 0;
}

engine::CVertexArray* createCube_noInstancing()
{
    float _bufferData[] = {
    /*|      pos              |      color      |*/
        -0.25f, -0.25f, -0.25f, 0.0f, 0.0f, 0.0f,
         0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
         0.25f,  0.25f, -0.25f, 1.0f, 1.0f, 0.0f,
        -0.25f,  0.25f, -0.25f, 0.0f, 1.0f, 0.0f,
        -0.25f, -0.25f,  0.25f, 0.0f, 0.0f, 1.0f,
         0.25f, -0.25f,  0.25f, 1.0f, 0.0f, 1.0f,
         0.25f,  0.25f,  0.25f, 1.0f, 1.0f, 1.0f,
        -0.25f,  0.25f,  0.25f, 0.0f, 1.0f, 1.0f
    };

    engine::uint32 _indices[] = { 
        0, 1, 2, 0, 2, 3, // first face
        1, 5, 6, 1, 6, 2, // second face
        5, 4, 7, 5, 7, 6, // third face
        4, 0, 3, 4, 3, 7, // fourth face
        3, 2, 6, 3, 6, 7, // fifth face
        0, 4, 5, 0, 5, 1  // sixth face
    };

    engine::CVertexBufferLayout _layoutPerVertex = { { "pos", engine::eElementType::Float3, false },
                                                     { "color", engine::eElementType::Float3, false } };

    auto _vbuffer = new engine::CVertexBuffer( _layoutPerVertex,
                                               engine::eBufferUsage::STATIC,
                                               sizeof( _bufferData ),
                                               _bufferData );

    auto _ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                              6 * 6, _indices );

    auto _cubeVAO = new engine::CVertexArray();
    _cubeVAO->addVertexBuffer( _vbuffer );
    _cubeVAO->setIndexBuffer( _ibuffer );

    return _cubeVAO;
}

engine::CVertexArray* createCube_Instancing()
{
    float _bufferData[] = {
    /*|      pos              |      color      |*/
        -0.25f, -0.25f, -0.25f, 0.0f, 0.0f, 0.0f,
         0.25f, -0.25f, -0.25f, 1.0f, 0.0f, 0.0f,
         0.25f,  0.25f, -0.25f, 1.0f, 1.0f, 0.0f,
        -0.25f,  0.25f, -0.25f, 0.0f, 1.0f, 0.0f,
        -0.25f, -0.25f,  0.25f, 0.0f, 0.0f, 1.0f,
         0.25f, -0.25f,  0.25f, 1.0f, 0.0f, 1.0f,
         0.25f,  0.25f,  0.25f, 1.0f, 1.0f, 1.0f,
        -0.25f,  0.25f,  0.25f, 0.0f, 1.0f, 1.0f
    };

    engine::uint32 _indices[] = { 
        0, 1, 2, 0, 2, 3, // first face
        1, 5, 6, 1, 6, 2, // second face
        5, 4, 7, 5, 7, 6, // third face
        4, 0, 3, 4, 3, 7, // fourth face
        3, 2, 6, 3, 6, 7, // fifth face
        0, 4, 5, 0, 5, 1  // sixth face
    };

    engine::CVertexBufferLayout _layoutPerVertex = { { "pos", engine::eElementType::Float3, false },
                                                     { "color", engine::eElementType::Float3, false } };

    auto _vbuffer = new engine::CVertexBuffer( _layoutPerVertex,
                                               engine::eBufferUsage::STATIC,
                                               sizeof( _bufferData ),
                                               _bufferData );

    auto _ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                              6 * 6, _indices );

    auto _modelMatricesBuffer = new engine::CVertexBuffer( { { "modelMatrix-col0", engine::eElementType::Float4, false },
                                                             { "modelMatrix-col1", engine::eElementType::Float4, false },
                                                             { "modelMatrix-col2", engine::eElementType::Float4, false },
                                                             { "modelMatrix-col3", engine::eElementType::Float4, false } },
                                                           engine::eBufferUsage::DYNAMIC,
                                                           g_num_rows * g_num_cols * g_num_floors * 4 * engine::sizeOfElement( engine::eElementType::Float4 ),
                                                           NULL );

    auto _cubeVAO = new engine::CVertexArray();
    _cubeVAO->addVertexBuffer( _vbuffer );
    _cubeVAO->addVertexBuffer( _modelMatricesBuffer, true ); // tell the vao that this has to be instanced
    _cubeVAO->setIndexBuffer( _ibuffer );

    g_cubeVAO_modelMatricesBuffer = _modelMatricesBuffer; // keep a reference to update the positions

    return _cubeVAO;
}

void drawGrid_noInstancing( engine::CShader* shader,
                            engine::CICamera* camera,
                            engine::CVertexArray* vao )
{
    shader->bind();
    vao->bind();

    shader->setMat4( "u_viewProjMatrix", camera->matProj() * camera->matView() );

    for ( size_t i = 0; i < g_num_rows; i++ )
    {
        for ( size_t j = 0; j < g_num_cols; j++ )
        {
            for ( size_t k = 0; k < g_num_floors; k++ )
            {
                float _x = 20.0f * ( ( (float)j ) / ( g_num_cols - 1 ) - 0.5f );    // range [-5, 5]
                float _y = 20.0f * ( ( (float)i ) / ( g_num_rows - 1 ) - 0.5f );    // range [-5, 5]
                float _z = 20.0f * ( ( (float)k ) / ( g_num_floors - 1 ) - 0.5f );  // range [-5, 5]
                
                engine::CMat4 _modelMatrix = engine::translation( { _x, _y, _z } );
                shader->setMat4( "u_modelMatrix", _modelMatrix );
                glDrawElements( GL_TRIANGLES, vao->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );
            }
        }
    }

    vao->unbind();
    shader->unbind();
}

void drawGrid_Instancing( engine::CShader* shader,
                          engine::CICamera* camera,
                          engine::CVertexArray* vao,
                          std::vector< engine::CMat4 >& bufferModelMatrices )
{
    shader->bind();
    vao->bind();

    shader->setMat4( "u_viewProjMatrix", camera->matProj() * camera->matView() );

    for ( size_t i = 0; i < g_num_rows; i++ )
    {
        for ( size_t j = 0; j < g_num_cols; j++ )
        {
            for ( size_t k = 0; k < g_num_floors; k++ )
            {
                float _x = 20.0f * ( ( (float)j ) / ( g_num_cols - 1 ) - 0.5f );    // range [-5, 5]
                float _y = 20.0f * ( ( (float)i ) / ( g_num_rows - 1 ) - 0.5f );    // range [-5, 5]
                float _z = 20.0f * ( ( (float)k ) / ( g_num_floors - 1 ) - 0.5f );  // range [-5, 5]

                int _index = k + j * g_num_floors + i * g_num_floors * g_num_cols;
                bufferModelMatrices[_index] =  engine::translation( { _x, _y, _z } );
            }
        }
    }

    // update our buffer data
    int _num_instances = g_num_rows * g_num_cols * g_num_floors;
    int _size_of_mat4 = 4 * engine::sizeOfElement( engine::eElementType::Float4 );
    g_cubeVAO_modelMatricesBuffer->updateData( _num_instances * _size_of_mat4,
                                               (engine::float32*) bufferModelMatrices.data() );

    // do the render call (instanced)
    glDrawElementsInstanced( GL_TRIANGLES, vao->indexBuffer()->count(), GL_UNSIGNED_INT, 0, _num_instances );

    vao->unbind();
    shader->unbind();
}
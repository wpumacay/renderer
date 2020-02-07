
#include <CEngine.h>

int g_num_rows = 30;
int g_num_cols = 30;
int g_num_floors = 30;

bool g_useInstancing = true;

std::unique_ptr<engine::CVertexArray> g_cubeVAO_NoInstancing = nullptr;
std::unique_ptr<engine::CVertexArray> g_cubeVAO_Instancing = nullptr;
engine::CVertexBuffer* g_cubeVAO_modelMatricesBufferRef = nullptr;

std::unique_ptr<engine::CVertexArray> createCube_noInstancing();
std::unique_ptr<engine::CVertexArray> createCube_Instancing();

void drawGrid_noInstancing( engine::CShader* shader,
                            engine::CICamera* camera,
                            engine::CVertexArray* vao );
void drawGrid_Instancing( engine::CShader* shader,
                          engine::CICamera* camera,
                          engine::CVertexArray* vao,
                          std::vector< engine::CMat4 >& bufferModelMatrices );

int main()
{
    auto _app = std::make_unique<engine::CApplication>();

    /* load the shader used for instancing-3d */
    std::string _baseNameInstancing3d = std::string( ENGINE_EXAMPLES_PATH ) + "instancing/shaders/instancing_3d";
    auto _shaderInstancing3dRef = engine::CShaderManager::CreateShaderFromFiles( "instancing_3d",
                                                                                 _baseNameInstancing3d + "_vs.glsl",
                                                                                 _baseNameInstancing3d + "_fs.glsl" );
    ENGINE_ASSERT( _shaderInstancing3dRef, "Couldn't load instancing-3d shader for our demo T_T" );

    std::string _baseNameNoInstancing3d = std::string( ENGINE_EXAMPLES_PATH ) + "instancing/shaders/no_instancing_3d";
    auto _shaderNoInstancing3dRef = engine::CShaderManager::CreateShaderFromFiles( "no_instancing_3d",
                                                                                   _baseNameNoInstancing3d + "_vs.glsl",
                                                                                   _baseNameNoInstancing3d + "_fs.glsl" );
    ENGINE_ASSERT( _shaderNoInstancing3dRef, "Couldn't load no-instancing-3d shader for our demo T_T" );

    g_cubeVAO_NoInstancing = createCube_noInstancing();
    g_cubeVAO_Instancing = createCube_Instancing();

    auto _bufferModelMatrices = std::vector< engine::CMat4 >( g_num_rows * g_num_cols * g_num_floors, engine::CMat4() );

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = std::make_unique<engine::COrbitCamera>( "orbit",
                                                           engine::CVec3( 0.0f, 0.0f, 3.0f ),
                                                           engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                           engine::eAxis::Y,
                                                           _cameraProjData,
                                                           _app->window()->width(),
                                                           _app->window()->height() );

    auto _cameraRef = _app->scene()->AddCamera( std::move( _camera ) );

    while( _app->active() )
    {
        if ( engine::CInputManager::IsKeyDown( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_I ) )
        {
            g_useInstancing = !g_useInstancing;
            ENGINE_TRACE( g_useInstancing ? "Using instancing" : "Not using instancing" );
        }

        _app->update();
        _app->begin();

        if ( g_useInstancing )
            drawGrid_Instancing( _shaderInstancing3dRef, _camera.get(), g_cubeVAO_Instancing.get(), _bufferModelMatrices );
        else
            drawGrid_noInstancing( _shaderNoInstancing3dRef, _camera.get(), g_cubeVAO_NoInstancing.get() );

        _app->render();
        _app->end();
    }

    return 0;
}

std::unique_ptr<engine::CVertexArray> createCube_noInstancing()
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

    auto _vbuffer = std::make_unique<engine::CVertexBuffer>( _layoutPerVertex, engine::eBufferUsage::STATIC,
                                                             sizeof( _bufferData ), _bufferData );

    auto _ibuffer = std::make_unique<engine::CIndexBuffer>( engine::eBufferUsage::STATIC, 6 * 6, _indices );

    auto _cubeVAO = std::make_unique<engine::CVertexArray>();
    _cubeVAO->addVertexBuffer( std::move( _vbuffer ) );
    _cubeVAO->setIndexBuffer( std::move( _ibuffer ) );

    return std::move( _cubeVAO );
}

std::unique_ptr<engine::CVertexArray> createCube_Instancing()
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

    auto _vbuffer = std::make_unique<engine::CVertexBuffer>( _layoutPerVertex, engine::eBufferUsage::STATIC,
                                                             sizeof( _bufferData ), _bufferData );

    auto _ibuffer = std::make_unique<engine::CIndexBuffer>( engine::eBufferUsage::STATIC, 6 * 6, _indices );

    auto _modelMatricesBufferLayout = engine::CVertexBufferLayout( { { "modelMatrix-col0", engine::eElementType::Float4, false },
                                                                     { "modelMatrix-col1", engine::eElementType::Float4, false },
                                                                     { "modelMatrix-col2", engine::eElementType::Float4, false },
                                                                     { "modelMatrix-col3", engine::eElementType::Float4, false } } );
    auto _modelMatricesBuffer = std::make_unique<engine::CVertexBuffer>( _modelMatricesBufferLayout, engine::eBufferUsage::DYNAMIC,
                                                                         g_num_rows * g_num_cols * g_num_floors * 4 * engine::sizeOfElement( engine::eElementType::Float4 ),
                                                                         (engine::float32*) NULL );

    g_cubeVAO_modelMatricesBufferRef = _modelMatricesBuffer.get(); // keep a reference to update the positions

    auto _cubeVAO = std::make_unique<engine::CVertexArray>();
    _cubeVAO->addVertexBuffer( std::move( _vbuffer ) );
    _cubeVAO->addVertexBuffer( std::move( _modelMatricesBuffer ), true ); // tell the vao that this has to be instanced
    _cubeVAO->setIndexBuffer( std::move( _ibuffer ) );

    return std::move( _cubeVAO );
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
    g_cubeVAO_modelMatricesBufferRef->updateData( _num_instances * _size_of_mat4,
                                               (engine::float32*) bufferModelMatrices.data() );

    // do the render call (instanced)
    glDrawElementsInstanced( GL_TRIANGLES, vao->indexBuffer()->count(), GL_UNSIGNED_INT, 0, _num_instances );

    vao->unbind();
    shader->unbind();
}

#include <CEngine.h>

#include <core/CTexture.h>
#include <assets/CTextureManager.h>

class ApplicationUi : public engine::CImguiUi
{



};

class Application : public engine::COpenGLApp
{

public :

    Application() : engine::COpenGLApp() {}
    ~Application() {}

protected :

    void _initUser() override
    {
        ENGINE_TRACE( "Initializing custom ui" );
        // m_uiPtr = new ApplicationUi( m_windowPtr->context() );
        // m_uiPtr->init();
    }

};


int main()
{
    auto _app = new Application();
    _app->init();

    auto _textureData = engine::CTextureManager::GetCachedTextureData( "img_container" );

    if ( _textureData )
    {
        std::cout << "texture-data:" << std::endl;
        std::cout << engine::toString( *_textureData ) << std::endl;
    }

    auto _texture = engine::CTextureManager::GetCachedTexture( "img_container" );

    ENGINE_ASSERT( _texture, "Could not retrieve a valid texture for the sample" );

    float _bufferData[] = {
    /*|      pos     |     color      |  texture  */
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    engine::uint32 _indices[] = { 
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    engine::CVertexBufferLayout _layout = { { "pos", engine::eElementType::Float2, false }, 
                                            { "col", engine::eElementType::Float3, false },
                                            { "uv",  engine::eElementType::Float2, false } };

    auto _vbuffer = new engine::CVertexBuffer( _layout,
                                               engine::eBufferUsage::STATIC,
                                               sizeof( _bufferData ),
                                               _bufferData );
    auto _ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                              6, _indices );

    auto _varray = new engine::CVertexArray();
    _varray->addVertexBuffer( _vbuffer );
    _varray->setIndexBuffer( _ibuffer );

    auto _shader = engine::LShaderManager::getShader( "basic2d_tex" );

    while( _app->isActive() )
    {
        _app->begin();

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _shader->bind();
        _varray->bind();
        _texture->bind();

        glDrawElements( GL_TRIANGLES, _varray->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );

        _texture->unbind();
        _varray->unbind();
        _shader->unbind();

        _app->end();
    }

    return 0;
}
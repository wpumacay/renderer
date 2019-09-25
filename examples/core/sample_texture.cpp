
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

    void setTextureToShow( std::shared_ptr< engine::CTexture > texture )
    {
        if ( !m_uiPtr )
            return;

        reinterpret_cast< engine::CImguiUiDemo* >( m_uiPtr )->showTexture( texture );
    }

protected :

    void _initUser() override
    {
        ENGINE_TRACE( "Initializing custom ui" );
        // m_uiPtr = new ApplicationUi( m_windowPtr->context() );
        m_uiPtr = new engine::CImguiUiDemo( m_windowPtr->context() );
        m_uiPtr->init();
    }

};


int main()
{
    auto _app = new Application();
    _app->init();

    auto _textureDataJpg = engine::CTextureManager::GetCachedTextureData( "img_container" );
    auto _textureDataPng = engine::CTextureManager::GetCachedTextureData( "img_smiley" );

    if ( _textureDataJpg )
    {
        std::cout << "texture-data-jpg:" << std::endl;
        std::cout << engine::toString( *_textureDataJpg ) << std::endl;
    }

    if ( _textureDataPng )
    {
        std::cout << "texture-data-png:" << std::endl;
        std::cout << engine::toString( *_textureDataPng ) << std::endl;
    }

    auto _textureContainer = engine::CTextureManager::GetCachedTexture( "img_container" );
    ENGINE_ASSERT( _textureContainer, "Could not retrieve valid texture for the sample - container" );

    auto _textureSmiley = engine::CTextureManager::GetCachedTexture( "img_smiley" );
    ENGINE_ASSERT( _textureContainer, "Could not retrieve valid texture for the sample - smiley" );

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

    auto _shader = engine::CShaderManager::GetCachedShader( "basic2d_textures" );

    _app->setTextureToShow( _textureContainer );

    while( _app->isActive() )
    {
        _app->begin();

        if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _shader->bind();
        _varray->bind();
        // _textureContainer->bind();
        _textureSmiley->bind();

        glDrawElements( GL_TRIANGLES, _varray->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );

        _textureSmiley->unbind();
        // _textureContainer->unbind();
        _varray->unbind();
        _shader->unbind();

        _app->update();
        _app->end();
    }

    return 0;
}
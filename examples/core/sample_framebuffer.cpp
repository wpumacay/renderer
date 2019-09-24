
#include <CEngine.h>

#include <core/CFrameBuffer.h>

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

    _app->setTextureToShow( _textureContainer );

    engine::CFrameBufferConfig _fbColorConfig;
    _fbColorConfig.attachment           = engine::eFboAttachment::COLOR;
    _fbColorConfig.width                = engine::COpenGLApp::GetWindow()->width();
    _fbColorConfig.height               = engine::COpenGLApp::GetWindow()->height();
    _fbColorConfig.texInternalFormat    = engine::eTextureFormat::RGB;
    _fbColorConfig.texFormat            = engine::eTextureFormat::RGB;
    _fbColorConfig.texPixelDataType     = engine::ePixelDataType::UINT_8;

    engine::CFrameBufferConfig _fbDepthConfig;
    _fbDepthConfig.attachment           = engine::eFboAttachment::DEPTH;
    _fbDepthConfig.width                = engine::COpenGLApp::GetWindow()->width();
    _fbDepthConfig.height               = engine::COpenGLApp::GetWindow()->height();
    _fbDepthConfig.texInternalFormat    = engine::eTextureFormat::DEPTH;
    _fbDepthConfig.texFormat            = engine::eTextureFormat::DEPTH;
    _fbDepthConfig.texPixelDataType     = engine::ePixelDataType::UINT_32;

    auto _framebufferColor = new engine::CFrameBuffer( _fbColorConfig );
    ENGINE_INFO( "Created framebuffer for color information" );

    auto _framebufferDepth = new engine::CFrameBuffer( _fbDepthConfig );
    ENGINE_INFO( "Created framebuffer for depth information" );

    while( _app->isActive() )
    {
        _app->begin();

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        // set the render target to our framebuffer

        // let the renderer to its job
        _app->update();

        _app->end();
    }

    return 0;
}
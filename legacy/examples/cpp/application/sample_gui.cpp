
#include <CEngine.h>

class SampleGuiLayer : public engine::CImGuiLayer
{

public :

    SampleGuiLayer( const std::string& name )
        : engine::CImGuiLayer( name ) {}

    void render() override
    {
        ImGui::Begin( "Sample-gui-layer" );

        m_hasFocus = ImGui::IsWindowFocused();
        m_isHovered = ImGui::IsWindowHovered();
        //// ENGINE_TRACE( "has-focus: {0}", m_hasFocus );
        //// ENGINE_TRACE( "is-hovered: {0}", m_isHovered );

        static bool _use_dockspace = false;
        ImGui::Checkbox( "Use dockspace", &_use_dockspace );
        engine::CApplication::GetInstance()->imguiManager()->setDockingSpace( _use_dockspace );

        ImGui::End();
    }

    bool onEvent( const engine::CInputEvent& event ) override
    {
        //// ENGINE_INFO( "Event-Information: \n\r{0}", engine::toString( event ) );
        return false;
    }

private :

    bool m_hasFocus;
    bool m_isHovered;
};

std::unique_ptr<engine::CVertexArray> createGeometry()
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

    auto _vbuffer = std::make_unique<engine::CVertexBuffer>( _layout,
                                                             engine::eBufferUsage::STATIC,
                                                             sizeof( _bufferData ),
                                                             _bufferData );

    auto _ibuffer = std::make_unique<engine::CIndexBuffer>( engine::eBufferUsage::STATIC, 6, _indices );

    auto _varray = std::make_unique<engine::CVertexArray>();
    _varray->addVertexBuffer( std::move( _vbuffer ) );
    _varray->setIndexBuffer( std::move( _ibuffer ) );

    return std::move( _varray );
}

int main()
{
    auto _windowProperties = engine::CWindowProps();
    _windowProperties.width = 1024;
    _windowProperties.height = 768;
    _windowProperties.title = "resizable-application";
    _windowProperties.clearColor = { 0.8f, 0.1f, 0.1f, 1.0f };
    _windowProperties.resizable = true;

    auto _app = std::make_unique<engine::CApplication>( _windowProperties );
    auto _sample_layer = std::make_unique<SampleGuiLayer>( "sample-layer" );
    _app->addGuiLayer( std::move( _sample_layer ) );

    auto _shader = engine::CShaderManager::GetCachedShader( "basic2d_no_textures" );
    auto _geometryVAO = createGeometry();

    while ( _app->active() )
    {
        tinyutils::Clock::Tick();
        if ( engine::CInputManager::IsKeyDown( engine::Keys::KEY_ESCAPE ) )
            break;

        _app->update();
        _app->begin();

        _shader->bind();
        _geometryVAO->bind();

        if ( _geometryVAO->indexBuffer() )
            glDrawElements( GL_TRIANGLES, _geometryVAO->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );
        else
            glDrawArrays( GL_TRIANGLES, 0, 6 );

        _geometryVAO->unbind();
        _shader->unbind();

        _app->render();

        _app->end();
        tinyutils::Clock::Tock();
    }

    return 0;
}

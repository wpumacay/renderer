
#include <window/CWindowGLFW.h>
#include <core/CVertexBuffer.h>
#include <core/CVertexArray.h>
#include <shaders/CShaderManager.h>

int main() {
    tinyutils::Logger::Init();

    auto window_props = engine::CWindowProps();
    window_props.width = 800;
    window_props.height = 600;
    window_props.title = "sample-shaders-c++";
    window_props.resizable = true;
    window_props.clearColor = { 0.2f, 0.4f, 0.8f, 1.0f };

    auto window = engine::CWindowGLFW::Create( window_props );

    const char* vertex_shader = R"(
        #version 330 core

        layout ( location = 0 ) in vec2 pos;
        layout ( location = 1 ) in vec3 color;

        out vec3 frag_color;
        
        void main() 
        {
             gl_Position = vec4( pos, 0.0f, 1.0f );
             frag_color = color;
        }
    )";

    const char* fragment_shader = R"(
        #version 330 core

        in vec3 frag_color;
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4( frag_color, 1.0f );
        }
    )";

    engine::CShaderManager::Init();
    auto shader_program = engine::CShaderManager::CreateProgramFromSources( "basic2d", vertex_shader, fragment_shader );

    engine::float32 buffer_data[] = {
    /*|      pos     |     color      |*/
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 1.0f, 1.0f, 1.0f
    };

    engine::CVertexBufferLayout buffer_layout = { { "pos", engine::eElementType::Float2, false },
                                                  { "color", engine::eElementType::Float3, false } };
    auto buffer = std::make_unique<engine::CVertexBuffer>( buffer_layout, engine::eBufferUsage::STATIC,
                                                           sizeof( buffer_data ), buffer_data );

    auto vertex_array = engine::CVertexArray::Create();
    vertex_array->AddVertexBuffer( std::move( buffer ) );

    while( window->Active() )
    {
        window->Begin();

        shader_program->Bind();
        vertex_array->Bind();

        glDrawArrays( GL_TRIANGLES, 0, 6 );

        vertex_array->Unbind();
        shader_program->Unbind();

        window->End();
    }
    return 0;
}
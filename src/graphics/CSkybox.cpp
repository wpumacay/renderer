
#include <graphics/CSkybox.h>

namespace engine
{

    CSkybox::CSkybox()
    {
        m_cubemap = nullptr;

        /* create the opengl resources required for this skybox */
        float32 _vertices[] =
        {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };

        auto _vBuffer = new CVertexBuffer( { { "position", eElementType::Float3, false } },
                                           eBufferUsage::STATIC,
                                           sizeof( _vertices ),
                                           _vertices );

        m_vertexArray = std::unique_ptr< CVertexArray >( new CVertexArray() );
        m_vertexArray->addVertexBuffer( _vBuffer );
    }

    CSkybox::~CSkybox()
    {
        m_cubemap = nullptr;
    }

    void CSkybox::render()
    {
        if ( !m_cubemap )
            return;

        // render using small optimization of rendering as last object:
        // 
        //  (1) make sure the depth of the skybox fragments are all 1.0, which must be configure 
        //      in the shader by setting z = w = 1.0, such that after persp. division we end up
        //      with depth = 1.0)
        //  (2) make sure we change the depth function such that the skybox fragments pass the test
        //      compared to the default depth buffer values of 1.0 (done here by using GL_LEQUAL)

        glDepthFunc( GL_LEQUAL );

        m_cubemap->bind();
        m_vertexArray->bind();

        glDrawArrays( GL_TRIANGLES, 0, 36 );

        m_cubemap->unbind();
        m_vertexArray->unbind();

        glDepthFunc( GL_LESS );
    }

}
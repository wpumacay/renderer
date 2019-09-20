
#include <LMesh.h>

namespace engine
{

    LMesh::LMesh( const std::vector< CVec3 >& vertices, 
                  const std::vector< CVec3 >& normals,
                  const std::vector< CVec2 >& texCoords,
                  const std::vector< CInd3 >& indices,
                  const eBufferUsage& buffersUsage )
        : LIRenderable()
    {
        m_type = eRenderableType::MESH;

        m_vertices  = vertices;
        m_normals   = normals;
        m_texCoords = texCoords;
        m_indices   = indices;

        m_vbufferVertices = new CVertexBuffer( { { "position", eElementType::Float3, false } },
                                               buffersUsage,
                                               sizeof( CVec3 ) * m_vertices.size(),
                                               (float32*) m_vertices.data() );

        m_vbufferNormals = new CVertexBuffer( { { "normal", eElementType::Float3, true } },
                                              buffersUsage,
                                              sizeof( CVec3 ) * m_normals.size(),
                                              (float32*) m_normals.data() );

        m_vbufferUVs = new CVertexBuffer( { { "texCoord", eElementType::Float2, false } },
                                          buffersUsage,
                                          sizeof( CVec2 ) * m_texCoords.size(),
                                          (float32*) m_texCoords.data() );

        m_ibuffer = new CIndexBuffer( buffersUsage,
                                      3 * m_indices.size(),
                                      (uint32*) m_indices.data() );

        m_varray = new CVertexArray();
        m_varray->addVertexBuffer( m_vbufferVertices );
        m_varray->addVertexBuffer( m_vbufferNormals );
        m_varray->addVertexBuffer( m_vbufferUVs );
        m_varray->setIndexBuffer( m_ibuffer );

        // Create a default material
        m_material = new LMaterial();

        scale = CVec3( 1.0f, 1.0f, 1.0f );
    }

    LMesh::~LMesh()
    {
        if ( m_varray )
            delete m_varray;

        m_varray            = NULL;
        m_vbufferVertices   = NULL;
        m_vbufferNormals    = NULL;
        m_vbufferUVs        = NULL;
        m_ibuffer           = NULL;
    }

    /* @TODO: Implement another way of using the textures. Currently we ...
              are binding each texture for each render call :( */
    void LMesh::render()
    {
        ENGINE_CORE_ASSERT( m_varray, "Mesh should have a valid GL-VAO" );
        ENGINE_CORE_ASSERT( m_varray->indexBuffer(), "Mesh's VAO should have index data" );

        if ( m_drawAsWireframe )
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        for ( LTexture* _texture : m_textures )
            _texture->bind();

        m_varray->bind();

        glDrawElements( GL_TRIANGLES,
                        m_varray->indexBuffer()->count(),
                        GL_UNSIGNED_INT, 0 );

        m_varray->unbind();

        for ( LTexture* _texture : m_textures )
            _texture->unbind();

        if ( m_drawAsWireframe )
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

}
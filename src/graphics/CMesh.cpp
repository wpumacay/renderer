
#include <graphics/CMesh.h>

namespace engine
{

    CMesh::CMesh( const std::string& name,
                  const std::vector< CVec3 >& vertices, 
                  const std::vector< CVec3 >& normals,
                  const std::vector< CVec2 >& texCoords,
                  const std::vector< CInd3 >& indices,
                  const eBufferUsage& buffersUsage )
        : CIRenderable( name )
    {
        m_type = eRenderableType::MESH;

        m_vertices  = vertices;
        m_normals   = normals;
        m_texCoords = texCoords;
        m_indices   = indices;

        m_vbufferVertices = std::unique_ptr< CVertexBuffer >( new CVertexBuffer( { { "position", eElementType::Float3, false } },
                                                                                 buffersUsage,
                                                                                 sizeof( CVec3 ) * m_vertices.size(),
                                                                                 (float32*) m_vertices.data() ) );

        m_vbufferNormals = std::unique_ptr< CVertexBuffer >( new CVertexBuffer( { { "normal", eElementType::Float3, true } },
                                                                                buffersUsage,
                                                                                sizeof( CVec3 ) * m_normals.size(),
                                                                                (float32*) m_normals.data() ) );

        m_vbufferUVs = std::unique_ptr< CVertexBuffer >( new CVertexBuffer( { { "texCoord", eElementType::Float2, false } },
                                                                            buffersUsage,
                                                                            sizeof( CVec2 ) * m_texCoords.size(),
                                                                            (float32*) m_texCoords.data() ) );

        m_ibuffer = std::unique_ptr< CIndexBuffer >( new CIndexBuffer( buffersUsage,
                                                                       3 * m_indices.size(),
                                                                       (uint32*) m_indices.data() ) );

        m_varray = std::unique_ptr< CVertexArray >( new CVertexArray() );
        m_varray->addVertexBuffer( m_vbufferVertices.get() );
        m_varray->addVertexBuffer( m_vbufferNormals.get() );
        m_varray->addVertexBuffer( m_vbufferUVs.get() );
        m_varray->setIndexBuffer( m_ibuffer.get() );
    }

    CMesh::~CMesh()
    {
        m_vbufferVertices   = nullptr;
        m_vbufferNormals    = nullptr;
        m_vbufferUVs        = nullptr;
        m_ibuffer           = nullptr;
    }

    void CMesh::render()
    {
        ENGINE_CORE_ASSERT( m_varray, "Mesh should have a valid GL-VAO" );
        ENGINE_CORE_ASSERT( m_varray->indexBuffer(), "Mesh's VAO should have index data" );

        if ( m_wireframe )
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        m_varray->bind();

        glDrawElements( GL_TRIANGLES,
                        m_varray->indexBuffer()->count(),
                        GL_UNSIGNED_INT, 0 );

        m_varray->unbind();

        if ( m_wireframe )
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

}
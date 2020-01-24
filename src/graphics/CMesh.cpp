
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

        CVertexBufferLayout _vbufferVerticesLayout = { { "position", eElementType::Float3, false } };
        CVertexBufferLayout _vbufferNormalsLayout = { { "normal", eElementType::Float3, true } };
        CVertexBufferLayout _vbufferUVsLayout = { { "texCoord", eElementType::Float2, false } };

        const uint32 _vbufferVerticesSize = sizeof( CVec3 ) * m_vertices.size();
        const uint32 _vbufferNormalsSize = sizeof( CVec3 ) * m_normals.size();
        const uint32 _vbufferUVsSize = sizeof( CVec2 ) * m_texCoords.size();

        float32* _vbufferVerticesData = (float32*) m_vertices.data();
        float32* _vbufferNormalsData = (float32*) m_normals.data();
        float32* _vbufferUVsData = (float32*) m_texCoords.data();

        auto _vbufferVertices = std::make_unique< CVertexBuffer >( _vbufferVerticesLayout, buffersUsage, _vbufferVerticesSize, _vbufferVerticesData );
        auto _vbufferNormals = std::make_unique< CVertexBuffer >( _vbufferNormalsLayout, buffersUsage, _vbufferNormalsSize, _vbufferNormalsData );
        auto _vbufferUVs = std::make_unique< CVertexBuffer >( _vbufferUVsLayout, buffersUsage, _vbufferUVsSize, _vbufferUVsData );

        auto _ibuffer = std::make_unique< CIndexBuffer >( buffersUsage, 3 * m_indices.size(), (uint32*) m_indices.data() );

        m_varray = std::make_unique< CVertexArray >();
        m_varray->addVertexBuffer( std::move( _vbufferVertices ) );
        m_varray->addVertexBuffer( std::move( _vbufferNormals ) );
        m_varray->addVertexBuffer( std::move( _vbufferUVs ) );
        m_varray->setIndexBuffer( std::move( _ibuffer ) );
    }

    CMesh::~CMesh()
    {
        m_varray = nullptr;
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
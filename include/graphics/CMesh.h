#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CVertexBuffer.h>
#include <core/CIndexBuffer.h>
#include <core/CVertexArray.h>
#include <graphics/CIRenderable.h>

namespace engine
{

    class CMesh : public CIRenderable
    {

    public :

        CMesh( const std::string& name,
               const std::vector<CVec3>& vertices, 
               const std::vector<CVec3>& normals,
               const std::vector<CVec2>& texCoords,
               const std::vector<CInd3>& indices,
               const eBufferUsage& buffersUsage = eBufferUsage::STATIC );

        ~CMesh();

        CVertexArray* vertexArray() const { return m_varray.get(); }
        CVertexBuffer* verticesBuffer() const { return m_vbufferVertices.get(); }
        CVertexBuffer* normalsBuffer() const { return m_vbufferNormals.get(); }
        CVertexBuffer* uvsBuffer() const { return m_vbufferUVs.get(); }
        CIndexBuffer* indexBuffer() const { return m_ibuffer.get(); }

        std::vector< CVec3 >& vertices() { return m_vertices; }
        std::vector< CVec3 >& normals() { return m_normals; }
        std::vector< CVec2 >& uvs() { return m_texCoords; }
        std::vector< CInd3 >& indices() { return m_indices; }

        void render() override;

        static eRenderableType GetStaticType() { return eRenderableType::MESH; }

    private :

        std::unique_ptr< CVertexBuffer >    m_vbufferVertices; // owned by mesh's vao
        std::unique_ptr< CVertexBuffer >    m_vbufferNormals; // owned by mesh's vao
        std::unique_ptr< CVertexBuffer >    m_vbufferUVs; // owned by mesh's vao
        std::unique_ptr< CIndexBuffer >     m_ibuffer; // owned by mesh's vao
        std::unique_ptr< CVertexArray>      m_varray; // owned by this mesh

        std::vector<CVec3> m_vertices;
        std::vector<CVec3> m_normals;
        std::vector<CVec2> m_texCoords;
        std::vector<CInd3> m_indices;
    };

}

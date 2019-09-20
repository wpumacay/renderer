
#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CVertexBuffer.h>
#include <core/CIndexBuffer.h>
#include <core/CVertexArray.h>
#include <LIRenderable.h>

namespace engine
{

    class LMesh : public LIRenderable
    {

    public :

        LMesh( const std::vector<CVec3>& vertices, 
               const std::vector<CVec3>& normals,
               const std::vector<CVec2>& texCoords,
               const std::vector<CInd3>& indices,
               const eBufferUsage& buffersUsage = eBufferUsage::STATIC );

        ~LMesh();

        CVertexArray* vertexArray() const { return m_varray; }
        CVertexBuffer* verticesBuffer() const { return m_vbufferVertices; }
        CVertexBuffer* normalsBuffer() const { return m_vbufferNormals; }
        CVertexBuffer* uvsBuffer() const { return m_vbufferUVs; }
        CIndexBuffer* indexBuffer() const { return m_ibuffer; }

        std::vector< CVec3 >& vertices() { return m_vertices; }
        std::vector< CVec3 >& normals() { return m_normals; }
        std::vector< CVec2 >& uvs() { return m_texCoords; }
        std::vector< CInd3 >& indices() { return m_indices; }

        void render() override;

    private :

        CVertexArray*   m_varray;
        CVertexBuffer*  m_vbufferVertices;
        CVertexBuffer*  m_vbufferNormals;
        CVertexBuffer*  m_vbufferUVs;
        CIndexBuffer*   m_ibuffer;

        std::vector<CVec3> m_vertices;
        std::vector<CVec3> m_normals;
        std::vector<CVec2> m_texCoords;
        std::vector<CInd3> m_indices;
    };

}

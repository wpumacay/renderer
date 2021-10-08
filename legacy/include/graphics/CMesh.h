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

        std::unique_ptr<CVertexArray>& vertexArray() { return m_varray; }
        const std::unique_ptr<CVertexArray>& vertexArray() const { return m_varray; }

        std::vector< CVec3 >& vertices() { return m_vertices; }
        std::vector< CVec3 >& normals() { return m_normals; }
        std::vector< CVec2 >& uvs() { return m_texCoords; }
        std::vector< CInd3 >& indices() { return m_indices; }

        static eRenderableType GetStaticType() { return eRenderableType::MESH; }

    protected :

        void _renderInternal() override;

    private :

        std::unique_ptr<CVertexArray> m_varray;

        std::vector<CVec3> m_vertices;
        std::vector<CVec3> m_normals;
        std::vector<CVec2> m_texCoords;
        std::vector<CInd3> m_indices;
    };

}

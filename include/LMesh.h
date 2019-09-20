
#pragma once

#include <CCommon.h>
#include <CMath.h>
#include "LVertexBuffer.h"
#include "LVertexArray.h"
#include "LIndexBuffer.h"
#include "LIRenderable.h"

using namespace std;


namespace engine
{



    class LMesh : public LIRenderable
    {

        private :

        LVertexArray* m_vertexArray;
        LIndexBuffer* m_indexBuffer;

        LVertexBuffer* m_vBuffer;
        LVertexBuffer* m_nBuffer;
        LVertexBuffer* m_tBuffer;
        vector<CVec3> m_vertices;
        vector<CVec3> m_normals;
        vector<CVec2> m_texCoords;
        vector<CInd3> m_indices;

        bool m_usesIndices;

        public :

        LMesh( const vector<CVec3>& vertices,
               const vector<CVec3>& normals );

        LMesh( const vector<CVec3>& vertices, 
               const vector<CVec3>& normals,
               const vector<CInd3>& indices );

        LMesh( const vector<CVec3>& vertices, 
               const vector<CVec3>& normals,
               const vector<CVec2>& texCoords,
               const vector<CInd3>& indices );

        LMesh( const vector<CVec3>& vertices,
               const vector<CVec3>& normals,
               const vector<CVec2>& texCoords );

        ~LMesh();

        LVertexArray* getVertexArray() const { return m_vertexArray; }
        LIndexBuffer* getIndexBuffer() const { return m_indexBuffer; }

        vector< CVec3 >& vertices() { return m_vertices; }
        vector< CInd3 >& indices() { return m_indices; }

        void render() override;
    };













}

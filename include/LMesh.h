
#pragma once

#include "LCommon.h"
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
        vector<LVec3> m_vertices;
        vector<LVec3> m_normals;
        vector<LVec2> m_texCoords;
        vector<LInd3> m_indices;

        bool m_usesIndices;

        public :

        LMesh( const vector<LVec3>& vertices,
               const vector<LVec3>& normals );

        LMesh( const vector<LVec3>& vertices, 
               const vector<LVec3>& normals,
               const vector<LInd3>& indices );

        LMesh( const vector<LVec3>& vertices, 
               const vector<LVec3>& normals,
               const vector<LVec2>& texCoords,
               const vector<LInd3>& indices );

        LMesh( const vector<LVec3>& vertices,
               const vector<LVec3>& normals,
               const vector<LVec2>& texCoords );

        ~LMesh();

        LVertexArray* getVertexArray() const { return m_vertexArray; }
        LIndexBuffer* getIndexBuffer() const { return m_indexBuffer; }

        vector< LVec3 >& vertices() { return m_vertices; }
        vector< LInd3 >& indices() { return m_indices; }

        void render() override;
    };













}

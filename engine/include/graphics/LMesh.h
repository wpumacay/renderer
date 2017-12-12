
#pragma once

#include "LIRenderable.h"

#include "../gl/LVertexBuffer.h"
#include "../gl/LVertexArray.h"
#include "../gl/LIndexBuffer.h"
#include "../material/LMaterial.h"

using namespace std;


namespace engine
{



    class LMesh : public LIRenderable
    {

        private :

        LVertexArray* m_vertexArray;
        LIndexBuffer* m_indexBuffer;
        LMaterial* m_material;

        public :

        string type;

        bool drawAsWireframe;

        LVec3 pos;
        glm::mat4 rotation;
        LVec3 scale;

        LMesh( const vector<LVec3>& vertices, 
               const vector<LVec3>& normals,
               const vector<LInd3>& indices );

        ~LMesh();


        glm::mat4 getModelMatrix();

        void setMaterial( LMaterial* pMaterial );
        LMaterial* getMaterial() const { return m_material; }

        void setTexCoords( const vector<LVec2>& texCoords );

        LVertexArray* getVertexArray() const { return m_vertexArray; }
        LIndexBuffer* getIndexBuffer() const { return m_indexBuffer; }

        void enableLighting() { m_material->enableLighting(); }
        void disableLighting() { m_material->disableLighting(); }

        void render() override;
    };













}

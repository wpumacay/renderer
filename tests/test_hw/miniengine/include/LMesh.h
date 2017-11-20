
#pragma once

#include "../LMiniCommon.h"

#include "LMaterial.h"

using namespace std;

namespace miniengine
{



    class LMesh
    {

        protected :

        LMaterial m_material;

        bool m_lightingEnabled;

        public :

        LVec3 scale;
        LVec3 rot;
        LVec3 pos;

        //#ifdef USE_MODERN_OPENGL

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_vbo_normals;
        GLuint m_ebo;
        GLuint m_programIndx;
        
        LVec3* m_vertices;
        LInd3* m_indices;
        LVec3* m_normals;

        GLuint m_numVertices;
        GLuint m_numIndices;
        GLuint m_numTris;

        //#endif

        LMesh( const vector<LVec3>& vertices,
               const vector<LInd3>& indices,
               const vector<LVec3>& normals );

        ~LMesh();

        void enableLighting() { m_lightingEnabled = true; }
        void disableLighting() { m_lightingEnabled = false; }
        LMaterial& material() { return m_material; }

        void render( const LRenderInfo& rInfo );

        virtual void init();
        virtual void draw();

        void dump()
        {
            cout << "x: " << pos.x << " - y: " << pos.y << " - z: " << pos.z << endl;
            cout << "sx: " << scale.x << " - sy: " << scale.y << " - sz: " << scale.z << endl;
            cout << "mr: " << m_material.ambient.x << " - mg: " << m_material.ambient.y << " - mb: " << m_material.ambient.z << endl;

            // normals
            for ( int q = 0; q < m_numVertices; q++ )
            {
                cout << "n.x: " << m_normals[q].x << " - "
                     << "n.y: " << m_normals[q].y << " - "
                     << "n.z: " << m_normals[q].z << endl;
            }
        }

    };











}
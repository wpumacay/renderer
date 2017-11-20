
#pragma once

#include "../LMiniCommon.h"

#include "LMesh.h"

namespace miniengine
{


    namespace meshType
    {
        enum _meshType
        {
            BOX,
            CYLINDER,
            SPHERE,
            EXTRUSION,
            PLANE
        };
    }

    struct LBuildParams
    {
        // box params
        GLfloat b_width;
        GLfloat b_height;
        GLfloat b_depth;

        // cylinder params
        GLfloat c_baseRadius;
        GLfloat c_height;

        // sphere params
        GLfloat s_radius;

        // extrusion params
        GLfloat* e_pPoints;
        GLuint   e_nPoints;
        GLfloat  e_depth;

        // plane params
        GLfloat p_width;
        GLfloat p_depth;
    };


    class LMeshBuilder
    {

        private :

        static void _compueFaceNormal();
        static void _computeNormalsSingle();
        static void _computeNormalsAvg();

        static LMesh* _createBox( const LBuildParams& pParams );
        static LMesh* _createCylinder( const LBuildParams& pParams );
        static LMesh* _createSphere( const LBuildParams& pParams );
        static LMesh* _createExtrusion( const LBuildParams& pParams );
        static LMesh* _createPlane( const LBuildParams& pParams );

        public :


        static LMesh* createMeshObject( meshType::_meshType pMeshType,
                                        const LBuildParams& pParams );


    };



}

#pragma once

#include <LCommon.h>
#include <LMesh.h>
#include <LModel.h>
#include <utils/CLogger.h>

// Assimp helper functionality
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

namespace engine
{

    struct CMeshBuilder
    {
    public :

        /**
        *   Creates the geometry for a plane with given dimensions along the x, y axes
        *
        *   @param sizeX    extent of the plane in the X axis
        *   @param sizeY    extent of the plane in the Y axis
        */
        static LMesh* createPlane( float sizeX, float sizeY );

        /**
        *   Creates the geometry for a box with given dimensions along the x, y and z axes
        *
        *   @param sizeX    extent of the box in the X axis
        *   @param sizeY    extent of the box in the Y axis
        *   @param sizeZ    extent of the box in the Z axis
        */
        static LMesh* createBox( float sizeX, float sizeY, float sizeZ );

        /**
        *   Creates the geometry for a sphere with given radius and tessellation parameters. The
        *   tessellation is made using just spherical coordinates and mapping the generated vertices,
        *   so the division is made in both theta (first-dim) and phi (second-dim)
        *
        *   @param radius   radius of the sphere
        *   @param nDiv1    number of divisions in the theta dimension
        *   @param nDiv2    number of divisions in the phi dimension
        */
        static LMesh* createSphere( float radius, 
                                    int nDiv1 = 10, 
                                    int nDiv2 = 10 );

        /**
        *   Creates the geometry for an ellipsoid with given axes lengths and tessellation
        *   parameters, similar to the case of 
        *
        *   @param radX     length of the x-axis
        *   @param radY     length of the y-axis
        *   @param radZ     length of the z-axis
        *   @param nDiv1    number of divisions in the theta dimension
        *   @param nDiv2    number of divisions in the phi dimension
        */
        static LMesh* createEllipsoid( float radX, float radY, float radZ,
                                       int nDiv1 = 10,
                                       int nDiv2 = 10 );

        /**
        *   Creates the geometry for a cylinder with given radius, height, and tessellation division
        *
        *   @param radius   radius of the cylinder (radius of top and bottom sections)
        *   @param height   height of the cylinder (distance between top and bottom sections)
        *   @param axis     axis of the cylinder (along the height), either x, y or z axis
        *   @param nDiv1    tessellation parameter for the number of divisions of the parallel sections
        */
        static LMesh* createCylinder( float radius, float height, 
                                      const eAxis& axis = eAxis::Z,
                                      int nDiv1 = 10 );

        static LMesh* createCapsule( float radius, float height, 
                                     const eAxis& axis = eAxis::Z, 
                                     int nDiv1 = 10,
                                     int nDiv2 = 10 );


        static LMesh* createArrow( float length, 
                                   const eAxis& axis = eAxis::Z );

        static LModel* createAxes( float length );

        static LMesh* createPerlinPatch( float width, float depth, int cellDivision, const eAxis& axis = eAxis::Z );

        // static LMesh* createHeightField();

        static LModel* createModelFromFile( const std::string& filename,
                                            const std::string& modelName );

    private :

        static LMesh* _processAssimpMesh( aiMesh* assimpMeshPtr );

        static void _processAssimpNode( LModel* modelPtr, 
                                        aiNode* assimpNodePtr, 
                                        const aiScene* assimpScenePtr );

    private :

        static LVec3 _rotateToMatchUpAxis( const LVec3& vec, const eAxis& axis );

//         static void _constructSmoothCylinderVertices( std::vector< LVec3 >& vertices,
//                                                       std::vector< LVec3 >& normals,
//                                                       std::vector< LVec2 >& uvs,
//                                                       const eAxis& axis = eAxis::Z );
// 
//         static void _constructNonSmoothCylinderVertices( std::vector< LVec3 >& vertices,
//                                                          std::vector< LVec3 >& normals,
//                                                          std::vector< LVec2 >& uvs,
//                                                          const eAxis& axis = eAxis::Z );

    };



}
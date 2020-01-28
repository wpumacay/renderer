#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <utils/CNoiseGenerator.h>
#include <assets/CTextureManager.h>
#include <graphics/CMesh.h>
#include <graphics/CModel.h>

// Assimp helper functionality
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <chrono>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

//// #define USE_HFIELD_FLAT_NORMALS

//// @todo: document functions properly. Just gave it a quick pass for completeness.

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

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
        *   @param axis     normal vector to the plane
        */
        static std::unique_ptr<CMesh> createPlane( float sizeX, float sizeY, const eAxis& axis = eAxis::Z );

        /**
        *   Creates the geometry for a box with given dimensions along the x, y and z axes
        *
        *   @param sizeX    extent of the box in the X axis
        *   @param sizeY    extent of the box in the Y axis
        *   @param sizeZ    extent of the box in the Z axis
        */
        static std::unique_ptr<CMesh> createBox( float sizeX, float sizeY, float sizeZ );

        /**
        *   @brief Creates the geometry for a sphere with given radius and tessellation parameters.
        *
        *   @details
        *   The tessellation is made using just spherical coordinates and mapping the generated vertices,
        *   so the division is made in both theta (first-dim) and phi (second-dim).
        *
        *   @param radius   radius of the sphere
        *   @param nDiv1    number of divisions in the theta dimension
        *   @param nDiv2    number of divisions in the phi dimension
        */
        static std::unique_ptr<CMesh> createSphere( float radius, 
                                                    int nDiv1 = 20, 
                                                    int nDiv2 = 20 );

        /**
        *   @brief Creates the geometry for an ellipsoid with given axes lengths and tessellation parameters.
        *
        *   @param radX     length of the x-axis
        *   @param radY     length of the y-axis
        *   @param radZ     length of the z-axis
        *   @param nDiv1    number of divisions in the theta dimension
        *   @param nDiv2    number of divisions in the phi dimension
        */
        static std::unique_ptr<CMesh> createEllipsoid( float radX, float radY, float radZ,
                                                       int nDiv1 = 20,
                                                       int nDiv2 = 20 );

        /**
        *   @brief Creates the geometry for a cylinder with given radius, height, and tessellation division.
        *
        *   @param radius   radius of the cylinder (radius of top and bottom sections)
        *   @param height   height of the cylinder (distance between top and bottom sections)
        *   @param axis     axis of the cylinder (along the height), either x, y or z axis
        *   @param nDiv1    tessellation parameter for the number of divisions of the parallel sections
        */
        static std::unique_ptr<CMesh> createCylinder( float radius, float height, 
                                                      const eAxis& axis = eAxis::Z,
                                                      int nDiv1 = 30 );

        /**
        *   @brief Creates the geometry for a capsule with given radius, height, and tessellation parameters.
        *
        *   @param radius   radius of the capsule (sections and top/bottom spherical caps)
        *   @param height   height of the capsule (distance between top and bottom sections)
        *   @param axis     axis of the capsule (along the height), either x, y or z axis
        *   @param nDiv1    tessellation parameter for the number of division of the parallel sections
        *   @param nDiv2    tessellation parameter for the number of divisions of the caps (similar to sphere param)
        */
        static std::unique_ptr<CMesh> createCapsule( float radius, float height, 
                                                     const eAxis& axis = eAxis::Z, 
                                                     int nDiv1 = 30,
                                                     int nDiv2 = 30 );

        /**
        *   @brief Creates the geometry for an arrow with given main axis
        *
        *   @details
        *   The resulting arrow is composed of a cylindrical body and cone-shape head with a given length
        *   (other dimensions are scaled accordingly to the length).
        *
        *   @param length   length of the cylindrical part of the arrow (without including head)
        *   @param axis     axis the arrow is pointing to, either x, y or z axis
        */
        static std::unique_ptr<CMesh> createArrow( float length, 
                                                   const eAxis& axis = eAxis::Z );

        /**
        *   @brief Creates the geometry for a set of axis that represent a frame of reference.
        *
        *   @details
        *   The resulting axes (three mutually perpendicular axis) is a model composed of arrows
        *   as submeshes.
        *
        *   @param length   length of each arrow representing an axis of the frame
        */
        static std::unique_ptr<CModel> createAxes( float length );

        /**
        *   @brief Creates the geometry of a patch of terrain generated using a perlin-noise 2d generator.
        *
        *   @param width            full width (x) of the rectangular section
        *   @param depth            full depth (y) of the rectangular section
        *   @param cellDivision     number of cells used to divide the section in a grid
        *   @param nOctaves         octaves-parameter used for the perlin-noise generator
        *   @param nPersistance     persistance-parameter used for the perlin noise generator
        *   @param nLacunarity      lacunarity-parameter used for the perlin noise generator
        *   @param nScale           scale-parameter used for the perlin noise generator
        *   @param axis             up-axis used for this geometry (height direction)
        */
        static std::unique_ptr<CMesh> createPerlinPatch( float width, float depth, int cellDivision, 
                                                         int nOctaves = 4, float nPersistance = 0.5f, 
                                                         float nLacunarity = 2.0f, float nScale = 10.0f, 
                                                         const eAxis& axis = eAxis::Z );

        /**
        *   @brief Creates the geometry of a patch of terrain based on somve given heightfield (elevation) data
        *
        *   @param nWidthSamples    number of points of the generated base grid in the x-direction
        *   @param nDepthSamples    number of points of the generated base grid in the y-direction
        *   @param widthExtent      length of the base in the x-direction
        *   @param depthExtent      length of the base in the y-direction
        *   @param centerX          position of the center of the field's base in the x-direction
        *   @param centerY          position of the center of the field's base in the y-direction
        *   @param heightData       elevation data of each point of the base grid
        *   @param heightBase       additional offset in the (-z) direction (kind of like a pedestal)
        *   @param axis             up axis used for this geometry (height direction)
        */
        static std::unique_ptr<CMesh> createHeightField( int nWidthSamples, int nDepthSamples, 
                                                         float widthExtent, float depthExtent, 
                                                         float centerX, float centerY,
                                                         const std::vector< float >& heightData, float heightBase, 
                                                         const eAxis& axis = eAxis::Z );

        /**
        *   @brief Creates the geometry of a model given the file that contains the model's data.
        *
        *   @param filename     full path to the model data (.stl, .obj, others supported by assimp)
        */
        static std::unique_ptr<CModel> createModelFromFile( const std::string& filename );

    private :

#ifndef DOXYGEN_SHOULD_SKIP_THIS

        static void _processAssimpNode( std::unique_ptr<CModel>& modelPtr, 
                                        aiNode* assimpNodePtr, 
                                        const aiScene* assimpScenePtr,
                                        const std::string& folderPath );
        static std::unique_ptr<CMesh> _processAssimpMesh( std::unique_ptr<CModel>& modelPtr, 
                                                          aiMesh* assimpMeshPtr,
                                                          const aiScene* assimpScenePtr,
                                                          const std::string& folderPath );

        static CVec3 _hfieldComputeSmoothNormal( int i, int j, int nWidthSamples, int nDepthSamples, 
                                                 float dw, float dd, float centerX, float centerY, 
                                                 const std::vector< float >& heightData, 
                                                 const eAxis& axis );

        static CVec3 _rotateToMatchUpAxis( const CVec3& vec, const eAxis& axis );
        static CVec3 _rotateBackFromUpAxis( const CVec3& vec, const eAxis& axis );

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

    private :

        static int32 s_numPlanes;
        static int32 s_numBoxes;
        static int32 s_numSpheres;
        static int32 s_numEllipsoids;
        static int32 s_numCapsules;
        static int32 s_numCylinders;
        static int32 s_numArrows;
        static int32 s_numAxes;
        static int32 s_numPerlinPatches;
        static int32 s_numHeightFields;
        static int32 s_numAssimpModels;
        static int32 s_numAssimpSubmeshes;
    };

}
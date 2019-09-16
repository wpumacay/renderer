
#pragma once

#include <LCommon.h>
#include <LMesh.h>
#include <LModel.h>
#include <utils/CLogger.h>

// Assimp helper functionality
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
                                    int nDiv1 = 20, 
                                    int nDiv2 = 20 );

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
                                       int nDiv1 = 20,
                                       int nDiv2 = 20 );

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
                                      int nDiv1 = 30 );

        /**
        *   Creates the geometry for a capsule with given radius (both section and spherical caps),
        *   height, and tessellation division
        *
        *   @param radius   radius of the capsule (sections and top/bottom spherical caps)
        *   @param height   height of the capsule (distance between top and bottom sections)
        *   @param axis     axis of the capsule (along the height), either x, y or z axis
        *   @param nDiv1    tessellation parameter for the number of division of the parallel sections
        *   @param nDiv2    tessellation parameter for the number of divisions of the caps (similar to sphere param)
        */
        static LMesh* createCapsule( float radius, float height, 
                                     const eAxis& axis = eAxis::Z, 
                                     int nDiv1 = 30,
                                     int nDiv2 = 30 );

        /**
        *   Creates the geometry for an arrow (composed of a cylindrical body and cone-shape head) 
        *   with a given length (other dimensions are scaled accordingly to the length).
        *
        *   @param length   length of the cylindrical part of the arrow (without including head)
        *   @param axis     axis the arrow is pointing to, either x, y or z axis
        */
        static LMesh* createArrow( float length, 
                                   const eAxis& axis = eAxis::Z );

        /**
        *   Creates the geometry for a set of axis that represent a frame of reference (three mutually
        *   perpendicular axes). The returned geometry is a model composed of arrows as submeshes.
        *
        *   @param length   length of each arrow representing an axis of the frame
        */
        static LModel* createAxes( float length );

        /**
        *   Creates the geometry of a patch of terrain generated using a perlin-noise 2d generator. It
        *   consists of a rectangular area in the x-y plane, elevated certain height using a generator.
        *
        *   @param width            full width (x) of the rectangular section
        *   @param depth            full depth (y) of the rectangular section
        *   @param cellDivision     number of cells used to divide the section in a grid
        *   @param axis             up-axis used for this geometry (height direction)
        */
        static LMesh* createPerlinPatch( float width, float depth, int cellDivision, const eAxis& axis = eAxis::Z );

        /**
        *   Creates the geometry of a patch of terrain based on somve given heightfield (elevation) data
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
        static LMesh* createHeightField( int nWidthSamples, int nDepthSamples, 
                                         float widthExtent, float depthExtent, 
                                         float centerX, float centerY,
                                         const std::vector< float >& heightData, float heightBase, 
                                         const eAxis& axis = eAxis::Z );

        /**
        *   Creates the geometry of a model given the file that contains the model's data. The parsing
        *   is done by the Assimp library into a model composed of submeshes.
        *
        *   @param filename     full path to the model data (.stl, .obj, others supported by assimp)
        *   @param modelName    name of the model (for internals identification purposes)
        */
        static LModel* createModelFromFile( const std::string& filename,
                                            const std::string& modelName );

    private :

        static LMesh* _processAssimpMesh( aiMesh* assimpMeshPtr );

        static void _processAssimpNode( LModel* modelPtr, 
                                        aiNode* assimpNodePtr, 
                                        const aiScene* assimpScenePtr );

    private :

        static LVec3 _rotateToMatchUpAxis( const LVec3& vec, const eAxis& axis );
        static LVec3 _rotateBackFromUpAxis( const LVec3& vec, const eAxis& axis );
    };



}
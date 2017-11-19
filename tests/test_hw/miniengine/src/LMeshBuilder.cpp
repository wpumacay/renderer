
#include "../include/LMeshBuilder.h"



using namespace std;

namespace miniengine
{


    LMesh* LMeshBuilder::createMeshObject( meshType::_meshType pMeshType,
                                           const LBuildParams& pParams )
    {
        LMesh* _mesh = NULL;

        switch ( pMeshType )
        {

            case meshType::BOX :
                _mesh = _createBox( pParams );
            break;

            case meshType::CYLINDER :
                _mesh = _createCylinder( pParams );
            break;

            case meshType::SPHERE :
                _mesh = _createSphere( pParams );
            break;

            case meshType::EXTRUSION :
                _mesh = _createExtrusion( pParams );
            break;

            case meshType::PLANE :
                _mesh = _createPlane( pParams );
            break;
        }

        return _mesh;
    }


    LMesh* LMeshBuilder::_createBox( const LBuildParams& pParams )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LInd3> _indices;

        GLfloat _w = pParams.b_width;
        GLfloat _h = pParams.b_height;
        GLfloat _d = pParams.b_depth;

        _vertices.push_back( LVec3( -0.5 * _w,  0.5 *_h,  0.5 * _d ) );
        _vertices.push_back( LVec3( -0.5 * _w, -0.5 *_h,  0.5 * _d ) );
        _vertices.push_back( LVec3(  0.5 * _w, -0.5 *_h,  0.5 * _d ) );
        _vertices.push_back( LVec3(  0.5 * _w,  0.5 *_h,  0.5 * _d ) );

        _vertices.push_back( LVec3( -0.5 * _w,  0.5 *_h, -0.5 * _d ) );
        _vertices.push_back( LVec3( -0.5 * _w, -0.5 *_h, -0.5 * _d ) );
        _vertices.push_back( LVec3(  0.5 * _w, -0.5 *_h, -0.5 * _d ) );
        _vertices.push_back( LVec3(  0.5 * _w,  0.5 *_h, -0.5 * _d ) );

        _indices.push_back( LInd3( 0, 1, 2 ) ); _indices.push_back( LInd3( 2, 3, 0 ) );// face 1
        _indices.push_back( LInd3( 0, 1, 5 ) ); _indices.push_back( LInd3( 5, 4, 0 ) );// face 2
        _indices.push_back( LInd3( 4, 5, 6 ) ); _indices.push_back( LInd3( 6, 7, 4 ) );// face 3
        _indices.push_back( LInd3( 3, 2, 6 ) ); _indices.push_back( LInd3( 6, 7, 3 ) );// face 4
        _indices.push_back( LInd3( 4, 0, 3 ) ); _indices.push_back( LInd3( 3, 7, 4 ) );// face 5
        _indices.push_back( LInd3( 5, 1, 2 ) ); _indices.push_back( LInd3( 2, 6, 5 ) );// face 6

        _mesh = new LMesh( _vertices, _indices );
        _mesh->init();

        return _mesh;
    }

    LMesh* LMeshBuilder::_createCylinder( const LBuildParams& pParams )
    {
        LMesh* _mesh = NULL;






        return _mesh;
    }

    LMesh* LMeshBuilder::_createSphere( const LBuildParams& pParams )
    {

        LMesh* _mesh = NULL;






        return _mesh;

    }

    LMesh* LMeshBuilder::_createExtrusion( const LBuildParams& pParams )
    {
        LMesh* _mesh = NULL;






        return _mesh;
    }
    
    LMesh* LMeshBuilder::_createPlane( const LBuildParams& pParams )
    {
        LMesh* _mesh = NULL;






        return _mesh;
    }









}

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
        vector<LVec3> _normals;
        vector<LInd3> _indices;

        GLfloat _w = pParams.b_width;
        GLfloat _h = pParams.b_height;
        GLfloat _d = pParams.b_depth;

        vector<LVec3> _normalsSource;

        _normalsSource.push_back( LVec3(  0,  0,  1 ) );
        _normalsSource.push_back( LVec3(  0,  0, -1 ) );
        _normalsSource.push_back( LVec3(  0,  1,  0 ) );
        _normalsSource.push_back( LVec3(  0, -1,  0 ) );
        _normalsSource.push_back( LVec3(  1,  0,  0 ) );
        _normalsSource.push_back( LVec3( -1,  0,  0 ) );

        LVec3 _scale( 0.5 * _w, 0.5 * _h, 0.5 * _d );

        // for each face, compute the vertices that form ...
        // the face perpendicular to that normal
        for ( int q = 0; q < _normalsSource.size(); q++ )
        {
            LVec3 _n = _normalsSource[q];

            // form a tri perpendicular right hand system
            LVec3 _s1 = LVec3( _n.y, _n.z, _n.x );
            LVec3 _s2 = LVec3::cross( _n, _s1 );

            // Add the indices accordingly
            _indices.push_back( LInd3( _vertices.size(),
                                       _vertices.size() + 1,
                                       _vertices.size() + 2 ) );

            _indices.push_back( LInd3( _vertices.size(),
                                       _vertices.size() + 2,
                                       _vertices.size() + 3 ) );

            // Generate each vertex of each face ...
            // according to these vectors
            LVec3 _v;

            _v = _n - _s1 - _s2;
            _v.scale( _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );

            _v = _n - _s1 + _s2;
            _v.scale( _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );

            _v = _n + _s1 + _s2;
            _v.scale( _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );

            _v = _n + _s1 - _s2;
            _v.scale( _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );

        }


        _mesh = new LMesh( _vertices, _indices, _normals );
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
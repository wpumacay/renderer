
#include "../include/LMeshBuilder.h"



using namespace std;

namespace miniengine
{

    GLfloat LMeshBuilder::s_letter_w_path[] = { -1.0f,   0.5f,
                                                -0.5f,  -0.5f,
                                                 0.0f,  -0.25f,
                                                 0.5f,  -0.5f,
                                                 1.0f,   0.5f,
                                                 0.5f,   0.5f,
                                                 0.25f,  0.0f,
                                                 0.0f,   0.25f,
                                                -0.25f,  0.0f,
                                                -0.5f,   0.5f };

    GLuint LMeshBuilder::s_letter_w_num = 10;

    GLfloat LMeshBuilder::s_letter_s_path[] = { -0.5f,   0.5f,
                                                -0.5f,  -0.25f,
                                                 0.25f, -0.25f,
                                                 0.25f, -0.5f,
                                                -0.5f,  -0.5f,
                                                -0.5f,  -0.75f,
                                                 0.5f,  -0.75f,
                                                 0.5f,   0.0f,
                                                -0.25f,  0.0f,
                                                -0.25f,  0.25f,
                                                 0.5f,   0.25f,
                                                 0.5f,   0.5f };

    GLuint LMeshBuilder::s_letter_s_num = 12;

    GLfloat LMeshBuilder::s_letter_p_path[] = { -0.5f,   0.5f,
                                                -0.5f,  -1.0f,
                                                -0.25f, -1.0f,
                                                -0.25f,  0.25f,
                                                 0.25f,  0.25f,
                                                 0.25f, -0.25f,
                                                -0.25f, -0.25f,
                                                -0.25f, -0.5f,
                                                 0.5f,  -0.5f,
                                                 0.5f,   0.5f };

    GLuint LMeshBuilder::s_letter_p_num = 10;

    GLfloat LMeshBuilder::s_letter_h_path[] = { -1.0f,   1.0f,
                                                -1.0f,  -1.0f,
                                                -0.5f,  -1.0f,
                                                -0.5f,  -0.25f,
                                                 0.5f,  -0.25f,
                                                 0.5f,  -1.0f,
                                                 1.0f,  -1.0f,
                                                 1.0f,   1.0f,
                                                 0.5f,   1.0f,
                                                 0.5f,   0.25f,
                                                -0.5f,   0.25f,
                                                -0.5f,   1.0f };

    GLuint LMeshBuilder::s_letter_h_num = 12;


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

        LPolygon _polyPath;

        GLuint _n_points = pParams.e_nPoints;
        GLfloat* _p_points = pParams.e_pPoints;
        GLfloat _depth = pParams.e_depth;

        for ( int q = 0; q < _n_points; q++ )
        {
            GLfloat _x = _p_points[2 * q];
            GLfloat _y = _p_points[2 * q + 1];

            _polyPath.addPoint( LVec2( _x, _y ) );
        }

        cout << "LMeshBuilder::_createExtrusion> triangulating" << endl;
        
        _polyPath.earClipTriangulate();
        
        cout << "LMeshBuilder::_createExtrusion> triangulating - DONE" << endl;

        vector<LInd3> _indices;
        vector<LVec3> _vertices;
        vector<LVec3> _normals;

        vector<LTri> _tris = _polyPath.tris();
        vector<LVec2> _v2d = _polyPath.points();

        GLuint _vertexSoFar = _vertices.size();

        // Create the front face

        for ( int q = 0; q < _tris.size(); q++ )
        {
            _indices.push_back( LInd3( _tris[q].i1 + _vertexSoFar, 
                                       _tris[q].i2 + _vertexSoFar, 
                                       _tris[q].i3 + _vertexSoFar ) );
        }

        for ( int q = 0; q < _v2d.size(); q++ )
        {
            LVec3 _v( _v2d[q].x, _v2d[q].y, 0.5 * _depth );
            LVec3 _n( 0.0f, 0.0f, 1.0f );

            //int _i = q;
            //int _j = ( q + 1 ) % _v2d.size();

            //LVec2 _t = _v2d[_j] - _v2d[_i];
            //LVec3 _n( _t.y, -_t.x, 0.0f );

            _vertices.push_back( _v );
            _normals.push_back( _n );
        }

        _vertexSoFar = _vertices.size();
        GLuint _faceNVertex = _vertexSoFar;

        // Create the back face

        for ( int q = 0; q < _tris.size(); q++ )
        {
            _indices.push_back( LInd3( _tris[q].i1 + _vertexSoFar, 
                                       _tris[q].i2 + _vertexSoFar, 
                                       _tris[q].i3 + _vertexSoFar ) );
        }

        for ( int q = 0; q < _v2d.size(); q++ )
        {
            LVec3 _v( _v2d[q].x, _v2d[q].y, -0.5 * _depth );
            LVec3 _n( 0.0f, 0.0f, -1.0f );

            _vertices.push_back( _v );
            _normals.push_back( _n );
        }

        _vertexSoFar = _vertices.size();

        // Create the sides

        for ( int q = 0; q < _v2d.size(); q++ )
        {
            int _i_front = q;
            int _j_front = ( q + 1 ) % _v2d.size();
            int _i_back = _i_front + _faceNVertex;
            int _j_back = _j_front + _faceNVertex;

            LVec2 _t = _v2d[_j_front] - _v2d[_i_front];
            LVec3 _n( _t.y, -_t.x, 0.0f );

            LVec3 _vif = LVec3( _v2d[_i_front].x, _v2d[_i_front].y, 0.5 * _depth );
            LVec3 _vjf = LVec3( _v2d[_j_front].x, _v2d[_j_front].y, 0.5 * _depth );
            LVec3 _vib = LVec3( _v2d[_i_front].x, _v2d[_i_front].y, -0.5 * _depth );
            LVec3 _vjb = LVec3( _v2d[_j_front].x, _v2d[_j_front].y, -0.5 * _depth );

            _indices.push_back( LInd3( _vertices.size(),
                                       _vertices.size() + 1,
                                       _vertices.size() + 3 ) );
            _indices.push_back( LInd3( _vertices.size() + 3,
                                       _vertices.size() + 1,
                                       _vertices.size() + 2 ) );

            _vertices.push_back( _vif ); 
            _vertices.push_back( _vjf );
            _vertices.push_back( _vjb ); 
            _vertices.push_back( _vib );

            _normals.push_back( _n );
            _normals.push_back( _n );
            _normals.push_back( _n );
            _normals.push_back( _n );
        }


        _mesh = new LMesh( _vertices, _indices, _normals );
        _mesh->init();

        return _mesh;
    }
    
    LMesh* LMeshBuilder::_createPlane( const LBuildParams& pParams )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LInd3> _indices;

        GLfloat _w = pParams.p_width;
        GLfloat _d = pParams.p_depth;

        LVec3 _n = LVec3( 0.0f, 1.0f, 0.0f );
        LVec3 _s1 = LVec3( 0.0f, 0.0f, 1.0f );
        LVec3 _s2 = LVec3::cross( 1.0f, 0.0f, 0.0f );

        LVec3 _scale = LVec3( 0.5 * _w, 0.0f, 0.5 * _d );

        _indices.push_back( LInd3( _vertices.size(),
                                   _vertices.size() + 1,
                                   _vertices.size() + 2 ) );

        _indices.push_back( LInd3( _vertices.size(),
                                   _vertices.size() + 2,
                                   _vertices.size() + 3 ) );

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

        _mesh = new LMesh( _vertices, _indices, _normals );
        _mesh->init();

        return _mesh;
    }









}
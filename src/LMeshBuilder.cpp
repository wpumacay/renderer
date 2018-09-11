
#include "LMeshBuilder.h"
#include "LHeightmapGenerator.h"

#include <map>

using namespace std;

namespace engine
{
    LMesh* LMeshBuilder::createSphere( GLfloat radius, int levelDivision , int numLevels )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LInd3> _indices;

        // adapted from here :
        // https://www.opengl.org/discussion_boards/showthread.php/159584-sphere-generation

        // make vertices
        GLfloat _x, _y, _z, _r;

        for ( int l = -numLevels; l <= numLevels; l++ )
        {
            // _y = ( ( float )l ) / ( numLevels + 1 );
            _y = sin( 0.5 * _PI * ( ( float )l ) / ( numLevels + 1 ) );

            for ( int d = 0; d < levelDivision; d++ )
            {
                _r = sqrt( 1.0f - _y * _y );
                _x = _r * cos( 2.0f * _PI * ( ( float ) d ) / levelDivision );
                _z = _r * sin( 2.0f * _PI * ( ( float ) d ) / levelDivision );

                _vertices.push_back( LVec3( radius * _x, radius * _y, radius * _z ) );
                _normals.push_back( LVec3( _x, _y, _z ) );
            }
        }

        for ( int l = 0; l < 2 * numLevels; l++ )
        {
            int _vl = l * levelDivision;
            int _vlNext = ( l + 1 ) * levelDivision;

            // Connect sides
            for ( int s = 0; s < levelDivision; s++ )
            {
                int _p0 = _vl + s;
                int _p1 = _vl + ( ( s + 1 ) % levelDivision );
                int _p0n = _vlNext + s;
                int _p1n = _vlNext + ( ( s + 1 ) % levelDivision );

                _indices.push_back( LInd3( _p0, _p1n, _p0n ) );
                _indices.push_back( LInd3( _p0, _p1, _p1n ) );
            }
        }

        _mesh = new LMesh( _vertices, _normals, _indices );

        return _mesh;
    }


    LMesh* LMeshBuilder::createBox( GLfloat width, GLfloat height, GLfloat depth )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LInd3> _indices;

        vector<LVec3> _normalsSource;

        _normalsSource.push_back( LVec3(  0,  0,  1 ) );
        _normalsSource.push_back( LVec3(  0,  0, -1 ) );
        _normalsSource.push_back( LVec3(  0,  1,  0 ) );
        _normalsSource.push_back( LVec3(  0, -1,  0 ) );
        _normalsSource.push_back( LVec3(  1,  0,  0 ) );
        _normalsSource.push_back( LVec3( -1,  0,  0 ) );

        LVec3 _scale( 0.5 * width, 0.5 * height, 0.5 * depth );

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


        _mesh = new LMesh( _vertices, _normals, _indices );

        return _mesh;
    }

    LMesh* LMeshBuilder::createCylinder( GLfloat radius, GLfloat height, int sectionDivision )
    {
        LMesh* _mesh = NULL;

        vector< LVec3 > _vertices;
        vector< LVec3 > _normals;
        vector< LInd3 > _indices;

        // Start cylinder tessellation

        // calculate section geometry
        vector< LVec3 > _sectionXZ;

        float _stepSectionAngle = 2 * _PI / sectionDivision;

        for ( int q = 0; q < sectionDivision; q++ )
        {
            float _x = radius * cos( q * _stepSectionAngle );
            float _z = radius * sin( q * _stepSectionAngle );

            _sectionXZ.push_back( LVec3( _x, 0, _z ) );
        }

        // calculate cylinder geometry
        int _baseIndx = 0;
        // up base
        for ( int q = 0; q < _sectionXZ.size(); q++ )
        {
            _vertices.push_back( _sectionXZ[q] + LVec3( 0, 0.5 * height, 0 ) );
            _normals.push_back( LVec3( 0, 1, 0 ) );
        }
        for ( int q = 1; q <= _sectionXZ.size() - 2; q++ )
        {
            _indices.push_back( LInd3( _baseIndx, _baseIndx + q, _baseIndx + q + 1 ) );
        }
        _baseIndx += _vertices.size();
        // body surface
        for ( int q = 0; q < _sectionXZ.size(); q++ )
        {
            // quad vertices
            auto _p0 = _sectionXZ[q] + LVec3( 0, 0.5 * height, 0 );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0.5 * height, 0 );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, -0.5 * height, 0 );
            auto _p3 = _sectionXZ[q] + LVec3( 0, -0.5 * height, 0 );

            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );
            _vertices.push_back( _p3 );

            float _nx = cos( ( q + 0.5 ) * _stepSectionAngle );
            float _nz = sin( ( q + 0.5 ) * _stepSectionAngle );

            auto _nQuad = LVec3( _nx, 0, _nz );

            _normals.push_back( _nQuad );
            _normals.push_back( _nQuad );
            _normals.push_back( _nQuad );
            _normals.push_back( _nQuad );

            _indices.push_back( LInd3( _baseIndx, _baseIndx + 2, _baseIndx + 1 ) );
            _indices.push_back( LInd3( _baseIndx, _baseIndx + 3, _baseIndx + 2 ) );

            _baseIndx += 4;
        }
        // down base
        for ( int q = 0; q < _sectionXZ.size(); q++ )
        {
            _vertices.push_back( _sectionXZ[q] + LVec3( 0, -0.5 * height, 0 ) );
            _normals.push_back( LVec3( 0, -1, 0 ) );
        }
        _baseIndx += _sectionXZ.size();
        for ( int q = 1; q <= _sectionXZ.size() - 2; q++ )
        {
            _indices.push_back( LInd3( _baseIndx, _baseIndx + q + 1, _baseIndx + q ) );
        }

        _mesh = new LMesh( _vertices, _normals, _indices );

        return _mesh;
    }

    LMesh* LMeshBuilder::createCapsule( GLfloat radius, GLfloat height, int sectionDivision, int capLevels )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LInd3> _indices;

        // Tessellate using cap-surface-cap approach

        // Build up cap *********************************
        // make vertices
        GLfloat _x, _y, _z, _r;
        int _baseIndx = 0;
        for ( int l = 0; l <= capLevels; l++ )
        {
            // _y = ( ( float )l ) / ( numLevels + 1 );
            _y = sin( 0.5 * _PI * ( ( float ) l ) / ( capLevels + 1 ) );

            for ( int d = 0; d < sectionDivision; d++ )
            {
                _r = sqrt( 1.0f - _y * _y );
                _x = _r * cos( 2.0f * _PI * ( ( float ) d ) / sectionDivision );
                _z = _r * sin( 2.0f * _PI * ( ( float ) d ) / sectionDivision );

                auto _upOffset = LVec3( 0, 0.5 * height, 0 );

                _vertices.push_back( LVec3( radius * _x, radius * _y, radius * _z ) + _upOffset );
                _normals.push_back( LVec3( _x, _y, _z ) );
            }
        }

        for ( int l = 0; l < capLevels; l++ )
        {
            int _vl = l * sectionDivision;
            int _vlNext = ( l + 1 ) * sectionDivision;

            // Connect sides
            for ( int s = 0; s < sectionDivision; s++ )
            {
                int _p0 = _baseIndx + _vl + s;
                int _p1 = _baseIndx + _vl + ( ( s + 1 ) % sectionDivision );
                int _p0n = _baseIndx + _vlNext + s;
                int _p1n = _baseIndx + _vlNext + ( ( s + 1 ) % sectionDivision );

                _indices.push_back( LInd3( _p0, _p1n, _p0n ) );
                _indices.push_back( LInd3( _p0, _p1, _p1n ) );
            }
        }
        _baseIndx += _vertices.size();
        // Build surface *******************************

        // calculate section geometry
        vector< LVec3 > _sectionXZ;

        float _stepSectionAngle = 2 * _PI / sectionDivision;

        for ( int q = 0; q < sectionDivision; q++ )
        {
            float _x = radius * cos( q * _stepSectionAngle );
            float _z = radius * sin( q * _stepSectionAngle );

            _sectionXZ.push_back( LVec3( _x, 0, _z ) );
        }

        // body surface
        
        for ( int q = 0; q < _sectionXZ.size(); q++ )
        {
            // quad vertices
            auto _p0 = _sectionXZ[q] + LVec3( 0, 0.5 * height, 0 );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0.5 * height, 0 );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, -0.5 * height, 0 );
            auto _p3 = _sectionXZ[q] + LVec3( 0, -0.5 * height, 0 );

            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );
            _vertices.push_back( _p3 );

            float _nx = cos( ( q + 0.5 ) * _stepSectionAngle );
            float _nz = sin( ( q + 0.5 ) * _stepSectionAngle );

            auto _nQuad = LVec3( _nx, 0, _nz );

            _normals.push_back( _nQuad );
            _normals.push_back( _nQuad );
            _normals.push_back( _nQuad );
            _normals.push_back( _nQuad );

            _indices.push_back( LInd3( _baseIndx, _baseIndx + 2, _baseIndx + 1 ) );
            _indices.push_back( LInd3( _baseIndx, _baseIndx + 3, _baseIndx + 2 ) );

            _baseIndx += 4;
        }


        // Build down cap ******************************
        // make vertices

        for ( int l = -capLevels; l <= 0; l++ )
        {
            // _y = ( ( float )l ) / ( numLevels + 1 );
            _y = sin( 0.5 * _PI * ( ( float ) l ) / ( capLevels + 1 ) );

            for ( int d = 0; d < sectionDivision; d++ )
            {
                _r = sqrt( 1.0f - _y * _y );
                _x = _r * cos( 2.0f * _PI * ( ( float ) d ) / sectionDivision );
                _z = _r * sin( 2.0f * _PI * ( ( float ) d ) / sectionDivision );

                auto _downOffset = LVec3( 0, -0.5 * height, 0 );

                _vertices.push_back( LVec3( radius * _x, radius * _y, radius * _z ) + _downOffset );
                _normals.push_back( LVec3( _x, _y, _z ) );
            }
        }

        for ( int l = 0; l < capLevels; l++ )
        {
            int _vl = l * sectionDivision;
            int _vlNext = ( l + 1 ) * sectionDivision;

            // Connect sides
            for ( int s = 0; s < sectionDivision; s++ )
            {
                int _p0 = _baseIndx + _vl + s;
                int _p1 = _baseIndx + _vl + ( ( s + 1 ) % sectionDivision );
                int _p0n = _baseIndx + _vlNext + s;
                int _p1n = _baseIndx + _vlNext + ( ( s + 1 ) % sectionDivision );

                _indices.push_back( LInd3( _p0, _p1n, _p0n ) );
                _indices.push_back( LInd3( _p0, _p1, _p1n ) );
            }
        }

        _mesh = new LMesh( _vertices, _normals, _indices );

        return _mesh;
    }

    LMesh* LMeshBuilder::createPlane( GLfloat width, GLfloat depth )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LInd3> _indices;


        LVec3 _n = LVec3( 0.0f, 1.0f, 0.0f );
        LVec3 _s1 = LVec3( 0.0f, 0.0f, 1.0f );
        LVec3 _s2 = LVec3( 1.0f, 0.0f, 0.0f );

        LVec3 _scale = LVec3( 0.5 * width, 0.0f, 0.5 * depth );

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

        _mesh = new LMesh( _vertices, _normals, _indices );

        return _mesh;
    }


    LMesh* LMeshBuilder::createFromFile( const char* filename )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LInd3> _indices;

        ifstream _fileHandle( filename );

        if ( !_fileHandle.is_open() )
        {
            cout << "LMeshBuilder::createFromFile> couldn't open the file " << filename << endl;
            return NULL;
        }

        string _line;

        getline( _fileHandle, _line );

        int _numFaces = stoi( _line );

        for ( int q = 0; q < _numFaces; q++ )
        {
            getline( _fileHandle, _line );

            int _numVertices = stoi( _line );

            for ( int p = 0; p < _numVertices; p++ )
            {
                getline( _fileHandle, _line );
                vector<string> _vStr = LMeshBuilder::_split( _line, ' ' );

                GLfloat _vx = stof( _vStr[0] );
                GLfloat _vy = stof( _vStr[1] );
                GLfloat _vz = stof( _vStr[2] );

                _vertices.push_back( LVec3( _vx, _vy, _vz ) );
            }

            int _i1 = _indices.size() * 3;
            int _i2 = _indices.size() * 3 + 1;
            int _i3 = _indices.size() * 3 + 2;

            _indices.push_back( LInd3( _i1, _i2, _i3 ) );

            LVec3 _n = _computeFaceNormal( _vertices[_i1], 
                                           _vertices[_i2], 
                                           _vertices[_i3] );
            _normals.push_back( _n );
            _normals.push_back( _n );
            _normals.push_back( _n );
        }

        _mesh = new LMesh( _vertices, _normals, _indices );
        
        return _mesh;
    }


    LMesh* LMeshBuilder::createFromObj( const char* filename )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LVec2> _texCoord;
        vector<LInd3> _indices;

        ifstream _fileHandle( filename );

        if ( !_fileHandle.is_open() )
        {
            cout << "LMeshBuilder::createFromObj> couldn't open the file " << filename << endl;
            return NULL;
        }

        LObjInfo _objInfo;

        _parseObj( _fileHandle, _objInfo );

        _mesh = new LMesh( _objInfo.vertices, _objInfo.normals, _objInfo.texCoords );
        
        return _mesh;
    }

    /***********************************************************************
    * HELPER FUNCTIONS
    ************************************************************************/


    LVec3 LMeshBuilder::_computeFaceNormal( LVec3 v1, LVec3 v2, LVec3 v3, bool normalize )
    {
        LVec3 _res;

        LVec3 _v12 = v2 - v1;
        LVec3 _v23 = v3 - v2;

        _res = LVec3::cross( _v12, _v23 );
        if ( normalize )
        {
            _res.normalize();
        }

        return _res;
    }

    vector<string> LMeshBuilder::_split( const string& txt, char separator )
    {
        vector<string> _res;
                    
        int pos = txt.find( separator );
        if ( pos == std::string::npos )
        {
            _res.push_back( txt );
            return _res;
        }

        int initpos = 0;

        while ( pos != std::string::npos )
        {
            _res.push_back( txt.substr( initpos, pos - initpos ) );
            initpos = pos + 1;

            pos = txt.find( separator, initpos );
        }

        _res.push_back( txt.substr( initpos, std::min( pos, (int) txt.size() ) - initpos ) );
                    
        return _res;
    }

    void LMeshBuilder::_parseObj( ifstream& fileHandle,
                                  LObjInfo& obj )
    {
        cout << "parsing geometry" << endl;

        map<string, LObjGeometryInfo> _objects;

        string _currentObjectName;

        string _materialsFile;
        string _line;

        while ( getline( fileHandle, _line ) )
        {
            vector<string> _vStr = _split( _line, ' ' );
            // cout << "??? " << _vStr[0] << endl;

            if ( _vStr[0] == OBJ_COMMENT )
            {
                continue;
            }
            else if ( _vStr[0] == OBJ_MATERIAL_LIB )
            {
                obj.materialFile = _vStr[1];
            }
            else if ( _vStr[0] == OBJ_OBJECT_NAME )
            {
                _currentObjectName = _vStr[1];
                _objects[_currentObjectName] = LObjGeometryInfo();
            }
            else if ( _vStr[0] == OBJ_GROUP_NAME )
            {
                // cout << "obj model loader doesn't support group names yet" << endl;
                // assert( false );
                continue;
            }
            else if ( _vStr[0] == OBJ_VERTEX )
            {
                if ( _currentObjectName == "" )
                {
                    _currentObjectName = "default";
                    _objects[_currentObjectName] = LObjGeometryInfo();
                }

                LVec3 _v( stof( _vStr[1] ), stof( _vStr[2] ), stof( _vStr[3] ) );

                _objects[_currentObjectName].vertices.push_back( _v );
            }
            else if ( _vStr[0] == OBJ_NORMAL )
            {
                LVec3 _n( stof( _vStr[1] ), stof( _vStr[2] ), stof( _vStr[3] ) );

                _objects[_currentObjectName].normals.push_back( _n );
            }
            else if ( _vStr[0] == OBJ_TEXTURE )
            {
                LVec2 _t( stof( _vStr[1] ), stof( _vStr[2] ) );

                _objects[_currentObjectName].texCoords.push_back( _t );
            }
            else if ( _vStr[0] == OBJ_MATERIAL_ID )
            {
                continue;// skip, no material id support yet
            }
            else if ( _vStr[0] == OBJ_SMOOTH_SHADING )
            {
                continue;
            }
            else if ( _vStr[0] == OBJ_FACE )
            {
                for ( int q = 1; q < 4; q++ )
                {
                    vector<string> _vVertex = _split( _vStr[q], '/' );

                    obj.vertices.push_back( _objects[_currentObjectName].vertices[ stoi( _vVertex[0] ) - 1 ] );
                    obj.texCoords.push_back( _objects[_currentObjectName].texCoords[ stoi( _vVertex[1] ) - 1 ] );
                    obj.normals.push_back( _objects[_currentObjectName].normals[ stoi( _vVertex[2] ) - 1 ] );
                }
            }
            else
            {
                // cout << "not supported: " << _vStr[0] << endl;
                continue;
            }
        }

        cout << "vl: " << obj.vertices.size() << endl;
        cout << "nl: " << obj.normals.size() << endl;
        cout << "tl: " << obj.texCoords.size() << endl;

        cout << "done geometry" << endl;
    }

    LMesh* LMeshBuilder::createPerlinPatch( GLfloat width, GLfloat depth, int cellDivision )
    {
        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LInd3> _indices;
        vector<LVec2> _texCoord;

        LHeightmapGenerator _hmapGenerator;

        float _dw = width / cellDivision;
        float _dd = depth / cellDivision;

        float _x0 = -( cellDivision / 2.0f ) * _dw;
        float _z0 = -( cellDivision / 2.0f ) * _dd;

        cout << "x0: " << _x0 << endl;
        cout << "z0: " << _z0 << endl;
        cout << "dw: " << _dw << endl;
        cout << "dd: " << _dd << endl;

        for ( int i = 0; i < cellDivision; i++ )
        {
            for ( int j = 0; j < cellDivision; j++ )
            {
                LVec3 _p0( _x0 + j * _dw, 
                           2.0f + 2.0f * _hmapGenerator.getHeight( _x0 + j * _dw, _z0 + i * _dd ),
                           _z0 + i * _dd );

                LVec3 _p1( _x0 + j * _dw,
                           2.0f + 2.0f * _hmapGenerator.getHeight( _x0 + j * _dw, _z0 + ( i + 1 ) * _dd ),
                           _z0 + ( i + 1 ) * _dd );

                LVec3 _p2( _x0 + ( j + 1 ) * _dw,
                           2.0f + 2.0f * _hmapGenerator.getHeight( _x0 + ( j + 1 ) * _dw, _z0 + ( i + 1 ) * _dd ),
                           _z0 + ( i + 1 ) * _dd );

                LVec3 _p3( _x0 + ( j + 1 ) * _dw,
                           2.0f + 2.0f * _hmapGenerator.getHeight( _x0 + ( j + 1 ) * _dw, _z0 + i * _dd ),
                           _z0 + i * _dd );

                // TODO: For now just make dummy indices for the rigid bodies :/
                _indices.push_back( LInd3( _vertices.size() + 0,
                                           _vertices.size() + 1,
                                           _vertices.size() + 2 ) );

                _indices.push_back( LInd3( _vertices.size() + 3,
                                           _vertices.size() + 4,
                                           _vertices.size() + 5 ) );

                LVec2 _t0( 0, 0 );
                LVec2 _t1( 0, 1 );
                LVec2 _t2( 1, 1 );
                LVec2 _t3( 1, 0 );

                _texCoord.push_back( _t0 );
                _texCoord.push_back( _t1 );
                _texCoord.push_back( _t2 );
                _texCoord.push_back( _t0 );
                _texCoord.push_back( _t2 );
                _texCoord.push_back( _t3 );

                _vertices.push_back( _p0 );
                _vertices.push_back( _p1 );
                _vertices.push_back( _p2 );

                _vertices.push_back( _p0 );
                _vertices.push_back( _p2 );
                _vertices.push_back( _p3 );

                LVec3 _nt1 = LVec3::cross( _p1 - _p0, _p2 - _p1 );
                LVec3 _nt2 = LVec3::cross( _p2 - _p0, _p3 - _p2 );

                _normals.push_back( _nt1 );
                _normals.push_back( _nt1 );
                _normals.push_back( _nt1 );

                _normals.push_back( _nt2 );
                _normals.push_back( _nt2 );
                _normals.push_back( _nt2 );
            }
        }

        // Smooth the normals
        for ( int q = 0; q < _vertices.size(); q++ )
        {
            // calculate the necessary neighbouring points
            /*
            *   1---2---3
            *   |   |   |
            *   4---v---6
            *   |   |   |
            *   7---8---9
            */

            LVec3 _pv = _vertices[q];
            LVec3 _p1( _pv.x - _dw, 
                       2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x - _dw, _pv.z - _dd ),
                       _pv.z - _dd );
            LVec3 _p2( _pv.x, 
                       2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x, _pv.z - _dd ),
                       _pv.z - _dd );
            LVec3 _p3( _pv.x + _dw, 
                       2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x + _dw, _pv.z - _dd ),
                       _pv.z - _dd );
            LVec3 _p4( _pv.x - _dw, 
                       2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x - _dw, _pv.z ),
                       _pv.z );
            LVec3 _p6( _pv.x + _dw, 
                       2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x + _dw, _pv.z ),
                       _pv.z );
            LVec3 _p7( _pv.x - _dw, 
                       2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x - _dw, _pv.z + _dd ),
                       _pv.z + _dd );
            LVec3 _p8( _pv.x, 
                       2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x, _pv.z + _dd ),
                       _pv.z + _dd );
            LVec3 _p9( _pv.x + _dw, 
                       2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x + _dw, _pv.z + _dd ),
                       _pv.z + _dd );

            LVec3 _n;

            _n = _n + LVec3::cross( _p4 - _p1, _pv - _p4 );
            _n = _n + LVec3::cross( _pv - _p1, _p2 - _pv );

            _n = _n + LVec3::cross( _pv - _p2, _p6 - _pv );
            _n = _n + LVec3::cross( _p8 - _p4, _pv - _p8 );

            _n = _n + LVec3::cross( _p8 - _pv, _p9 - _p8 );
            _n = _n + LVec3::cross( _p9 - _pv, _p6 - _p9 );

            _n.normalize();

            _normals[q] = _n;
        }

        return new LMesh( _vertices, _normals, _indices, _texCoord );
    }

}
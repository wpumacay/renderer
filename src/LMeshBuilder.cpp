
#include "LMeshBuilder.h"
#include "LHeightmapGenerator.h"

#include <map>

using namespace std;

// @TODO: Refactor this part, as some portions are being repeated over and over (common tesselations)

namespace engine
{
    LMesh* LMeshBuilder::createSphere( GLfloat radius, int levelDivision , int numLevels )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LVec2> _texCoords;
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

                // from here: https://en.wikipedia.org/wiki/UV_mapping#Finding_UV_on_a_sphere

                float _u = 0.5 + ( atan2( -_z, -_x ) / ( 2 * _PI ) );
                float _v = 0.5 - ( asin( -_y ) / _PI );


                _texCoords.push_back( LVec2( _u, _v ) );
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

        _mesh = new LMesh( _vertices, _normals, _texCoords, _indices );

        return _mesh;
    }


    LMesh* LMeshBuilder::createBox( GLfloat width, GLfloat height, GLfloat depth )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LVec2> _texCoords;
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
        // #if AXIS_X == 1
        //     LVec3 _s1 = LVec3( _n.z, _n.x, _n.y );
        //     LVec3 _s2 = LVec3::cross( _n, _s1 );
        // #elif AXIS_Y == 1
        //     LVec3 _s1 = LVec3( _n.y, _n.z, _n.x );
        //     LVec3 _s2 = LVec3::cross( _n, _s1 );
        // #elif AXIS_Z == 1
        //     LVec3 _s1 = LVec3( _n.x, _n.y, _n.z );
        //     LVec3 _s2 = LVec3::cross( _n, _s1 );
        // #else
            LVec3 _s1 = LVec3( _n.y, _n.z, _n.x );
            LVec3 _s2 = LVec3::cross( _n, _s1 );
        // #endif

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
            _texCoords.push_back( LVec2( 1, 1 ) );

            _v = _n - _s1 + _s2;
            _v.scale( _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );
            _texCoords.push_back( LVec2( 1, 0 ) );

            _v = _n + _s1 + _s2;
            _v.scale( _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );
            _texCoords.push_back( LVec2( 0, 0 ) );

            _v = _n + _s1 - _s2;
            _v.scale( _scale.x, _scale.y, _scale.z );

            _vertices.push_back( _v );
            _normals.push_back( _n );
            _texCoords.push_back( LVec2( 0, 1 ) );

        }


        _mesh = new LMesh( _vertices, _normals, _texCoords, _indices );

        return _mesh;
    }

    LMesh* LMeshBuilder::createCylinder( GLfloat radius, GLfloat height, int sectionDivision )
    {
        LMesh* _mesh = NULL;

        vector< LVec3 > _vertices;
        vector< LVec3 > _normals;
        vector< LVec2 > _texCoords;
        vector< LInd3 > _indices;

        // Start cylinder tessellation

        // calculate section geometry
        vector< LVec3 > _sectionXZ;

        float _stepSectionAngle = 2 * _PI / sectionDivision;

        for ( int q = 0; q < sectionDivision; q++ )
        {
            float _x = radius * cos( q * _stepSectionAngle );
            float _z = radius * sin( q * _stepSectionAngle );
        #if AXIS_X == 1
            _sectionXZ.push_back( LVec3( 0, _z, _x ) );
        #elif AXIS_Y == 1
            _sectionXZ.push_back( LVec3( _x, 0, _z ) );
        #elif AXIS_Z == 1
            _sectionXZ.push_back( LVec3( _z, _x, 0 ) );
        #else
            _sectionXZ.push_back( LVec3( _x, 0, _z ) );
        #endif
        }

        // calculate cylinder geometry
        int _baseIndx = 0;
        // up base
        for ( int q = 0; q < _sectionXZ.size(); q++ )
        {
        #if AXIS_X == 1
            _vertices.push_back( _sectionXZ[q] + LVec3( 0.5 * height, 0, 0 ) );
            _normals.push_back( LVec3( 1, 0, 0 ) );
        #elif AXIS_Y == 1
            _vertices.push_back( _sectionXZ[q] + LVec3( 0, 0.5 * height, 0 ) );
            _normals.push_back( LVec3( 0, 1, 0 ) );
        #elif AXIS_Z == 1
            _vertices.push_back( _sectionXZ[q] + LVec3( 0, 0, 0.5 * height ) );
            _normals.push_back( LVec3( 0, 0, 1 ) );
        #else
            _vertices.push_back( _sectionXZ[q] + LVec3( 0, 0.5 * height, 0 ) );
            _normals.push_back( LVec3( 0, 1, 0 ) );
        #endif
            // @TODO: Check this part, there might be something wrong with the texture coordinates
            _texCoords.push_back( LVec2( 0.5 + ( _sectionXZ[q].z / ( 2 * radius ) ),
                                         0.5 + ( _sectionXZ[q].x / ( 2 * radius ) ) ) );
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
        #if AXIS_X == 1
            auto _p0 = _sectionXZ[q] + LVec3( 0.5 * height, 0, 0 );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0.5 * height, 0, 0 );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( -0.5 * height, 0, 0 );
            auto _p3 = _sectionXZ[q] + LVec3( -0.5 * height, 0, 0 );
        #elif AXIS_Y == 1
            auto _p0 = _sectionXZ[q] + LVec3( 0, 0.5 * height, 0 );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0.5 * height, 0 );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, -0.5 * height, 0 );
            auto _p3 = _sectionXZ[q] + LVec3( 0, -0.5 * height, 0 );
        #elif AXIS_Z == 1
            auto _p0 = _sectionXZ[q] + LVec3( 0, 0, 0.5 * height );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0, 0.5 * height );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0, -0.5 * height );
            auto _p3 = _sectionXZ[q] + LVec3( 0, 0, -0.5 * height );
        #else
            auto _p0 = _sectionXZ[q] + LVec3( 0, 0.5 * height, 0 );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0.5 * height, 0 );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, -0.5 * height, 0 );
            auto _p3 = _sectionXZ[q] + LVec3( 0, -0.5 * height, 0 );
        #endif

            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );
            _vertices.push_back( _p3 );

            // @TODO: Check this part, there might be something wrong with the texture coordinates
            _texCoords.push_back( LVec2( 0.5 + ( atan2( _p0.z, _p0.x ) / ( 2 * _PI ) ), 
                                  0.5 + _p0.y / height ) );
            _texCoords.push_back( LVec2( 0.5 + ( atan2( _p1.z, _p1.x ) / ( 2 * _PI ) ), 
                                  0.5 + _p1.y / height ) );
            _texCoords.push_back( LVec2( 0.5 + ( atan2( _p2.z, _p2.x ) / ( 2 * _PI ) ), 
                                  0.5 + _p2.y / height ) );
            _texCoords.push_back( LVec2( 0.5 + ( atan2( _p3.z, _p3.x ) / ( 2 * _PI ) ), 
                                  0.5 + _p3.y / height ) );

            // float _nx = cos( ( q + 0.5 ) * _stepSectionAngle );
            // float _nz = sin( ( q + 0.5 ) * _stepSectionAngle );

            // auto _nQuad = LVec3( _nx, 0, _nz );

            // _normals.push_back( _nQuad );
            // _normals.push_back( _nQuad );
            // _normals.push_back( _nQuad );
            // _normals.push_back( _nQuad );

            // For "smooth" normals
            float _nx1 = cos( ( q ) * _stepSectionAngle );
            float _nz1 = sin( ( q ) * _stepSectionAngle );

            float _nx2 = cos( ( q + 1 ) * _stepSectionAngle );
            float _nz2 = sin( ( q + 1 ) * _stepSectionAngle );

        #if AXIS_X == 1
            auto _nQuad1 = LVec3( 0, _nz1, _nx1 );
            auto _nQuad2 = LVec3( 0, _nz2, _nx2 );
        #elif AXIS_Y == 1
            auto _nQuad1 = LVec3( _nx1, 0, _nz1 );
            auto _nQuad2 = LVec3( _nx2, 0, _nz2 );
        #elif AXIS_Z == 1
            auto _nQuad1 = LVec3( _nz1, _nx1, 0 );
            auto _nQuad2 = LVec3( _nz2, _nx2, 0 );
        #else
            auto _nQuad1 = LVec3( _nx1, 0, _nz1 );
            auto _nQuad2 = LVec3( _nx2, 0, _nz2 );
        #endif
            _normals.push_back( _nQuad1 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad1 );

            _indices.push_back( LInd3( _baseIndx, _baseIndx + 2, _baseIndx + 1 ) );
            _indices.push_back( LInd3( _baseIndx, _baseIndx + 3, _baseIndx + 2 ) );

            _baseIndx += 4;
        }
        // down base
        for ( int q = 0; q < _sectionXZ.size(); q++ )
        {
        #if AXIS_X == 1
            _vertices.push_back( _sectionXZ[q] + LVec3( -0.5 * height, 0, 0 ) );
            _normals.push_back( LVec3( -1, 0, 0 ) );
        #elif AXIS_Y == 1
            _vertices.push_back( _sectionXZ[q] + LVec3( 0, -0.5 * height, 0 ) );
            _normals.push_back( LVec3( 0, -1, 0 ) );
        #elif AXIS_Z == 1
            _vertices.push_back( _sectionXZ[q] + LVec3( 0, 0, -0.5 * height ) );
            _normals.push_back( LVec3( 0, 0, -1 ) );
        #else
            _vertices.push_back( _sectionXZ[q] + LVec3( 0, -0.5 * height, 0 ) );
            _normals.push_back( LVec3( 0, -1, 0 ) );
        #endif
            // @TODO: Check this part, there might be something wrong with the texture coordinates
            _texCoords.push_back( LVec2( 0.5 + ( _sectionXZ[q].z / ( 2 * radius ) ),
                                         0.5 + ( _sectionXZ[q].x / ( 2 * radius ) ) ) );
        }
        _baseIndx += _sectionXZ.size();
        for ( int q = 1; q <= _sectionXZ.size() - 2; q++ )
        {
            _indices.push_back( LInd3( _baseIndx, _baseIndx + q + 1, _baseIndx + q ) );
        }

        _mesh = new LMesh( _vertices, _normals, _texCoords, _indices );

        return _mesh;
    }

    LMesh* LMeshBuilder::createCapsule( GLfloat radius, GLfloat height, int sectionDivision, int capLevels )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LVec2> _texCoords;
        vector<LInd3> _indices;

        /* Capsule format
        *
        *   |-- radius --|-- height --|-- radius --|
        */
        
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

            #if AXIS_X == 1
                auto _upOffset = LVec3( 0.5 * height, 0, 0 );
                _vertices.push_back( LVec3( radius * _y, radius * _z, radius * _x ) + _upOffset );
                _normals.push_back( LVec3( _y, _z, _x ) );
            #elif AXIS_Y == 1
                auto _upOffset = LVec3( 0, 0.5 * height, 0 );
                _vertices.push_back( LVec3( radius * _x, radius * _y, radius * _z ) + _upOffset );
                _normals.push_back( LVec3( _x, _y, _z ) );
            #elif AXIS_Z == 1
                auto _upOffset = LVec3( 0, 0, 0.5 * height );
                _vertices.push_back( LVec3( radius * _z, radius * _x, radius * _y ) + _upOffset );
                _normals.push_back( LVec3( _z, _x, _y ) );
            #else
                auto _upOffset = LVec3( 0, 0.5 * height, 0 );
                _vertices.push_back( LVec3( radius * _x, radius * _y, radius * _z ) + _upOffset );
                _normals.push_back( LVec3( _x, _y, _z ) );
            #endif
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
        #if AXIS_X == 1
            _sectionXZ.push_back( LVec3( 0, _z, _x ) );
        #elif AXIS_Y == 1
            _sectionXZ.push_back( LVec3( _x, 0, _z ) );
        #elif AXIS_Z == 1
            _sectionXZ.push_back( LVec3( _z, _x, 0 ) );
        #else
            _sectionXZ.push_back( LVec3( _x, 0, _z ) );
        #endif
        }

        // body surface
        
        for ( int q = 0; q < _sectionXZ.size(); q++ )
        {
            // quad vertices
        #if AXIS_X == 1
            auto _p0 = _sectionXZ[q] + LVec3( 0.5 * height, 0, 0 );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0.5 * height, 0, 0 );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( -0.5 * height, 0, 0 );
            auto _p3 = _sectionXZ[q] + LVec3( -0.5 * height, 0, 0 );
        #elif AXIS_Y == 1
            auto _p0 = _sectionXZ[q] + LVec3( 0, 0.5 * height, 0 );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0.5 * height, 0 );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, -0.5 * height, 0 );
            auto _p3 = _sectionXZ[q] + LVec3( 0, -0.5 * height, 0 );
        #elif AXIS_Z == 1
            auto _p0 = _sectionXZ[q] + LVec3( 0, 0, 0.5 * height );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0, 0.5 * height );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0, -0.5 * height );
            auto _p3 = _sectionXZ[q] + LVec3( 0, 0, -0.5 * height );
        #else
            auto _p0 = _sectionXZ[q] + LVec3( 0, 0.5 * height, 0 );
            auto _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0.5 * height, 0 );
            auto _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, -0.5 * height, 0 );
            auto _p3 = _sectionXZ[q] + LVec3( 0, -0.5 * height, 0 );
        #endif

            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );
            _vertices.push_back( _p3 );
        //     // For "flat" normals
        //     float _nx = cos( ( q + 0.5 ) * _stepSectionAngle );
        //     float _nz = sin( ( q + 0.5 ) * _stepSectionAngle );
        // #if AXIS_X == 1
        //     auto _nQuad = LVec3( 0, _nz, _nx );
        // #elif AXIS_Y == 1
        //     auto _nQuad = LVec3( _nx, 0, _nz );
        // #elif AXIS_Z == 1
        //     auto _nQuad = LVec3( _nz, _nx, 0 );
        // #else
        //     auto _nQuad = LVec3( _nx, 0, _nz );
        // #endif
        //     _normals.push_back( _nQuad );
        //     _normals.push_back( _nQuad );
        //     _normals.push_back( _nQuad );
        //     _normals.push_back( _nQuad );

            // For "smooth" normals
            float _nx1 = cos( ( q ) * _stepSectionAngle );
            float _nz1 = sin( ( q ) * _stepSectionAngle );

            float _nx2 = cos( ( q + 1 ) * _stepSectionAngle );
            float _nz2 = sin( ( q + 1 ) * _stepSectionAngle );

        #if AXIS_X == 1
            auto _nQuad1 = LVec3( 0, _nz1, _nx1 );
            auto _nQuad2 = LVec3( 0, _nz2, _nx2 );
        #elif AXIS_Y == 1
            auto _nQuad1 = LVec3( _nx1, 0, _nz1 );
            auto _nQuad2 = LVec3( _nx2, 0, _nz2 );
        #elif AXIS_Z == 1
            auto _nQuad1 = LVec3( _nz1, _nx1, 0 );
            auto _nQuad2 = LVec3( _nz2, _nx2, 0 );
        #else
            auto _nQuad1 = LVec3( _nx1, 0, _nz1 );
            auto _nQuad2 = LVec3( _nx2, 0, _nz2 );
        #endif
            _normals.push_back( _nQuad1 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad1 );

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
            #if AXIS_X == 1
                auto _downOffset = LVec3( -0.5 * height, 0, 0 );
                _vertices.push_back( LVec3( radius * _y, radius * _z, radius * _x ) + _downOffset );
                _normals.push_back( LVec3( _y, _z, _x ) );
            #elif AXIS_Y == 1
                auto _downOffset = LVec3( 0, -0.5 * height, 0 );
                _vertices.push_back( LVec3( radius * _x, radius * _y, radius * _z ) + _downOffset );
                _normals.push_back( LVec3( _x, _y, _z ) );
            #elif AXIS_Z == 1
                auto _downOffset = LVec3( 0, 0, -0.5 * height );
                _vertices.push_back( LVec3( radius * _z, radius * _x, radius * _y ) + _downOffset );
                _normals.push_back( LVec3( _z, _x, _y ) );
            #else
                auto _downOffset = LVec3( 0, -0.5 * height, 0 );
                _vertices.push_back( LVec3( radius * _x, radius * _y, radius * _z ) + _downOffset );
                _normals.push_back( LVec3( _x, _y, _z ) );
            #endif
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

        for ( int q = 0; q < _vertices.size(); q++ )
        {
            //// Project capsule onto sphere of radius ( h + 2r )
            // calculate spherical coordinates
            float _rC = sqrt( _vertices[q].x * _vertices[q].x + 
                              _vertices[q].y * _vertices[q].y +
                              _vertices[q].z * _vertices[q].z );
            float _thetaC = acos( _vertices[q].z / _rC );
            float _yawC = atan2( _vertices[q].y, _vertices[q].x );// [-pi, pi]
            // convert to coordinates in projection sphere
            float _rS = height + 2 * radius;
            float _thetaS = _thetaC;
            float _yawS = _yawC;
            // Transform to coordinates in sphere
            float _xS = sin( _thetaS ) * cos( _yawS );
            float _yS = sin( _thetaS ) * sin( _yawS );
            float _zS = cos( _thetaS );
            // Extract UVs from spherical uv wrapping
            float _u = 0.5 + ( atan2( -_zS, -_xS ) / ( 2 * _PI ) );
            float _v = 0.5 - ( asin( -_yS ) / _PI );

            _texCoords.push_back( LVec2( _u, _v ) );
        }

        _mesh = new LMesh( _vertices, _normals, _texCoords, _indices );

        return _mesh;
    }

    LMesh* LMeshBuilder::createPlane( GLfloat width, GLfloat depth, float texRangeWidth, float texRangeDepth )
    {
        LMesh* _mesh = NULL;

        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LVec2> _texCoords;
        vector<LInd3> _indices;

    #if AXIS_X == 1
        LVec3 _n = LVec3( 1.0f, 0.0f, 0.0f );
        LVec3 _s1 = LVec3( 0.0f, 1.0f, 0.0f );
        LVec3 _s2 = LVec3( 0.0f, 0.0f, 1.0f );
        LVec3 _scale = LVec3( 0.0f, 0.5 * depth, 0.5 * width );
    #elif AXIS_Y == 1
        LVec3 _n = LVec3( 0.0f, 1.0f, 0.0f );
        LVec3 _s1 = LVec3( 0.0f, 0.0f, 1.0f );
        LVec3 _s2 = LVec3( 1.0f, 0.0f, 0.0f );
        LVec3 _scale = LVec3( 0.5 * width, 0.0f, 0.5 * depth );
    #elif AXIS_Z == 1
        LVec3 _n = LVec3( 0.0f, 0.0f, 1.0f );
        LVec3 _s1 = LVec3( 1.0f, 0.0f, 0.0f );
        LVec3 _s2 = LVec3( 0.0f, 1.0f, 0.0f );
        LVec3 _scale = LVec3( 0.5 * depth, 0.5 * width, 0.0f );
    #else
        LVec3 _n = LVec3( 0.0f, 1.0f, 0.0f );
        LVec3 _s1 = LVec3( 0.0f, 0.0f, 1.0f );
        LVec3 _s2 = LVec3( 1.0f, 0.0f, 0.0f );
        LVec3 _scale = LVec3( 0.5 * width, 0.0f, 0.5 * depth );
    #endif

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
        _texCoords.push_back( LVec2( 0, 0 ) );

        _v = _n - _s1 + _s2;
        _v.scale( _scale.x, _scale.y, _scale.z );

        _vertices.push_back( _v );
        _normals.push_back( _n );
        _texCoords.push_back( LVec2( texRangeWidth, 0 ) );

        _v = _n + _s1 + _s2;
        _v.scale( _scale.x, _scale.y, _scale.z );

        _vertices.push_back( _v );
        _normals.push_back( _n );
        _texCoords.push_back( LVec2( texRangeWidth, texRangeDepth ) );

        _v = _n + _s1 - _s2;
        _v.scale( _scale.x, _scale.y, _scale.z );

        _vertices.push_back( _v );
        _normals.push_back( _n );
        _texCoords.push_back( LVec2( 0, texRangeDepth ) );

        _mesh = new LMesh( _vertices, _normals, _texCoords, _indices );

        return _mesh;
    }

    LMesh* LMeshBuilder::createArrow( GLfloat length, const std::string& axis )
    {
        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LVec2> _texCoords;
        vector<LInd3> _indices;

        int _sectionDivision = 10;

        // Arrow dimensions
        const float _radiusCyl    = 0.05 * length;
        const float _radiusCone   = 0.075 * length;
        const float _lengthCyl    = 0.8 * length;
        const float _lengthCone   = 0.2 * length;

        // Tesselate cylinder ***********************************************************************
        // calculate section geometry
        vector<LVec3> _sectionXZ;

        float _stepSectionAngle = 2 * _PI / _sectionDivision;

        for ( size_t q = 0; q < _sectionDivision; q++ )
        {
            float _x = _radiusCyl * std::cos( q * _stepSectionAngle );
            float _z = _radiusCyl * std::sin( q * _stepSectionAngle );

            if ( axis == "x" )
                _sectionXZ.push_back( LVec3( 0, _z, _x ) );
            else if ( axis == "y" )
                _sectionXZ.push_back( LVec3( _x, 0, _z ) );
            else if ( axis == "z" )
                _sectionXZ.push_back( LVec3( _z, _x, 0 ) );
            else
                _sectionXZ.push_back( LVec3( _x, 0, _z ) );
        }

        // calculate cylinder geometry
        int _baseIndx = 0;

        // down base ****************************************
        for ( size_t q = 0; q < _sectionXZ.size(); q++ )
        {
            _vertices.push_back( _sectionXZ[q] );

            if ( axis == "x" )
                _normals.push_back( LVec3( -1, 0, 0 ) );
            else if ( axis == "y" )
                _normals.push_back( LVec3( 0, -1, 0 ) );
            else if ( axis == "z" )
                _normals.push_back( LVec3( 0, 0, -1 ) );
            else
                _normals.push_back( LVec3( 0, -1, 0 ) );

            if ( axis == "x" )
            {
                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZ[q].y / ( 2 * _radiusCyl ) ),
                                             0.5 + ( _sectionXZ[q].z / ( 2 * _radiusCyl ) ) ) );
            }
            else if ( axis == "y" )
            {
                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZ[q].z / ( 2 * _radiusCyl ) ),
                                             0.5 + ( _sectionXZ[q].x / ( 2 * _radiusCyl ) ) ) );
            }
            else if ( axis == "z" )
            {
                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZ[q].x / ( 2 * _radiusCyl ) ),
                                             0.5 + ( _sectionXZ[q].y / ( 2 * _radiusCyl ) ) ) );
            }
            else
            {
                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZ[q].z / ( 2 * _radiusCyl ) ),
                                             0.5 + ( _sectionXZ[q].x / ( 2 * _radiusCyl ) ) ) );
            }
        }
        for ( size_t q = 1; q <= _sectionXZ.size() - 2; q++ )
        {
            _indices.push_back( LInd3( _baseIndx, _baseIndx + q, _baseIndx + q + 1 ) );
        }
        _baseIndx += _vertices.size();
        // **************************************************

        // body surface *************************************
        for ( size_t q = 0; q < _sectionXZ.size(); q++ )
        {
            // quad vertices
            LVec3 _p0, _p1, _p2, _p3;

            if ( axis == "x" )
            {
                _p0 = _sectionXZ[q] +  LVec3( _lengthCyl, 0, 0 );
                _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( _lengthCyl, 0, 0 );
                _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0, 0 );
                _p3 = _sectionXZ[q] + LVec3( 0, 0, 0 );
            }
            else if ( axis == "y" )
            {
                _p0 = _sectionXZ[q] +  LVec3( 0, _lengthCyl, 0 );
                _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, _lengthCyl, 0 );
                _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0, 0 );
                _p3 = _sectionXZ[q] + LVec3( 0, 0, 0 );
            }
            else if ( axis == "z" )
            {
                _p0 = _sectionXZ[q] +  LVec3( 0, 0, _lengthCyl );
                _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0, _lengthCyl );
                _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0, 0 );
                _p3 = _sectionXZ[q] + LVec3( 0, 0, 0 );
            }
            else
            {
                _p0 = _sectionXZ[q] +  LVec3( 0, _lengthCyl, 0 );
                _p1 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, _lengthCyl, 0 );
                _p2 = _sectionXZ[( q + 1 ) % _sectionXZ.size()] + LVec3( 0, 0, 0 );
                _p3 = _sectionXZ[q] + LVec3( 0, 0, 0 );
            }

            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );
            _vertices.push_back( _p3 );

            if ( axis == "x" )
            {
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p0.y, _p0.z ) / ( 2.0 * _PI ) ), 
                                             _p0.x / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p1.y, _p1.z ) / ( 2.0 * _PI ) ), 
                                             _p1.x / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p2.y, _p2.z ) / ( 2.0 * _PI ) ), 
                                             _p2.x / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p3.y, _p3.z ) / ( 2.0 * _PI ) ), 
                                             _p3.x / _lengthCyl ) );
            }
            else if ( axis == "y" )
            {
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p0.z, _p0.x ) / ( 2.0 * _PI ) ), 
                                             _p0.y / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p1.z, _p1.x ) / ( 2.0 * _PI ) ), 
                                             _p1.y / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p2.z, _p2.x ) / ( 2.0 * _PI ) ), 
                                             _p2.y / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p3.z, _p3.x ) / ( 2.0 * _PI ) ), 
                                             _p3.y / _lengthCyl ) );
            }
            else if ( axis == "z" )
            {
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p0.x, _p0.y ) / ( 2.0 * _PI ) ), 
                                             _p0.z / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p1.x, _p1.y ) / ( 2.0 * _PI ) ), 
                                             _p1.z / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p2.x, _p2.y ) / ( 2.0 * _PI ) ), 
                                             _p2.z / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p3.x, _p3.y ) / ( 2.0 * _PI ) ), 
                                             _p3.z / _lengthCyl ) );
            }
            else
            {
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p0.z, _p0.x ) / ( 2.0 * _PI ) ), 
                                             _p0.y / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p1.z, _p1.x ) / ( 2.0 * _PI ) ), 
                                             _p1.y / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p2.z, _p2.x ) / ( 2.0 * _PI ) ), 
                                             _p2.y / _lengthCyl ) );
                _texCoords.push_back( LVec2( 0.5 + ( std::atan2( _p3.z, _p3.x ) / ( 2.0 * _PI ) ), 
                                             _p3.y / _lengthCyl ) );
            }

            // For "smooth" normals
            float _nz1 = std::sin( ( q ) * _stepSectionAngle );
            float _nx1 = std::cos( ( q ) * _stepSectionAngle );

            float _nx2 = std::cos( ( q + 1 ) * _stepSectionAngle );
            float _nz2 = std::sin( ( q + 1 ) * _stepSectionAngle );

            LVec3 _nQuad1, _nQuad2;

            if ( axis == "x" )
            {
                _nQuad1 = LVec3( 0, _nz1, _nx1 );
                _nQuad2 = LVec3( 0, _nz2, _nx2 );
            }
            else if ( axis == "y" )
            {
                _nQuad1 = LVec3( _nx1, 0, _nz1 );
                _nQuad2 = LVec3( _nx2, 0, _nz2 );
            }
            else if ( axis == "z" )
            {
                _nQuad1 = LVec3( _nz1, _nx1, 0 );
                _nQuad2 = LVec3( _nz2, _nx2, 0 );
            }
            else
            {
                _nQuad1 = LVec3( _nx1, 0, _nz1 );
                _nQuad2 = LVec3( _nx2, 0, _nz2 );
            }

            _normals.push_back( _nQuad1 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad1 );

            _indices.push_back( LInd3( _baseIndx, _baseIndx + 2, _baseIndx + 1 ) );
            _indices.push_back( LInd3( _baseIndx, _baseIndx + 3, _baseIndx + 2 ) );

            _baseIndx += 4;
        }
        // **************************************************

        // Tesselate cone ***************************************************************************

        // Build base points
        vector<LVec3> _sectionXZCone;
        vector<LVec3> _sectionXZConeIn;

        float _stepSectionAngleCone = 2.0 * _PI / _sectionDivision;

        for ( size_t q = 0; q < _sectionDivision; q++ )
        {
            float _x = _radiusCone * std::cos( q * _stepSectionAngleCone );
            float _z = _radiusCone * std::sin( q * _stepSectionAngleCone );

            if ( axis == "x" )
                _sectionXZCone.push_back( LVec3( 0, _z, _x ) );
            else if ( axis == "y" )
                _sectionXZCone.push_back( LVec3( _x, 0, _z ) );
            else if ( axis == "z" )
                _sectionXZCone.push_back( LVec3( _z, _x, 0 ) );
            else
                _sectionXZCone.push_back( LVec3( _x, 0, _z ) );

            _x = _radiusCyl * std::cos( q * _stepSectionAngleCone );
            _z = _radiusCyl * std::sin( q * _stepSectionAngleCone );

            if ( axis == "x" )
                _sectionXZConeIn.push_back( LVec3( 0, _z, _x ) );
            else if ( axis == "y" )
                _sectionXZConeIn.push_back( LVec3( _x, 0, _z ) );
            else if ( axis == "z" )
                _sectionXZConeIn.push_back( LVec3( _z, _x, 0 ) );
            else
                _sectionXZConeIn.push_back( LVec3( _x, 0, _z ) );
        }

        // Build surface - tesselate using strips of triangles
        for ( size_t q = 0; q < _sectionXZCone.size(); q++ )
        {
            _indices.push_back( LInd3( _vertices.size(),
                                       _vertices.size() + 1,
                                       _vertices.size() + 2 ) );

            LVec3 _p0, _p1, _p2;

            if ( axis == "x" )
            {
                _p0 = _sectionXZCone[ q % _sectionXZCone.size() ] + LVec3( _lengthCyl, 0, 0 );
                _p1 = _sectionXZCone[ ( q + 1 ) % _sectionXZCone.size() ] + LVec3( _lengthCyl, 0, 0 );
                _p2 = LVec3( _lengthCyl + _lengthCone, 0, 0 );
            }
            else if ( axis == "y" )
            {
                _p0 = _sectionXZCone[ q % _sectionXZCone.size() ] + LVec3( 0, _lengthCyl, 0 );
                _p1 = _sectionXZCone[ ( q + 1 ) % _sectionXZCone.size() ] + LVec3( 0, _lengthCyl, 0 );
                _p2 = LVec3( 0, _lengthCyl + _lengthCone, 0 );
            }
            else if ( axis == "z" )
            {
                _p0 = _sectionXZCone[ q % _sectionXZCone.size() ] + LVec3( 0, 0, _lengthCyl );
                _p1 = _sectionXZCone[ ( q + 1 ) % _sectionXZCone.size() ] + LVec3( 0, 0, _lengthCyl );
                _p2 = LVec3( 0, 0, _lengthCyl + _lengthCone );
            }
            else
            {
                _p0 = _sectionXZCone[ q % _sectionXZCone.size() ] + LVec3( 0, _lengthCyl, 0 );
                _p1 = _sectionXZCone[ ( q + 1 ) % _sectionXZCone.size() ] + LVec3( 0, _lengthCyl, 0 );
                _p2 = LVec3( 0, _lengthCyl + _lengthCone, 0 );
            }

            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );

            float _nx0 = std::cos( ( q ) * _stepSectionAngleCone );
            float _nz0 = std::sin( ( q ) * _stepSectionAngleCone );

            float _nx1 = std::cos( ( q + 1 ) * _stepSectionAngleCone );
            float _nz1 = std::sin( ( q + 1 ) * _stepSectionAngleCone );

            LVec3 _n0, _n1, _n2;

            if ( axis == "x" )
            {
                _n0 = LVec3( 0, _nz0, _nx0 );
                _n1 = LVec3( 0, _nz1, _nx1 );
                _n2 = LVec3( 1, 0, 0 );
            }
            else if ( axis == "y" )
            {
                _n0 = LVec3( _nx0, 0, _nz0 );
                _n1 = LVec3( _nx1, 0, _nz1 );
                _n2 = LVec3( 0, 1, 0 );
            }
            else if ( axis == "z" )
            {
                _n0 = LVec3( _nz0, _nx0, 0 );
                _n1 = LVec3( _nz1, _nx1, 0 );
                _n2 = LVec3( 0, 0, 1 );
            }

            _normals.push_back( _n0 );
            _normals.push_back( _n1 );
            _normals.push_back( _n2 );

            _texCoords.push_back( LVec2( q / _sectionXZCone.size(), 1 ) );
            _texCoords.push_back( LVec2( ( q + 1 ) / _sectionXZCone.size(), 1 ) );
            _texCoords.push_back( LVec2( ( q + 0.5 ) / _sectionXZCone.size(), 0 ) );
        }

        // Build bottom base - strip of "kind of quads" ( ring tessellation )
        _baseIndx = _vertices.size();

        for ( size_t q = 0; q < _sectionXZCone.size(); q++ )
        {
            LVec3 _p0, _p1, _p2, _p3;

            if ( axis == "x" )
            {
                _p0 = _sectionXZCone[q] + LVec3( _lengthCyl, 0, 0 );
                _p1 = _sectionXZConeIn[q] + LVec3( _lengthCyl, 0, 0 );
                _p2 = _sectionXZConeIn[( q + 1 ) % _sectionXZConeIn.size()] + LVec3( _lengthCyl, 0, 0 );
                _p3 = _sectionXZ[( q + 1 ) % _sectionXZCone.size()] + LVec3( _lengthCyl, 0, 0 );
            }
            else if ( axis == "y" )
            {
                _p0 = _sectionXZCone[q] + LVec3( 0, _lengthCyl, 0 );
                _p1 = _sectionXZConeIn[q] + LVec3( 0, _lengthCyl, 0 );
                _p2 = _sectionXZConeIn[( q + 1 ) % _sectionXZConeIn.size()] + LVec3( 0, _lengthCyl, 0 );
                _p3 = _sectionXZ[( q + 1 ) % _sectionXZCone.size()] + LVec3( 0, _lengthCyl, 0 );
            }
            else if ( axis == "z" )
            {
                _p0 = _sectionXZCone[q] + LVec3( 0, 0, _lengthCyl );
                _p1 = _sectionXZConeIn[q] + LVec3( 0, 0, _lengthCyl );
                _p2 = _sectionXZConeIn[( q + 1 ) % _sectionXZConeIn.size()] + LVec3( 0, 0, _lengthCyl );
                _p3 = _sectionXZ[( q + 1 ) % _sectionXZCone.size()] + LVec3( 0, 0, _lengthCyl );
            }
            else
            {
                _p0 = _sectionXZCone[q] + LVec3( 0, _lengthCyl, 0 );
                _p1 = _sectionXZConeIn[q] + LVec3( 0, _lengthCyl, 0 );
                _p2 = _sectionXZConeIn[( q + 1 ) % _sectionXZConeIn.size()] + LVec3( 0, _lengthCyl, 0 );
                _p3 = _sectionXZ[( q + 1 ) % _sectionXZCone.size()] + LVec3( 0, _lengthCyl, 0 );
            }


            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );
            _vertices.push_back( _p3 );

            LVec3 _normal;
            if ( axis == "x" )
                _normal = LVec3( -1, 0, 0 );
            else if ( axis == "y" )
                _normal = LVec3( 0, -1, 0 );
            else if ( axis == "z" )
                _normal = LVec3( 0, 0, -1 );
            else
                _normal = LVec3( 0, -1, 0 );

            _normals.push_back( _normal );
            _normals.push_back( _normal );
            _normals.push_back( _normal );
            _normals.push_back( _normal );

            if ( axis == "x" )
            {
                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZCone[q].y / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[q].z / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZConeIn[q].y / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZConeIn[q].z / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZConeIn[( q + 1 ) % _sectionXZConeIn.size()].y / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZConeIn.size()].z / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZCone.size()].y / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZCone.size()].z / ( 2 * _radiusCone ) ) ) );
            }
            else if ( axis == "y" )
            {
                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZCone[q].z / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[q].x / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZConeIn[q].z / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZConeIn[q].x / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZConeIn[( q + 1 ) % _sectionXZConeIn.size()].z / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZConeIn.size()].x / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZCone.size()].z / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZCone.size()].x / ( 2 * _radiusCone ) ) ) );
            }
            else if ( axis == "z" )
            {
                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZCone[q].x / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[q].y / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZConeIn[q].x / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZConeIn[q].y / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZConeIn[( q + 1 ) % _sectionXZConeIn.size()].x / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZConeIn.size()].y / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZCone.size()].x / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZCone.size()].y / ( 2 * _radiusCone ) ) ) );
            }
            else
            {
                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZCone[q].z / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[q].x / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZConeIn[q].z / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZConeIn[q].x / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZConeIn[( q + 1 ) % _sectionXZConeIn.size()].z / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZConeIn.size()].x / ( 2 * _radiusCone ) ) ) );

                _texCoords.push_back( LVec2( 0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZCone.size()].z / ( 2 * _radiusCone ) ),
                                             0.5 + ( _sectionXZCone[( q + 1 ) % _sectionXZCone.size()].x / ( 2 * _radiusCone ) ) ) );
            }

            _indices.push_back( LInd3( _baseIndx, _baseIndx + 2, _baseIndx + 1 ) );
            _indices.push_back( LInd3( _baseIndx, _baseIndx + 3, _baseIndx + 2 ) );

            _baseIndx += 4;
        }

        return new LMesh( _vertices, _normals,_texCoords, _indices );
    }

    LModel* LMeshBuilder::createAxes( GLfloat length )
    {
        auto _axesModel = new LModel( "" );

        auto _axisX         = LMeshBuilder::createArrow( length, "x" );
        auto _axisY         = LMeshBuilder::createArrow( length, "y" );
        auto _axisZ         = LMeshBuilder::createArrow( length, "z" );
        auto _axisCenter    = LMeshBuilder::createSphere( 0.2 * length );

        _axisX->getMaterial()->ambient  = { 1.0, 0.0, 0.0 };
        _axisX->getMaterial()->diffuse  = { 1.0, 0.0, 0.0 };
        _axisX->getMaterial()->specular = { 1.0, 0.0, 0.0 };

        _axisY->getMaterial()->ambient  = { 0.0, 1.0, 0.0 };
        _axisY->getMaterial()->diffuse  = { 0.0, 1.0, 0.0 };
        _axisY->getMaterial()->specular = { 0.0, 1.0, 0.0 };

        _axisZ->getMaterial()->ambient  = { 0.0, 0.0, 1.0 };
        _axisZ->getMaterial()->diffuse  = { 0.0, 0.0, 1.0 };
        _axisZ->getMaterial()->specular = { 0.0, 0.0, 1.0 };

        _axisCenter->getMaterial()->ambient  = { 0.3, 0.3, 0.3 };
        _axisCenter->getMaterial()->diffuse  = { 0.3, 0.3, 0.3 };
        _axisCenter->getMaterial()->specular = { 0.3, 0.3, 0.3 };

        _axesModel->addMesh( _axisX );
        _axesModel->addMesh( _axisY );
        _axesModel->addMesh( _axisZ );
        _axesModel->addMesh( _axisCenter );

        return _axesModel;
    }

    LModel* LMeshBuilder::createModelFromFile( const std::string& filename,
                                               const std::string& modelName )
    {
        auto _assimpScenePtr = aiImportFile( filename.c_str(),
                                             aiProcessPreset_TargetRealtime_MaxQuality );

        if ( !_assimpScenePtr )
        {
            return NULL;
        }

        // Create a temporary holder to place the processes data from assimp
        auto _model = new LModel( modelName );
        // recursively copy the data from assimp to our data structure
        _processAssimpNode( _model, _assimpScenePtr->mRootNode, _assimpScenePtr );

        // make sure we release the assimp resources
        // @TODO: Should do this in a assetsModelManager (to avoid repetitions)
        aiReleaseImport( _assimpScenePtr );

        return _model;
    }


    void LMeshBuilder::_processAssimpNode( LModel* modelPtr, 
                                           aiNode* assimpNodePtr, 
                                           const aiScene* assimpScenePtr )
    {
        for ( size_t i = 0; i < assimpNodePtr->mNumMeshes; i++ )
        {
            aiMesh* _assimpMeshPtr = assimpScenePtr->mMeshes[ assimpNodePtr->mMeshes[i] ];
            modelPtr->addMesh( _processAssimpMesh( _assimpMeshPtr ) );
        }

        for ( size_t i = 0; i < assimpNodePtr->mNumChildren; i++ )
        {
            _processAssimpNode( modelPtr,
                                assimpNodePtr->mChildren[i],
                                assimpScenePtr );
        }
    }

    LMesh* LMeshBuilder::_processAssimpMesh( aiMesh* assimpMeshPtr )
    {
        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LVec2> _texCoords;
        vector<LInd3> _indices;

        for ( size_t i = 0; i < assimpMeshPtr->mNumVertices; i++ )
        {
            _vertices.push_back( LVec3( assimpMeshPtr->mVertices[i].x,
                                        assimpMeshPtr->mVertices[i].y,
                                        assimpMeshPtr->mVertices[i].z ) );

            _normals.push_back( LVec3( assimpMeshPtr->mNormals[i].x,
                                       assimpMeshPtr->mNormals[i].y,
                                       assimpMeshPtr->mNormals[i].z ) );

            if ( assimpMeshPtr->mTextureCoords[0] )
            {
                _texCoords.push_back( LVec2( assimpMeshPtr->mTextureCoords[0][i].x,
                                             assimpMeshPtr->mTextureCoords[0][i].y ) );
            }
            else
            {
                _texCoords.push_back( LVec2( 0.0f, 0.0f ) );
            }
        }

        for ( size_t i = 0; i < assimpMeshPtr->mNumFaces; i++ )
        {
            aiFace _assimpFace = assimpMeshPtr->mFaces[i];
            // grab only in tris. we are assuming it comes this way
            // @TODO: Check this part as may have to support quads
            for ( size_t j = 0; j < _assimpFace.mNumIndices / 3; j++ )
            {
                _indices.push_back( LInd3( _assimpFace.mIndices[ 3 * j + 0 ],
                                           _assimpFace.mIndices[ 3 * j + 1 ],
                                           _assimpFace.mIndices[ 3 * j + 2 ] ) );
            }
        }

        return new LMesh( _vertices, _normals, _texCoords, _indices );
    }

    // LMesh* LMeshBuilder::createFromFile( const char* filename )
    // {
    //     LMesh* _mesh = NULL;

    //     vector<LVec3> _vertices;
    //     vector<LVec3> _normals;
    //     vector<LInd3> _indices;

    //     ifstream _fileHandle( filename );

    //     if ( !_fileHandle.is_open() )
    //     {
    //         cout << "LMeshBuilder::createFromFile> couldn't open the file " << filename << endl;
    //         return NULL;
    //     }

    //     string _line;

    //     getline( _fileHandle, _line );

    //     int _numFaces = stoi( _line );

    //     for ( int q = 0; q < _numFaces; q++ )
    //     {
    //         getline( _fileHandle, _line );

    //         int _numVertices = stoi( _line );

    //         for ( int p = 0; p < _numVertices; p++ )
    //         {
    //             getline( _fileHandle, _line );
    //             vector<string> _vStr = LMeshBuilder::_split( _line, ' ' );

    //             GLfloat _vx = stof( _vStr[0] );
    //             GLfloat _vy = stof( _vStr[1] );
    //             GLfloat _vz = stof( _vStr[2] );

    //             _vertices.push_back( LVec3( _vx, _vy, _vz ) );
    //         }

    //         int _i1 = _indices.size() * 3;
    //         int _i2 = _indices.size() * 3 + 1;
    //         int _i3 = _indices.size() * 3 + 2;

    //         _indices.push_back( LInd3( _i1, _i2, _i3 ) );

    //         LVec3 _n = _computeFaceNormal( _vertices[_i1], 
    //                                        _vertices[_i2], 
    //                                        _vertices[_i3] );
    //         _normals.push_back( _n );
    //         _normals.push_back( _n );
    //         _normals.push_back( _n );
    //     }

    //     _mesh = new LMesh( _vertices, _normals, _indices );
        
    //     return _mesh;
    // }


    // LMesh* LMeshBuilder::createFromObj( const char* filename )
    // {
    //     LMesh* _mesh = NULL;

    //     vector<LVec3> _vertices;
    //     vector<LVec3> _normals;
    //     vector<LVec2> _texCoord;
    //     vector<LInd3> _indices;

    //     ifstream _fileHandle( filename );

    //     if ( !_fileHandle.is_open() )
    //     {
    //         cout << "LMeshBuilder::createFromObj> couldn't open the file " << filename << endl;
    //         return NULL;
    //     }

    //     LObjInfo _objInfo;

    //     _parseObj( _fileHandle, _objInfo );

    //     _mesh = new LMesh( _objInfo.vertices, _objInfo.normals, _objInfo.texCoords );
        
    //     return _mesh;
    // }

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

    // void LMeshBuilder::_parseObj( ifstream& fileHandle,
    //                               LObjInfo& obj )
    // {
    //     cout << "parsing geometry" << endl;

    //     map<string, LGeometryInfo> _objects;

    //     string _currentObjectName;

    //     string _materialsFile;
    //     string _line;

    //     while ( getline( fileHandle, _line ) )
    //     {
    //         vector<string> _vStr = _split( _line, ' ' );
    //         // cout << "??? " << _vStr[0] << endl;

    //         if ( _vStr[0] == OBJ_COMMENT )
    //         {
    //             continue;
    //         }
    //         else if ( _vStr[0] == OBJ_MATERIAL_LIB )
    //         {
    //             obj.materialFile = _vStr[1];
    //         }
    //         else if ( _vStr[0] == OBJ_OBJECT_NAME )
    //         {
    //             _currentObjectName = _vStr[1];
    //             _objects[_currentObjectName] = LGeometryInfo();
    //         }
    //         else if ( _vStr[0] == OBJ_GROUP_NAME )
    //         {
    //             // cout << "obj model loader doesn't support group names yet" << endl;
    //             // assert( false );
    //             continue;
    //         }
    //         else if ( _vStr[0] == OBJ_VERTEX )
    //         {
    //             if ( _currentObjectName == "" )
    //             {
    //                 _currentObjectName = "default";
    //                 _objects[_currentObjectName] = LGeometryInfo();
    //             }

    //             LVec3 _v( stof( _vStr[1] ), stof( _vStr[2] ), stof( _vStr[3] ) );

    //             _objects[_currentObjectName].vertices.push_back( _v );
    //         }
    //         else if ( _vStr[0] == OBJ_NORMAL )
    //         {
    //             LVec3 _n( stof( _vStr[1] ), stof( _vStr[2] ), stof( _vStr[3] ) );

    //             _objects[_currentObjectName].normals.push_back( _n );
    //         }
    //         else if ( _vStr[0] == OBJ_TEXTURE )
    //         {
    //             LVec2 _t( stof( _vStr[1] ), stof( _vStr[2] ) );

    //             _objects[_currentObjectName].texCoords.push_back( _t );
    //         }
    //         else if ( _vStr[0] == OBJ_MATERIAL_ID )
    //         {
    //             continue;// skip, no material id support yet
    //         }
    //         else if ( _vStr[0] == OBJ_SMOOTH_SHADING )
    //         {
    //             continue;
    //         }
    //         else if ( _vStr[0] == OBJ_FACE )
    //         {
    //             for ( int q = 1; q < 4; q++ )
    //             {
    //                 vector<string> _vVertex = _split( _vStr[q], '/' );

    //                 obj.vertices.push_back( _objects[_currentObjectName].vertices[ stoi( _vVertex[0] ) - 1 ] );
    //                 obj.texCoords.push_back( _objects[_currentObjectName].texCoords[ stoi( _vVertex[1] ) - 1 ] );
    //                 obj.normals.push_back( _objects[_currentObjectName].normals[ stoi( _vVertex[2] ) - 1 ] );
    //             }
    //         }
    //         else
    //         {
    //             // cout << "not supported: " << _vStr[0] << endl;
    //             continue;
    //         }
    //     }

    //     cout << "vl: " << obj.vertices.size() << endl;
    //     cout << "nl: " << obj.normals.size() << endl;
    //     cout << "tl: " << obj.texCoords.size() << endl;

    //     cout << "done geometry" << endl;
    // }

    LMesh* LMeshBuilder::createPerlinPatch( GLfloat width, GLfloat depth, int cellDivision )
    {
        vector<LVec3> _vertices;
        vector<LVec3> _normals;
        vector<LInd3> _indices;
        vector<LVec2> _texCoord;

        #if AXIS_X == 1
            std::string axis = "x";
        #elif AXIS_Y == 1
            std::string axis = "y";
        #elif AXIS_Z == 1
            std::string axis = "z";
        #else
            std::string axis = "y";
        #endif

        LHeightmapGenerator _hmapGenerator;

        float _dw = width / cellDivision;
        float _dd = depth / cellDivision;

        float _x0 = -( cellDivision / 2.0f ) * _dw;
        float _z0 = -( cellDivision / 2.0f ) * _dd;

        // cout << "x0: " << _x0 << endl;
        // cout << "z0: " << _z0 << endl;
        // cout << "dw: " << _dw << endl;
        // cout << "dd: " << _dd << endl;

        for ( int i = 0; i < cellDivision; i++ )
        {
            for ( int j = 0; j < cellDivision; j++ )
            {
                auto _p0 = _createVec3WithAxis( _x0 + j * _dw, 
                                                2.0f + 2.0f * _hmapGenerator.getHeight( _x0 + j * _dw, _z0 + i * _dd ),
                                                _z0 + i * _dd,
                                                axis );

                auto _p1 = _createVec3WithAxis( _x0 + j * _dw,
                                                2.0f + 2.0f * _hmapGenerator.getHeight( _x0 + j * _dw, _z0 + ( i + 1 ) * _dd ),
                                                _z0 + ( i + 1 ) * _dd,
                                                axis );

                auto _p2 = _createVec3WithAxis( _x0 + ( j + 1 ) * _dw,
                                                2.0f + 2.0f * _hmapGenerator.getHeight( _x0 + ( j + 1 ) * _dw, _z0 + ( i + 1 ) * _dd ),
                                                _z0 + ( i + 1 ) * _dd,
                                                axis );

                auto _p3 = _createVec3WithAxis( _x0 + ( j + 1 ) * _dw,
                                                2.0f + 2.0f * _hmapGenerator.getHeight( _x0 + ( j + 1 ) * _dw, _z0 + i * _dd ),
                                                _z0 + i * _dd,
                                                axis );

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
            auto _p1 = _createVec3WithAxis( _pv.x - _dw,
                                            2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x - _dw, _pv.z - _dd ),
                                            _pv.z - _dd,
                                            axis );
            auto _p2 = _createVec3WithAxis( _pv.x, 
                                            2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x, _pv.z - _dd ),
                                            _pv.z - _dd,
                                            axis );
            auto _p3 = _createVec3WithAxis( _pv.x + _dw, 
                                            2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x + _dw, _pv.z - _dd ),
                                            _pv.z - _dd,
                                            axis );
            auto _p4 = _createVec3WithAxis( _pv.x - _dw, 
                                            2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x - _dw, _pv.z ),
                                            _pv.z,
                                            axis );
            auto _p6 = _createVec3WithAxis( _pv.x + _dw, 
                                            2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x + _dw, _pv.z ),
                                            _pv.z,
                                            axis );
            auto _p7 = _createVec3WithAxis( _pv.x - _dw, 
                                            2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x - _dw, _pv.z + _dd ),
                                            _pv.z + _dd,
                                            axis );
            auto _p8 = _createVec3WithAxis( _pv.x, 
                                            2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x, _pv.z + _dd ),
                                            _pv.z + _dd,
                                            axis );
            auto _p9 = _createVec3WithAxis( _pv.x + _dw, 
                                            2.0f + 2.0f * _hmapGenerator.getHeight( _pv.x + _dw, _pv.z + _dd ),
                                            _pv.z + _dd,
                                            axis );

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

        return new LMesh( _vertices, _normals, _texCoord, _indices );
    }

    LVec3 LMeshBuilder::_createVec3WithAxis( float x, float y, float z, const std::string& axis )
    {
        if ( axis == "x" )
            return LVec3( y, z, x );
        else if ( axis == "y" )
            return LVec3( x, y, z );
        else if ( axis == "z" )
            return LVec3( z, x, y );

        return LVec3( x, y, z );
    }

}

#include <graphics/CMeshBuilder.h>

namespace engine
{

    CMesh* CMeshBuilder::createPlane( float sizeX, float sizeY, const eAxis& axis )
    {
        std::vector< CVec3 > _vertices = { _rotateToMatchUpAxis( {  0.5f * sizeX, -0.5f * sizeY, 0.0f }, axis ),
                                           _rotateToMatchUpAxis( {  0.5f * sizeX,  0.5f * sizeY, 0.0f }, axis ),
                                           _rotateToMatchUpAxis( { -0.5f * sizeX,  0.5f * sizeY, 0.0f }, axis ),
                                           _rotateToMatchUpAxis( { -0.5f * sizeX, -0.5f * sizeY, 0.0f }, axis ) };

        std::vector< CVec3 > _normals = { _rotateToMatchUpAxis( { 0.0f, 0.0f, 1.0f }, axis ), 
                                          _rotateToMatchUpAxis( { 0.0f, 0.0f, 1.0f }, axis ),
                                          _rotateToMatchUpAxis( { 0.0f, 0.0f, 1.0f }, axis ),
                                          _rotateToMatchUpAxis( { 0.0f, 0.0f, 1.0f }, axis ) };

        std::vector< CVec2 > _texCoords = { { 0.5f * sizeX * 0.0f, 0.5f * sizeY * 0.0f }, 
                                            { 0.5f * sizeX * 1.0f, 0.5f * sizeY * 0.0f },
                                            { 0.5f * sizeX * 1.0f, 0.5f * sizeY * 1.0f }, 
                                            { 0.5f * sizeX * 0.0f, 0.5f * sizeY * 1.0f } };

        std::vector< CInd3 > _indices = { { 0, 1, 2 }, { 0, 2, 3 } };

        auto _name = std::string( "plane:" ) + std::to_string( CMeshBuilder::s_numPlanes++ );

        return new CMesh( _name, _vertices, _normals, _texCoords, _indices );
    }

    CMesh* CMeshBuilder::createBox( float sizeX, float sizeY, float sizeZ )
    {
        std::vector< CVec3 > _vertices;
        std::vector< CVec3 > _normals;
        std::vector< CVec2 > _texCoords;
        std::vector< CInd3 > _indices;

        std::vector< CVec3 > _normalsSource = { { 0.0f,  0.0f,  1.0f },
                                                { 0.0f,  0.0f, -1.0f },
                                                { 0.0f,  1.0f,  0.0f },
                                                { 0.0f, -1.0f,  0.0f },
                                                { 1.0f,  0.0f,  0.0f },
                                                {-1.0f,  0.0f,  0.0f } };

        auto _scale = CVec3( 0.5 * sizeX, 0.5 * sizeY, 0.5 * sizeZ );

        // for each face, compute the vertices that form the face perpendicular to that normal
        for ( int q = 0; q < _normalsSource.size(); q++ )
        {
            // Add the indices accordingly
            _indices.push_back( { (GLint)_vertices.size(), (GLint)_vertices.size() + 1, (GLint)_vertices.size() + 2 } );
            _indices.push_back( { (GLint)_vertices.size(), (GLint)_vertices.size() + 2, (GLint)_vertices.size() + 3 } );

            // Form a perpendicular right hand system based on the current normal
            CVec3 _n = _normalsSource[q];
            CVec3 _s1 = { _n.y, _n.z, _n.x };
            CVec3 _s2 = CVec3::cross( _n, _s1 );

            // Generate each vertex of each face according to these vectors
            auto _v0 = CVec3::scale( _n - _s1 - _s2, _scale );
            auto _v1 = CVec3::scale( _n + _s1 - _s2, _scale );
            auto _v2 = CVec3::scale( _n + _s1 + _s2, _scale );
            auto _v3 = CVec3::scale( _n - _s1 + _s2, _scale );

            _vertices.push_back( _v0 );
            _vertices.push_back( _v1 );
            _vertices.push_back( _v2 );
            _vertices.push_back( _v3 );

            _texCoords.push_back( { 0.0f, 0.0f } );
            _texCoords.push_back( { 1.0f, 0.0f } );
            _texCoords.push_back( { 1.0f, 1.0f } );
            _texCoords.push_back( { 0.0f, 1.0f } );

            _normals.push_back( _n );
            _normals.push_back( _n );
            _normals.push_back( _n );
            _normals.push_back( _n );
        }

        auto _name = std::string( "box:" ) + std::to_string( CMeshBuilder::s_numBoxes++ );

        return new CMesh( _name, _vertices, _normals, _texCoords, _indices );
    }

    CMesh* CMeshBuilder::createSphere( float radius, int nDiv1, int nDiv2 )
    {
        std::vector< CVec3 > _vertices;
        std::vector< CVec3 > _normals;
        std::vector< CVec2 > _texCoords;
        std::vector< CInd3 > _indices;

        // Construct all vertices using spherical coordinates, except the poles
        for ( size_t i = 0; i <= nDiv1; i++ )
        {
            for ( size_t j = 0; j <= nDiv2; j++ )
            {
                float _theta = 2.0f * ENGINE_PI * ( ( ( float ) i ) / nDiv1 );
                float _phi = -ENGINE_PI / 2.0f + ENGINE_PI * ( ( ( float ) j ) / nDiv2 );

                auto _vertex = CVec3( radius * cos( _theta ) * cos( _phi ),
                                      radius * sin( _theta ) * cos( _phi ),
                                      radius * sin( _phi ) );
                auto _normal = CVec3::normalize( _vertex );

                _vertices.push_back( _vertex );
                _normals.push_back( _normal );

                // uvs-calculations adapted from threejs repo:
                //      https://github.com/mrdoob/three.js/blob/d80e106de2f096cf8b50b67b4d029dd0b64370e8/src/geometries/SphereGeometry.js#L86
                float _vOffset = 0.0f;
                _vOffset = ( j == 0 ) ? ( 0.5f / nDiv1 ) : _vOffset;
                _vOffset = ( j == nDiv2 ) ? ( -0.5f / nDiv1 ) : _vOffset;

                float _u = ((float)i) / nDiv1;
                float _v = ((float)j) / nDiv2;
                _texCoords.push_back( { _u, _v + _vOffset } );
            }
        }

        // Compute the indices for this tessellation (intermediate part without poles)
        for ( size_t i = 0; i < nDiv1; i++ )
        {
            for ( size_t j = 0; j < nDiv2; j++ )
            {
                int _ip0 = ( i + 0 ) * ( nDiv2 + 1 ) + ( j + 0 );
                int _ip1 = ( i + 1 ) * ( nDiv2 + 1 ) + ( j + 0 );
                int _ip2 = ( i + 1 ) * ( nDiv2 + 1 ) + ( j + 1 );
                int _ip3 = ( i + 0 ) * ( nDiv2 + 1 ) + ( j + 1 );

                if ( j != 0 )
                    _indices.push_back( { _ip0, _ip1, _ip2 } );

                if ( j != ( nDiv2 - 1 ) )
                    _indices.push_back( { _ip0, _ip2, _ip3 } );
            }
        }

        auto _name = std::string( "sphere:" ) + std::to_string( CMeshBuilder::s_numSpheres++ );

        return new CMesh( _name, _vertices, _normals, _texCoords, _indices );
    }

    CMesh* CMeshBuilder::createEllipsoid( float radX, float radY, float radZ, int nDiv1, int nDiv2 )
    {
        std::vector< CVec3 > _vertices;
        std::vector< CVec3 > _normals;
        std::vector< CVec2 > _texCoords;
        std::vector< CInd3 > _indices;

        // Construct all vertices using spherical coordinates, except the poles
        for ( size_t i = 0; i <= nDiv1; i++ )
        {
            for ( size_t j = 0; j <= nDiv2; j++ )
            {
                float _theta = 2.0f * ENGINE_PI * ( ( ( float ) i ) / nDiv1 );
                float _phi = -ENGINE_PI / 2.0f + ENGINE_PI * ( ( ( float ) j ) / nDiv2 );

                auto _vertex = CVec3( radX * cos( _theta ) * cos( _phi ),
                                      radY * sin( _theta ) * cos( _phi ),
                                      radZ * sin( _phi ) );
                auto _normal = CVec3::normalize( _vertex );

                _vertices.push_back( _vertex );
                _normals.push_back( _normal );

                float _vOffset = 0.0f;
                _vOffset = ( j == 0 ) ? ( 0.5f / nDiv1 ) : _vOffset;
                _vOffset = ( j == nDiv2 ) ? ( -0.5f / nDiv1 ) : _vOffset;

                float _u = ((float)i) / nDiv1;
                float _v = ((float)j) / nDiv2;
                _texCoords.push_back( { _u, _v + _vOffset } );
            }
        }

        // Compute the indices for this tessellation (intermediate part without poles)
        for ( size_t i = 0; i < nDiv1; i++ )
        {
            for ( size_t j = 0; j < nDiv2; j++ )
            {
                int _ip0 = ( i + 0 ) * ( nDiv2 + 1 ) + ( j + 0 );
                int _ip1 = ( i + 1 ) * ( nDiv2 + 1 ) + ( j + 0 );
                int _ip2 = ( i + 1 ) * ( nDiv2 + 1 ) + ( j + 1 );
                int _ip3 = ( i + 0 ) * ( nDiv2 + 1 ) + ( j + 1 );

                if ( j != 0 )
                    _indices.push_back( { _ip0, _ip1, _ip2 } );

                if ( j != ( nDiv2 - 1 ) )
                    _indices.push_back( { _ip0, _ip2, _ip3 } );
            }
        }

        auto _name = std::string( "ellipsoid:" ) + std::to_string( CMeshBuilder::s_numEllipsoids++ );

        return new CMesh( _name, _vertices, _normals, _texCoords, _indices );
    }

    CMesh* CMeshBuilder::createCylinder( float radius, float height, const eAxis& axis, int nDiv1 )
    {
        std::vector< CVec3 > _vertices;
        std::vector< CVec3 > _normals;
        std::vector< CVec2 > _texCoords;
        std::vector< CInd3 > _indices;

        // Compute points in any section ***********************************************************
        std::vector< CVec3 > _sectionXY;
        for ( int q = 0; q <= nDiv1; q++ )
        {
            float _x = radius * cos( 2.0f * ENGINE_PI * ( ( (float) q ) / nDiv1 ) );
            float _y = radius * sin( 2.0f * ENGINE_PI * ( ( (float) q ) / nDiv1 ) );
            _sectionXY.push_back( { _x, _y, 0.0f } );
        }

        // Construct cylinder geometry *************************************************************
        int _baseIndx = 0;

        ////////// Up base //////////
        for ( int q = 0; q < nDiv1; q++ )
        {
            _vertices.push_back( _rotateToMatchUpAxis( _sectionXY[q] + CVec3( 0.0f, 0.0f, 0.5f * height ), axis ) );
            _normals.push_back( _rotateToMatchUpAxis( { 0.0f, 0.0f, 1.0f }, axis ) );

            // @TODO: Check this part, there might be something wrong with the texture coordinates
            _texCoords.push_back( { 0.5f + ( _sectionXY[q].x / ( 2.0f * radius ) ),
                                    0.5f + ( _sectionXY[q].y / ( 2.0f * radius ) ) } );
        }

        for ( int q = 1; q <= nDiv1 - 2; q++ )
            _indices.push_back( CInd3( _baseIndx, _baseIndx + q, _baseIndx + q + 1 ) );

        _baseIndx += _vertices.size();

        ////////// Body surface //////////
        for ( int q = 0; q < nDiv1; q++ )
        {
            // quad vertices
            auto _p0 = _sectionXY[q + 0] + CVec3( 0.0f, 0.0f, -0.5f * height );
            auto _p1 = _sectionXY[q + 1] + CVec3( 0.0f, 0.0f, -0.5f * height );
            auto _p2 = _sectionXY[q + 1] + CVec3( 0.0f, 0.0f, 0.5f * height );
            auto _p3 = _sectionXY[q + 0] + CVec3( 0.0f, 0.0f, 0.5f * height );

            _vertices.push_back( _rotateToMatchUpAxis( _p0, axis ) );
            _vertices.push_back( _rotateToMatchUpAxis( _p1, axis ) );
            _vertices.push_back( _rotateToMatchUpAxis( _p2, axis ) );
            _vertices.push_back( _rotateToMatchUpAxis( _p3, axis ) );

            _texCoords.push_back( { ( (float) ( q + 0 ) ) / nDiv1, 0.0f } );
            _texCoords.push_back( { ( (float) ( q + 1 ) ) / nDiv1, 0.0f } );
            _texCoords.push_back( { ( (float) ( q + 1 ) ) / nDiv1, 1.0f } );
            _texCoords.push_back( { ( (float) ( q + 0 ) ) / nDiv1, 1.0f } );

            auto _nQuad1 = _rotateToMatchUpAxis( CVec3::normalize( _sectionXY[q + 0] ), axis );
            auto _nQuad2 = _rotateToMatchUpAxis( CVec3::normalize( _sectionXY[q + 1] ), axis );

            _normals.push_back( _nQuad1 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad1 );

            _indices.push_back( { _baseIndx, _baseIndx + 1, _baseIndx + 2 } );
            _indices.push_back( { _baseIndx, _baseIndx + 2, _baseIndx + 3 } );

            _baseIndx += 4;
        }

        ////////// Down base //////////
        for ( int q = 0; q < nDiv1; q++ )
        {
            _vertices.push_back( _rotateToMatchUpAxis( _sectionXY[q] + CVec3( 0.0f, 0.0f, -0.5f * height ), axis ) );
            _normals.push_back( _rotateToMatchUpAxis( { 0.0f, 0.0f, -1.0f }, axis ) );

            // @TODO: Check this part, there might be something wrong with the texture coordinates
            _texCoords.push_back( { 0.5f + ( _sectionXY[q].x / ( 2.0f * radius ) ),
                                    0.5f + ( _sectionXY[q].y / ( 2.0f * radius ) ) } );
        }

        for ( int q = 1; q <= nDiv1 - 2; q++ )
            _indices.push_back( { _baseIndx, _baseIndx + q + 1, _baseIndx + q } );

        auto _name = std::string( "cylinder:" ) + std::to_string( CMeshBuilder::s_numCylinders++ );

        return new CMesh( _name, _vertices, _normals, _texCoords, _indices );
    }

    CMesh* CMeshBuilder::createCapsule( float radius, float height, const eAxis& axis, int nDiv1, int nDiv2 )
    {
        CMesh* _mesh = NULL;

        std::vector< CVec3 > _vertices;
        std::vector< CVec3 > _normals;
        std::vector< CVec2 > _texCoords;
        std::vector< CInd3 > _indices;

        /* Capsule format
        *
        *   |-- radius --|-- height --|-- radius --|
        */
        
        // Tessellate using cap-surface-cap approach

        // Build up cap *********************************
        int _baseIndx = 0;
        for ( size_t i = 0; i <= nDiv1; i++ )
        {
            for ( size_t j = 0; j <= nDiv2; j++ )
            {
                float _theta = 2.0f * ENGINE_PI * ( ( ( float ) i ) / nDiv1 );
                float _phi = 0.5f * ENGINE_PI * ( ( ( float ) j ) / nDiv2 );

                // vertex without offset to up-position
                auto _vertex = _rotateToMatchUpAxis( CVec3( radius * cos( _theta ) * cos( _phi ),
                                                            radius * sin( _theta ) * cos( _phi ),
                                                            radius * sin( _phi ) ),
                                                     axis );
                auto _normal = CVec3::normalize( _vertex );
                // apply offset to up position
                _vertex = _vertex + _rotateToMatchUpAxis( { 0.0f, 0.0f, 0.5f * height }, axis );

                _vertices.push_back( _vertex );
                _normals.push_back( _normal );

                // uvs-calculations adapted from threejs repo:
                //      https://github.com/mrdoob/three.js/blob/d80e106de2f096cf8b50b67b4d029dd0b64370e8/src/geometries/SphereGeometry.js#L86
                float _vOffset = ( j == nDiv2 ) ? ( -0.5f / nDiv1 ) : _vOffset;

                float _u = ((float)i) / nDiv1;
                float _v = 0.5f + 0.5f * ( ( (float) j ) / nDiv2 );
                _texCoords.push_back( { _u, _v } );
            }
        }

        for ( size_t i = 0; i < nDiv1; i++ )
        {
            for ( size_t j = 0; j < nDiv2; j++ )
            {
                int _ip0 = ( i + 0 ) * ( nDiv2 + 1 ) + ( j + 0 );
                int _ip1 = ( i + 1 ) * ( nDiv2 + 1 ) + ( j + 0 );
                int _ip2 = ( i + 1 ) * ( nDiv2 + 1 ) + ( j + 1 );
                int _ip3 = ( i + 0 ) * ( nDiv2 + 1 ) + ( j + 1 );

                _indices.push_back( { _baseIndx + _ip0, _baseIndx + _ip1, _baseIndx + _ip2 } );

                if ( j != ( nDiv2 - 1 ) )
                    _indices.push_back( { _ip0, _ip2, _ip3 } );
            }
        }

        _baseIndx += _vertices.size();
        // Build surface *******************************

        // calculate section geometry
        std::vector< CVec3 > _sectionXY;
        for ( int q = 0; q <= nDiv1; q++ )
        {
            float _x = radius * cos( 2.0f * ENGINE_PI * ( ( (float) q ) / nDiv1 ) );
            float _y = radius * sin( 2.0f * ENGINE_PI * ( ( (float) q ) / nDiv1 ) );
            _sectionXY.push_back( { _x, _y, 0.0f } );
        }

        // construct cylindrical surface
        for ( int q = 0; q < nDiv1; q++ )
        {
            // quad vertices
            auto _p0 = _sectionXY[q + 0] + CVec3( 0.0f, 0.0f, -0.5f * height );
            auto _p1 = _sectionXY[q + 1] + CVec3( 0.0f, 0.0f, -0.5f * height );
            auto _p2 = _sectionXY[q + 1] + CVec3( 0.0f, 0.0f, 0.5f * height );
            auto _p3 = _sectionXY[q + 0] + CVec3( 0.0f, 0.0f, 0.5f * height );

            _vertices.push_back( _rotateToMatchUpAxis( _p0, axis ) );
            _vertices.push_back( _rotateToMatchUpAxis( _p1, axis ) );
            _vertices.push_back( _rotateToMatchUpAxis( _p2, axis ) );
            _vertices.push_back( _rotateToMatchUpAxis( _p3, axis ) );

            auto _nQuad1 = _rotateToMatchUpAxis( CVec3::normalize( _sectionXY[( q + 0 ) % _sectionXY.size()] ), axis );
            auto _nQuad2 = _rotateToMatchUpAxis( CVec3::normalize( _sectionXY[( q + 1 ) % _sectionXY.size()] ), axis );

            _normals.push_back( _nQuad1 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad1 );

            _texCoords.push_back( { ( (float) ( q + 0 ) ) / nDiv1, 0.0f } );
            _texCoords.push_back( { ( (float) ( q + 1 ) ) / nDiv1, 0.0f } );
            _texCoords.push_back( { ( (float) ( q + 1 ) ) / nDiv1, 1.0f } );
            _texCoords.push_back( { ( (float) ( q + 0 ) ) / nDiv1, 1.0f } );

            _indices.push_back( { _baseIndx, _baseIndx + 1, _baseIndx + 2 } );
            _indices.push_back( { _baseIndx, _baseIndx + 2, _baseIndx + 3 } );

            _baseIndx += 4;
        }

        // Build down cap ******************************
        for ( size_t i = 0; i <= nDiv1; i++ )
        {
            for ( size_t j = 0; j <= nDiv2; j++ )
            {
                float _theta = 2.0f * ENGINE_PI * ( ( ( float ) i ) / nDiv1 );
                float _phi = -0.5f * ENGINE_PI * ( ( ( float ) j ) / nDiv2 );

                // vertex without offset to up-position
                auto _vertex = _rotateToMatchUpAxis( CVec3( radius * cos( _theta ) * cos( _phi ),
                                                            radius * sin( _theta ) * cos( _phi ),
                                                            radius * sin( _phi ) ),
                                                     axis );
                auto _normal = CVec3::normalize( _vertex );
                // apply offset to up position
                _vertex = _vertex - _rotateToMatchUpAxis( { 0.0f, 0.0f, 0.5f * height }, axis );

                _vertices.push_back( _vertex );
                _normals.push_back( _normal );

                // uvs-calculations adapted from threejs repo:
                //      https://github.com/mrdoob/three.js/blob/d80e106de2f096cf8b50b67b4d029dd0b64370e8/src/geometries/SphereGeometry.js#L86
                float _vOffset = ( j == nDiv2 ) ? ( -0.5f / nDiv1 ) : _vOffset;

                float _u = ((float)i) / nDiv1;
                float _v = 0.5f + 0.5f * ( ( (float) j ) / nDiv2 );
                _texCoords.push_back( { _u, 1.0f - ( _v ) } );
            }
        }

        for ( size_t i = 0; i < nDiv1; i++ )
        {
            for ( size_t j = 0; j < nDiv2; j++ )
            {
                int _ip0 = ( i + 0 ) * ( nDiv2 + 1 ) + ( j + 0 );
                int _ip1 = ( i + 1 ) * ( nDiv2 + 1 ) + ( j + 0 );
                int _ip2 = ( i + 1 ) * ( nDiv2 + 1 ) + ( j + 1 );
                int _ip3 = ( i + 0 ) * ( nDiv2 + 1 ) + ( j + 1 );

                _indices.push_back( { _baseIndx + _ip0, _baseIndx + _ip1, _baseIndx + _ip2 } );

                if ( j != ( nDiv2 - 1 ) )
                    _indices.push_back( { _baseIndx + _ip0, _baseIndx + _ip2, _baseIndx + _ip3 } );
            }
        }

        auto _name = std::string( "capsule:" ) + std::to_string( CMeshBuilder::s_numCapsules++ );

        return new CMesh( _name, _vertices, _normals, _texCoords, _indices );
    }

    CMesh* CMeshBuilder::createArrow( float length, const eAxis& axis )
    {
        std::vector< CVec3 > _vertices;
        std::vector< CVec3 > _normals;
        std::vector< CVec2 > _texCoords;
        std::vector< CInd3 > _indices;

        // Arrow construction parameters
        const int _nDiv1 = 10;
        const float _radiusCyl  = 0.05 * length;
        const float _radiusCone = 0.075 * length;
        const float _lengthCyl  = 0.8 * length;
        const float _lengthCone = 0.2 * length;

        // Tesselate cylinder ***********************************************************************

        // calculate section geometry
        std::vector< CVec3 > _sectionXY;
        for ( size_t q = 0; q <= _nDiv1; q++ )
        {
            float _x = _radiusCyl * cos( 2.0f * ENGINE_PI * ( ( (float) q ) / _nDiv1 ) );
            float _y = _radiusCyl * sin( 2.0f * ENGINE_PI * ( ( (float) q ) / _nDiv1 ) );
            _sectionXY.push_back( { _x, _y, 0.0f } );
        }

        // calculate cylinder geometry
        int _baseIndx = 0;

        // down base ****************************************
        for ( size_t q = 0; q < _nDiv1; q++ )
        {
            _vertices.push_back( _rotateToMatchUpAxis( _sectionXY[q], axis ) );
            _normals.push_back( _rotateToMatchUpAxis( { 0.0f, 0.0f, -1.0f }, axis ) );

            _texCoords.push_back( { 0.5f + ( _sectionXY[q].x / ( 2.0f * _radiusCyl ) ),
                                    0.5f + ( _sectionXY[q].y / ( 2.0f * _radiusCyl ) ) } );
        }

        for ( size_t q = 1; q <= _nDiv1 - 2; q++ )
        {
            _indices.push_back( CInd3( _baseIndx, _baseIndx + q + 1, _baseIndx + q ) );
        }

        _baseIndx += _vertices.size();
        // **************************************************

        // body surface *************************************
        for ( size_t q = 0; q < _nDiv1; q++ )
        {
            // quad vertices
            auto _p0 = _sectionXY[q + 0] + CVec3( 0.0f, 0.0f, 0.0f );
            auto _p1 = _sectionXY[q + 1] + CVec3( 0.0f, 0.0f, 0.0f );
            auto _p2 = _sectionXY[q + 1] + CVec3( 0.0f, 0.0f, _lengthCyl );
            auto _p3 = _sectionXY[q + 0] + CVec3( 0.0f, 0.0f, _lengthCyl );

            _vertices.push_back( _rotateToMatchUpAxis( _p0, axis ) );
            _vertices.push_back( _rotateToMatchUpAxis( _p1, axis ) );
            _vertices.push_back( _rotateToMatchUpAxis( _p2, axis ) );
            _vertices.push_back( _rotateToMatchUpAxis( _p3, axis ) );

            // @TODO: Check this part, there might be something wrong with the texture coordinates
            _texCoords.push_back( { ( (float) ( q + 0 ) ) / _nDiv1, 0.0f } );
            _texCoords.push_back( { ( (float) ( q + 1 ) ) / _nDiv1, 0.0f } );
            _texCoords.push_back( { ( (float) ( q + 1 ) ) / _nDiv1, 1.0f } );
            _texCoords.push_back( { ( (float) ( q + 0 ) ) / _nDiv1, 1.0f } );

            auto _nQuad1 = _rotateToMatchUpAxis( CVec3::normalize( _sectionXY[q + 0] ), axis );
            auto _nQuad2 = _rotateToMatchUpAxis( CVec3::normalize( _sectionXY[q + 1] ), axis );

            _normals.push_back( _nQuad1 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad2 );
            _normals.push_back( _nQuad1 );

            _indices.push_back( { _baseIndx, _baseIndx + 1, _baseIndx + 2 } );
            _indices.push_back( { _baseIndx, _baseIndx + 2, _baseIndx + 3 } );

            _baseIndx += 4;
        }
        // **************************************************

        // Tesselate cone ***************************************************************************

        // Build base points
        std::vector< CVec3 > _sectionXYConeOut;
        std::vector< CVec3 > _sectionXYConeIn;
        for ( size_t q = 0; q < _nDiv1; q++ )
        {
            float _xOut = _radiusCone * cos( 2.0f * ENGINE_PI * ( ( (float) q ) / _nDiv1 ) );
            float _yOut = _radiusCone * sin( 2.0f * ENGINE_PI * ( ( (float) q ) / _nDiv1 ) );
            _sectionXYConeOut.push_back( { _xOut, _yOut, 0.0f } );

            float _xIn = _radiusCyl * cos( 2.0f * ENGINE_PI * ( ( (float) q ) / _nDiv1 ) );
            float _yIn = _radiusCyl * sin( 2.0f * ENGINE_PI * ( ( (float) q ) / _nDiv1 ) );
            _sectionXYConeIn.push_back( { _xIn, _yIn, 0.0f } );
        }

        // Build surface - tesselate using strips of triangles
        for ( size_t q = 0; q < _sectionXYConeOut.size(); q++ )
        {
            _indices.push_back( { (GLint) _vertices.size(), (GLint) _vertices.size() + 1, (GLint) _vertices.size() + 2 } );

            auto _p0 = _rotateToMatchUpAxis( _sectionXYConeOut[( q + 0 ) % _sectionXYConeOut.size()] + CVec3( 0, 0, _lengthCyl ), axis );
            auto _p1 = _rotateToMatchUpAxis( _sectionXYConeOut[( q + 1 ) % _sectionXYConeOut.size()] + CVec3( 0, 0, _lengthCyl ), axis );
            auto _p2 = _rotateToMatchUpAxis( { 0, 0, _lengthCyl + _lengthCone }, axis );

            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );

            auto _nQuad0 = _rotateToMatchUpAxis( _sectionXYConeOut[(q + 0) % _sectionXYConeOut.size()], axis );
            auto _nQuad1 = _rotateToMatchUpAxis( _sectionXYConeOut[(q + 1) % _sectionXYConeOut.size()], axis );
            auto _nQuad2 = _rotateToMatchUpAxis( { 0.0f, 0.0f, 1.0f }, axis );

            _normals.push_back( _nQuad0 );
            _normals.push_back( _nQuad1 );
            _normals.push_back( _nQuad2 );

            _texCoords.push_back( { ( q + 0.0f ) / _sectionXYConeOut.size(), 1.0f } );
            _texCoords.push_back( { ( q + 1.0f ) / _sectionXYConeOut.size(), 1.0f } );
            _texCoords.push_back( { ( q + 0.5f ) / _sectionXYConeOut.size(), 0.0f } );
        }

        // Build bottom base - strip of "kind of quads" ( ring tessellation )
        _baseIndx = _vertices.size();

        for ( size_t q = 0; q < _sectionXYConeOut.size(); q++ )
        {
            auto _p0 = _rotateToMatchUpAxis( _sectionXYConeOut[( q + 0 ) % _sectionXYConeOut.size()] + CVec3( 0, 0, _lengthCyl ), axis );
            auto _p1 = _rotateToMatchUpAxis( _sectionXYConeIn[( q + 0 ) % _sectionXYConeIn.size()] + CVec3( 0, 0, _lengthCyl ), axis );
            auto _p2 = _rotateToMatchUpAxis( _sectionXYConeIn[( q + 1 ) % _sectionXYConeIn.size()] + CVec3( 0, 0, _lengthCyl ), axis );
            auto _p3 = _rotateToMatchUpAxis( _sectionXYConeOut[( q + 1 ) % _sectionXYConeOut.size()] + CVec3( 0, 0, _lengthCyl ), axis );

            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );
            _vertices.push_back( _p3 );

            auto _normal = _rotateToMatchUpAxis( { 0.0f, 0.0f, -1.0f }, axis );

            _normals.push_back( _normal );
            _normals.push_back( _normal );
            _normals.push_back( _normal );
            _normals.push_back( _normal );

            _texCoords.push_back( { 0.5f + ( _sectionXYConeOut[q].x / ( 2.0f * _radiusCone ) ),
                                    0.5f + ( _sectionXYConeOut[q].y / ( 2.0f * _radiusCone ) ) } );

            _texCoords.push_back( { 0.5f + ( _sectionXYConeIn[q].x / ( 2.0f * _radiusCone ) ),
                                    0.5f + ( _sectionXYConeIn[q].y / ( 2.0f * _radiusCone ) ) } );

            _texCoords.push_back( { 0.5f + ( _sectionXYConeIn[( q + 1 ) % _sectionXYConeIn.size()].x / ( 2.0f * _radiusCone ) ),
                                    0.5f + ( _sectionXYConeIn[( q + 1 ) % _sectionXYConeIn.size()].y / ( 2.0f * _radiusCone ) ) } );

            _texCoords.push_back( { 0.5f + ( _sectionXYConeOut[( q + 1 ) % _sectionXYConeOut.size()].x / ( 2.0f * _radiusCone ) ),
                                    0.5f + ( _sectionXYConeOut[( q + 1 ) % _sectionXYConeOut.size()].y / ( 2.0f * _radiusCone ) ) } );

            _indices.push_back( { _baseIndx, _baseIndx + 2, _baseIndx + 1 } );
            _indices.push_back( { _baseIndx, _baseIndx + 3, _baseIndx + 2 } );

            _baseIndx += 4;
        }

        auto _name = std::string( "arrow:" ) + std::to_string( CMeshBuilder::s_numArrows++ );

        return new CMesh( _name, _vertices, _normals,_texCoords, _indices );
    }

    CModel* CMeshBuilder::createAxes( float length )
    {
        auto _name = std::string( "axes:" ) + std::to_string( CMeshBuilder::s_numAxes++ );
        auto _axesModel = new CModel( _name );

        auto _axisX         = std::unique_ptr< CMesh >( CMeshBuilder::createArrow( length, eAxis::X ) );
        auto _axisY         = std::unique_ptr< CMesh >( CMeshBuilder::createArrow( length, eAxis::Y ) );
        auto _axisZ         = std::unique_ptr< CMesh >( CMeshBuilder::createArrow( length, eAxis::Z ) );
        auto _axisCenter    = std::unique_ptr< CMesh >( CMeshBuilder::createSphere( 0.2 * length ) );

        dynamic_cast< CPhongMaterial* >( _axisX->material() )->ambient  = { 1.0, 0.0, 0.0 };
        dynamic_cast< CPhongMaterial* >( _axisX->material() )->diffuse  = { 1.0, 0.0, 0.0 };
        dynamic_cast< CPhongMaterial* >( _axisX->material() )->specular = { 1.0, 0.0, 0.0 };

        dynamic_cast< CPhongMaterial* >( _axisY->material() )->ambient  = { 0.0, 1.0, 0.0 };
        dynamic_cast< CPhongMaterial* >( _axisY->material() )->diffuse  = { 0.0, 1.0, 0.0 };
        dynamic_cast< CPhongMaterial* >( _axisY->material() )->specular = { 0.0, 1.0, 0.0 };

        dynamic_cast< CPhongMaterial* >( _axisZ->material() )->ambient  = { 0.0, 0.0, 1.0 };
        dynamic_cast< CPhongMaterial* >( _axisZ->material() )->diffuse  = { 0.0, 0.0, 1.0 };
        dynamic_cast< CPhongMaterial* >( _axisZ->material() )->specular = { 0.0, 0.0, 1.0 };

        dynamic_cast< CPhongMaterial* >( _axisCenter->material() )->ambient  = { 0.3, 0.3, 0.3 };
        dynamic_cast< CPhongMaterial* >( _axisCenter->material() )->diffuse  = { 0.3, 0.3, 0.3 };
        dynamic_cast< CPhongMaterial* >( _axisCenter->material() )->specular = { 0.3, 0.3, 0.3 };

        _axesModel->addMesh( std::move( _axisX ) );
        _axesModel->addMesh( std::move( _axisY ) );
        _axesModel->addMesh( std::move( _axisZ ) );
        _axesModel->addMesh( std::move( _axisCenter ) );

        return _axesModel;
    }

    CMesh* CMeshBuilder::createPerlinPatch( float width, float depth, int cellDivision, 
                                            int nOctaves, float nPersistance, float nLacunarity, float nScale, 
                                            const eAxis& axis )
    {
        std::vector< CVec3 > _vertices;
        std::vector< CVec3 > _normals;
        std::vector< CInd3 > _indices;
        std::vector< CVec2 > _texCoord;

        CNoiseGenerator::Config( nOctaves, nPersistance, nLacunarity, nScale );

        float _dw = width / cellDivision;
        float _dd = depth / cellDivision;

        float _x0 = -( cellDivision / 2.0f ) * _dw;
        float _y0 = -( cellDivision / 2.0f ) * _dd;

        for ( int i = 0; i < cellDivision; i++ )
        {
            for ( int j = 0; j < cellDivision; j++ )
            {
                auto _p0 = _rotateToMatchUpAxis( CVec3( _x0 + ( j + 0 ) * _dw, 
                                                        _y0 + ( i + 0 ) * _dd, 
                                                        2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _x0 + ( j + 0 ) * _dw, _y0 + ( i + 0 ) * _dd ) ), axis );

                auto _p1 = _rotateToMatchUpAxis( CVec3( _x0 + ( j + 1 ) * _dw, 
                                                        _y0 + ( i + 0 ) * _dd, 
                                                        2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _x0 + ( j + 1 ) * _dw, _y0 + ( i + 0 ) * _dd ) ), axis );

                auto _p2 = _rotateToMatchUpAxis( CVec3( _x0 + ( j + 1 ) * _dw, 
                                                        _y0 + ( i + 1 ) * _dd, 
                                                        2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _x0 + ( j + 1 ) * _dw, _y0 + ( i + 1 ) * _dd ) ), axis );

                auto _p3 = _rotateToMatchUpAxis( CVec3( _x0 + ( j + 0 ) * _dw, 
                                                        _y0 + ( i + 1 ) * _dd, 
                                                        2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _x0 + ( j + 0 ) * _dw, _y0 + ( i + 1 ) * _dd ) ), axis );

                // TODO: For now just make dummy indices for the rigid bodies :/
                _indices.push_back( { (GLint) _vertices.size() + 0, (GLint) _vertices.size() + 1, (GLint) _vertices.size() + 2 } );
                _indices.push_back( { (GLint) _vertices.size() + 3, (GLint) _vertices.size() + 4, (GLint) _vertices.size() + 5 } );

                CVec2 _t0( 0, 0 );
                CVec2 _t1( 0, 1 );
                CVec2 _t2( 1, 1 );
                CVec2 _t3( 1, 0 );

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

                auto _nt1 = CVec3::cross( _p1 - _p0, _p2 - _p1 );
                auto _nt2 = CVec3::cross( _p2 - _p0, _p3 - _p2 );

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

            auto _pv = _rotateBackFromUpAxis( _vertices[q], axis );

            auto _p1 = _rotateToMatchUpAxis( { _pv.x - _dw, _pv.y + _dd, 2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _pv.x - _dw, _pv.y + _dd ) }, axis );
            auto _p2 = _rotateToMatchUpAxis( { _pv.x      , _pv.y + _dd, 2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _pv.x      , _pv.y + _dd ) }, axis );
            auto _p3 = _rotateToMatchUpAxis( { _pv.x + _dw, _pv.y + _dd, 2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _pv.x + _dw, _pv.y + _dd ) }, axis );
            auto _p4 = _rotateToMatchUpAxis( { _pv.x - _dw, _pv.y      , 2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _pv.x - _dw, _pv.y       ) }, axis );
            auto _p6 = _rotateToMatchUpAxis( { _pv.x + _dw, _pv.y      , 2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _pv.x + _dw, _pv.y       ) }, axis );
            auto _p7 = _rotateToMatchUpAxis( { _pv.x - _dw, _pv.y - _dd, 2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _pv.x - _dw, _pv.y - _dd ) }, axis );
            auto _p8 = _rotateToMatchUpAxis( { _pv.x      , _pv.y - _dd, 2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _pv.x      , _pv.y - _dd ) }, axis );
            auto _p9 = _rotateToMatchUpAxis( { _pv.x + _dw, _pv.y - _dd, 2.0f + 2.0f * CNoiseGenerator::GetNoise2d( _pv.x + _dw, _pv.y - _dd ) }, axis );

            CVec3 _n;

            _n = _n + CVec3::cross( _p4 - _p1, _pv - _p4 );
            _n = _n + CVec3::cross( _pv - _p1, _p2 - _pv );

            _n = _n + CVec3::cross( _pv - _p2, _p6 - _pv );
            _n = _n + CVec3::cross( _p8 - _p4, _pv - _p8 );

            _n = _n + CVec3::cross( _p8 - _pv, _p9 - _p8 );
            _n = _n + CVec3::cross( _p9 - _pv, _p6 - _p9 );

            _n.normalize();

            _normals[q] = _n;
        }

        auto _name = std::string( "perlinPatch:" ) + std::to_string( CMeshBuilder::s_numPerlinPatches++ );

        return new CMesh( _name, _vertices, _normals, _texCoord, _indices );
    }

    CMesh* CMeshBuilder::createHeightField( int nWidthSamples, int nDepthSamples, 
                                            float widthExtent, float depthExtent, 
                                            float centerX, float centerY,
                                            const std::vector< float >& heightData, float heightBase,
                                            const eAxis& axis )
    {
        std::vector< CVec3 > _vertices;
        std::vector< CVec3 > _normals;
        std::vector< CVec2 > _texCoords;
        std::vector< CInd3 > _indices;

        ENGINE_CORE_ASSERT( heightData.size() == nWidthSamples * nDepthSamples, "Mismatch in number of heightmap samples" );

        float _minNegHeight = 0.0f; // catch minimum negative heights
        for ( size_t i = 0; i < nWidthSamples - 1; i++ )
        {
            for ( size_t j = 0; j < nDepthSamples - 1; j++ )
            {
                auto _p0 = _rotateToMatchUpAxis( { widthExtent * ( ( (float)( i + 0 ) ) / nWidthSamples ) - centerX, 
                                                   depthExtent * ( ( (float)( j + 0 ) ) / nDepthSamples ) - centerY, 
                                                   heightData[( i + 0 ) * nDepthSamples + ( j + 0 )] }, 
                                                 axis );

                auto _p1 = _rotateToMatchUpAxis( { widthExtent * ( ( (float)( i + 1 ) ) / nWidthSamples ) - centerX, 
                                                   depthExtent * ( ( (float)( j + 0 ) ) / nDepthSamples ) - centerY, 
                                                   heightData[( i + 1 ) * nDepthSamples + ( j + 0 )] }, 
                                                 axis );

                auto _p2 = _rotateToMatchUpAxis( { widthExtent * ( ( (float)( i + 1 ) ) / nWidthSamples ) - centerX, 
                                                   depthExtent * ( ( (float)( j + 1 ) ) / nDepthSamples ) - centerY, 
                                                   heightData[( i + 1 ) * nDepthSamples + ( j + 1 )] }, 
                                                 axis );

                auto _p3 = _rotateToMatchUpAxis( { widthExtent * ( ( (float)( i + 0 ) ) / nWidthSamples ) - centerX, 
                                                   depthExtent * ( ( (float)( j + 1 ) ) / nDepthSamples ) - centerY, 
                                                   heightData[( i + 0 ) * nDepthSamples + ( j + 1 )] }, 
                                                 axis );

                _indices.push_back( { (GLint) _vertices.size() + 0, (GLint) _vertices.size() + 1, (GLint) _vertices.size() + 2 } );
                _indices.push_back( { (GLint) _vertices.size() + 3, (GLint) _vertices.size() + 4, (GLint) _vertices.size() + 5 } );

                CVec2 _t0( ( (float)( i + 0 ) ) / nWidthSamples, ( (float)( j + 0 ) ) / nDepthSamples );
                CVec2 _t1( ( (float)( i + 0 ) ) / nWidthSamples, ( (float)( j + 1 ) ) / nDepthSamples );
                CVec2 _t2( ( (float)( i + 1 ) ) / nWidthSamples, ( (float)( j + 1 ) ) / nDepthSamples );
                CVec2 _t3( ( (float)( i + 1 ) ) / nWidthSamples, ( (float)( j + 0 ) ) / nDepthSamples );

                _texCoords.push_back( _t0 );
                _texCoords.push_back( _t1 );
                _texCoords.push_back( _t2 );
                _texCoords.push_back( _t0 );
                _texCoords.push_back( _t2 );
                _texCoords.push_back( _t3 );

                _vertices.push_back( _p0 );
                _vertices.push_back( _p1 );
                _vertices.push_back( _p2 );
                _vertices.push_back( _p0 );
                _vertices.push_back( _p2 );
                _vertices.push_back( _p3 );

                auto _nt1 = CVec3::cross( _p1 - _p0, _p2 - _p1 );
                auto _nt2 = CVec3::cross( _p2 - _p0, _p3 - _p2 );

                _normals.push_back( _nt1 );
                _normals.push_back( _nt1 );
                _normals.push_back( _nt1 );

                _normals.push_back( _nt2 );
                _normals.push_back( _nt2 );
                _normals.push_back( _nt2 );

                // store the minimum height for later usage
                _minNegHeight = std::min( _minNegHeight, heightData[( i + 0 ) * nDepthSamples + ( j + 0 )] );
                _minNegHeight = std::min( _minNegHeight, heightData[( i + 1 ) * nDepthSamples + ( j + 0 )] );
                _minNegHeight = std::min( _minNegHeight, heightData[( i + 1 ) * nDepthSamples + ( j + 1 )] );
                _minNegHeight = std::min( _minNegHeight, heightData[( i + 0 ) * nDepthSamples + ( j + 1 )] );
            }
        }

        struct SideInfo
        {
            int i0;
            int j0;
            int i1;
            int j1;
            CVec3 normal;
        };

        std::vector< SideInfo > _sides = { {         0        ,         0        , nWidthSamples - 1,         0        , {  0.0f, -1.0f, 0.0f } },
                                           { nWidthSamples - 1,         0        , nWidthSamples - 1, nDepthSamples - 1, {  1.0f,  0.0f, 0.0f } },
                                           { nWidthSamples - 1, nDepthSamples - 1,         0        , nDepthSamples - 1, {  0.0f,  1.0f, 0.0f } },
                                           {         0        , nDepthSamples - 1,         0        ,         0        , { -1.0f,  0.0f, 0.0f } } };

        for ( size_t s = 0; s < _sides.size(); s++ )
        {
            int i0 = _sides[s].i0;
            int j0 = _sides[s].j0;
            int i1 = _sides[s].i1;
            int j1 = _sides[s].j1;

            std::vector< int > _ii;
            if ( i1 > i0 ) { for ( int i = i0; i <= i1; i++ ) { _ii.push_back( i ); } }
            else           { for ( int i = i0; i >= i1; i-- ) { _ii.push_back( i ); } }

            std::vector< int > _jj;
            if ( j1 > j0 ) { for ( int j = j0; j <= j1; j++ ) { _jj.push_back( j ); } }
            else           { for ( int j = j0; j >= j1; j-- ) { _jj.push_back( j ); } }

            for ( int iind = 0; iind < _ii.size(); iind++ )
            {
                for ( int jind = 0; jind < _jj.size(); jind++ )
                {
                    if ( _ii.size() > 1 && iind >= ( _ii.size() - 1 ) )
                        continue;
                    if ( _jj.size() > 1 && jind >= ( _jj.size() - 1 ) )
                        continue;

                    int ii0 = _ii[iind];
                    int jj0 = _jj[jind];
                    int ii1 = ( _ii.size() > 1 ) ? ( _ii[iind + 1] ) : ( ii0 );
                    int jj1 = ( _jj.size() > 1 ) ? ( _jj[jind + 1] ) : ( jj0 );

                    auto _p0 = _rotateToMatchUpAxis( { -centerX + widthExtent * ( (float)ii0 ) / nWidthSamples, -centerY + depthExtent * ( (float)jj0 ) / nDepthSamples, heightData[ii0 * nDepthSamples + jj0] }, axis );
                    auto _p1 = _rotateToMatchUpAxis( { -centerX + widthExtent * ( (float)ii1 ) / nWidthSamples, -centerY + depthExtent * ( (float)jj1 ) / nDepthSamples, heightData[ii1 * nDepthSamples + jj1] }, axis );
                    auto _p2 = _rotateToMatchUpAxis( { -centerX + widthExtent * ( (float)ii1 ) / nWidthSamples, -centerY + depthExtent * ( (float)jj1 ) / nDepthSamples, _minNegHeight - heightBase }, axis );
                    auto _p3 = _rotateToMatchUpAxis( { -centerX + widthExtent * ( (float)ii0 ) / nWidthSamples, -centerY + depthExtent * ( (float)jj0 ) / nDepthSamples, _minNegHeight - heightBase }, axis );

                    _indices.push_back( { (GLint) _vertices.size() + 0, (GLint) _vertices.size() + 1, (GLint) _vertices.size() + 2 } );
                    _indices.push_back( { (GLint) _vertices.size() + 0, (GLint) _vertices.size() + 2, (GLint) _vertices.size() + 3 } );

                    _vertices.push_back( _p0 );
                    _vertices.push_back( _p1 );
                    _vertices.push_back( _p2 );
                    _vertices.push_back( _p3 );

                    _texCoords.push_back( { -1.0f, -1.0f } ); // let the values be clamped to some user-specified color with GL_CLAMP_TO_BORDER
                    _texCoords.push_back( { -1.0f, -1.0f } ); // let the values be clamped to some user-specified color with GL_CLAMP_TO_BORDER
                    _texCoords.push_back( { -1.0f, -1.0f } ); // let the values be clamped to some user-specified color with GL_CLAMP_TO_BORDER
                    _texCoords.push_back( { -1.0f, -1.0f } ); // let the values be clamped to some user-specified color with GL_CLAMP_TO_BORDER

                    _normals.push_back( _rotateToMatchUpAxis( _sides[s].normal, axis ) );
                    _normals.push_back( _rotateToMatchUpAxis( _sides[s].normal, axis ) );
                    _normals.push_back( _rotateToMatchUpAxis( _sides[s].normal, axis ) );
                    _normals.push_back( _rotateToMatchUpAxis( _sides[s].normal, axis ) );
                }
            }
        }

        // bottom base plane
        {
            auto _p0 = _rotateToMatchUpAxis( { -centerX + widthExtent * 0.0f                            , -centerY + depthExtent * 0.0f                             , _minNegHeight - heightBase }, axis );
            auto _p1 = _rotateToMatchUpAxis( { -centerX + widthExtent * ( 1.0f - 1.0f / nWidthSamples ) , -centerY + depthExtent * 0.0f                             , _minNegHeight - heightBase }, axis );
            auto _p2 = _rotateToMatchUpAxis( { -centerX + widthExtent * ( 1.0f - 1.0f / nWidthSamples ) , -centerY + depthExtent * ( 1.0f - 1.0f / nDepthSamples )  , _minNegHeight - heightBase }, axis );
            auto _p3 = _rotateToMatchUpAxis( { -centerX + widthExtent * 0.0f                            , -centerY + depthExtent * ( 1.0f - 1.0f / nDepthSamples )  , _minNegHeight - heightBase }, axis );

            _indices.push_back( { (GLint) _vertices.size() + 0, (GLint) _vertices.size() + 1, (GLint) _vertices.size() + 2 } );
            _indices.push_back( { (GLint) _vertices.size() + 0, (GLint) _vertices.size() + 2, (GLint) _vertices.size() + 3 } );

            _vertices.push_back( _p0 );
            _vertices.push_back( _p1 );
            _vertices.push_back( _p2 );
            _vertices.push_back( _p3 );

            _texCoords.push_back( { -1.0f, -1.0f } ); // let the values be clamped to some user-specified color with GL_CLAMP_TO_BORDER
            _texCoords.push_back( { -1.0f, -1.0f } ); // let the values be clamped to some user-specified color with GL_CLAMP_TO_BORDER
            _texCoords.push_back( { -1.0f, -1.0f } ); // let the values be clamped to some user-specified color with GL_CLAMP_TO_BORDER
            _texCoords.push_back( { -1.0f, -1.0f } ); // let the values be clamped to some user-specified color with GL_CLAMP_TO_BORDER

            _normals.push_back( _rotateToMatchUpAxis( { 0.0f, 0.0f, -1.0f }, axis ) );
            _normals.push_back( _rotateToMatchUpAxis( { 0.0f, 0.0f, -1.0f }, axis ) );
            _normals.push_back( _rotateToMatchUpAxis( { 0.0f, 0.0f, -1.0f }, axis ) );
            _normals.push_back( _rotateToMatchUpAxis( { 0.0f, 0.0f, -1.0f }, axis ) );
        }

        auto _name = std::string( "heightField:" ) + std::to_string( CMeshBuilder::s_numHeightFields++ );

        return new CMesh( _name, _vertices, _normals, _texCoords, _indices );
    }

    CModel* CMeshBuilder::createModelFromFile( const std::string& filename )
    {
        auto _assimpScenePtr = aiImportFile( filename.c_str(),
                                             aiProcessPreset_TargetRealtime_MaxQuality );

        if ( !_assimpScenePtr )
        {
            return NULL;
        }

        // Create a temporary holder to place the processes data from assimp
        auto _name = std::string( "assimpModel:" ) + std::to_string( CMeshBuilder::s_numAssimpModels++ );
        auto _model = new CModel( _name );
        // recursively copy the data from assimp to our data structure
        _processAssimpNode( _model, _assimpScenePtr->mRootNode, _assimpScenePtr );

        // make sure we release the assimp resources
        // @TODO: Should do this in a assetsModelManager (to avoid repetitions)
        aiReleaseImport( _assimpScenePtr );

        return _model;
    }

    void CMeshBuilder::_processAssimpNode( CModel* modelPtr, 
                                           aiNode* assimpNodePtr, 
                                           const aiScene* assimpScenePtr )
    {
        for ( size_t i = 0; i < assimpNodePtr->mNumMeshes; i++ )
        {
            aiMesh* _assimpMeshPtr = assimpScenePtr->mMeshes[ assimpNodePtr->mMeshes[i] ];
            auto _meshPtr = std::unique_ptr< CMesh >( _processAssimpMesh( modelPtr, _assimpMeshPtr ) );
            modelPtr->addMesh( std::move( _meshPtr ) );
        }

        for ( size_t i = 0; i < assimpNodePtr->mNumChildren; i++ )
        {
            _processAssimpNode( modelPtr,
                                assimpNodePtr->mChildren[i],
                                assimpScenePtr );
        }
    }

    CMesh* CMeshBuilder::_processAssimpMesh( CModel* modelPtr, 
                                             aiMesh* assimpMeshPtr )
    {
        std::vector< CVec3 > _vertices;
        std::vector< CVec3 > _normals;
        std::vector< CVec2 > _texCoords;
        std::vector< CInd3 > _indices;

        for ( size_t i = 0; i < assimpMeshPtr->mNumVertices; i++ )
        {
            _vertices.push_back( CVec3( assimpMeshPtr->mVertices[i].x,
                                        assimpMeshPtr->mVertices[i].y,
                                        assimpMeshPtr->mVertices[i].z ) );

            _normals.push_back( CVec3( assimpMeshPtr->mNormals[i].x,
                                       assimpMeshPtr->mNormals[i].y,
                                       assimpMeshPtr->mNormals[i].z ) );

            if ( assimpMeshPtr->mTextureCoords[0] )
            {
                _texCoords.push_back( CVec2( assimpMeshPtr->mTextureCoords[0][i].x,
                                             assimpMeshPtr->mTextureCoords[0][i].y ) );
            }
            else
            {
                _texCoords.push_back( CVec2( 0.0f, 0.0f ) );
            }
        }

        for ( size_t i = 0; i < assimpMeshPtr->mNumFaces; i++ )
        {
            aiFace _assimpFace = assimpMeshPtr->mFaces[i];
            // grab only in tris. we are assuming it comes this way
            // @TODO: Check this part as may have to support quads
            for ( size_t j = 0; j < _assimpFace.mNumIndices / 3; j++ )
            {
                _indices.push_back( CInd3( _assimpFace.mIndices[ 3 * j + 0 ],
                                           _assimpFace.mIndices[ 3 * j + 1 ],
                                           _assimpFace.mIndices[ 3 * j + 2 ] ) );
            }
        }

        auto _name = modelPtr->name() + std::string( ":submesh:" ) + std::to_string( CMeshBuilder::s_numAssimpSubmeshes++ );

        return new CMesh( _name, _vertices, _normals, _texCoords, _indices );
    }

    /***********************************************************************
    * HELPER FUNCTIONS
    ************************************************************************/

    CVec3 CMeshBuilder::_rotateToMatchUpAxis( const CVec3& vec, const eAxis& axis )
    {
        if ( axis == eAxis::X )
            return { vec.z, vec.x, vec.y };
        else if ( axis == eAxis::Y )
            return { vec.y, vec.z, vec.x };
        else if ( axis == eAxis::Z )
            return { vec.x, vec.y, vec.z };

        ENGINE_CORE_ASSERT( false, "Invalid axis given for conversion, should be either x, y or z" );
        return { vec.x, vec.y, vec.z };
    }

    CVec3 CMeshBuilder::_rotateBackFromUpAxis( const CVec3& vec, const eAxis& axis )
    {
        if ( axis == eAxis::X )
            return { vec.y, vec.z, vec.x };
        else if ( axis == eAxis::Y )
            return { vec.z, vec.x, vec.y };
        else if ( axis == eAxis::Z )
            return { vec.x, vec.y, vec.z };

        ENGINE_CORE_ASSERT( false, "Invalid axis given for conversion, should be either x, y or z" );
        return { vec.x, vec.y, vec.z };
    }

    /* some counters to keep track of the number of meshes created, and some simple name assignemnt */

    int32 CMeshBuilder::s_numPlanes             = 0;
    int32 CMeshBuilder::s_numBoxes              = 0;
    int32 CMeshBuilder::s_numSpheres            = 0;
    int32 CMeshBuilder::s_numEllipsoids         = 0;
    int32 CMeshBuilder::s_numCapsules           = 0;
    int32 CMeshBuilder::s_numCylinders          = 0;
    int32 CMeshBuilder::s_numArrows             = 0;
    int32 CMeshBuilder::s_numAxes               = 0;
    int32 CMeshBuilder::s_numPerlinPatches      = 0;
    int32 CMeshBuilder::s_numHeightFields       = 0;
    int32 CMeshBuilder::s_numAssimpModels       = 0;
    int32 CMeshBuilder::s_numAssimpSubmeshes    = 0;

}
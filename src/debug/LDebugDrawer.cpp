
#include <debug/LDebugDrawer.h>

using namespace std;



namespace engine
{

    LDebugDrawer* LDebugDrawer::_INSTANCE = NULL;

    LDebugDrawer::LDebugDrawer()
    {
        m_shaderLinesRef = ( engine::LShaderDebug3d* ) LShaderManager::getShader( "debug3d" );

        m_linesRenderBufferPositions = vector< LDLinePositions >( DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE );
        m_linesRenderBufferColors = vector< LDLinePositionsColors >( DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE );

        m_linesVAO = new LVertexArray();

        m_linesPositionsVBO = new LVertexBuffer( GL_DYNAMIC_DRAW );
        m_linesPositionsVBO->setData( sizeof( LDLinePositions ) * DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE,
                                      3, ( GLfloat* ) m_linesRenderBufferPositions.data() );

        m_linesColorsVBO = new LVertexBuffer( GL_DYNAMIC_DRAW );
        m_linesColorsVBO->setData( sizeof( LDLinePositionsColors ) * DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE,
                                   3, (GLfloat*)m_linesRenderBufferColors.data() );

        m_linesVAO->addBuffer( m_linesPositionsVBO, 0 );
        m_linesVAO->addBuffer( m_linesColorsVBO, 1 );
    }

    LDebugDrawer* LDebugDrawer::GetInstance()
    {
        if ( LDebugDrawer::_INSTANCE == NULL )
        {
            LDebugDrawer::_INSTANCE = new LDebugDrawer();
        }

        return LDebugDrawer::_INSTANCE;
    }

    void LDebugDrawer::Release()
    {
        if ( LDebugDrawer::_INSTANCE == NULL )
        {
            cout << "LDebugDrawer::release> already released" << endl;
            return;
        }

        delete LDebugDrawer::_INSTANCE;
        LDebugDrawer::_INSTANCE = NULL;
    }

    LDebugDrawer::~LDebugDrawer()
    {
        m_linesPositions.clear();
        m_linesColors.clear();

        delete m_linesVAO;
        m_linesPositionsVBO = NULL;
        m_linesColorsVBO = NULL;
    }

    void LDebugDrawer::setupMatrices( const CMat4& viewMatrix, const CMat4& projectionMatrix )
    {
        m_viewMat = viewMatrix;
        m_projMat = projectionMatrix;
    }

    void LDebugDrawer::render()
    {
        _renderLines();
    }

    void LDebugDrawer::_renderLines()
    {
        glLineWidth( 2.5f );

        for ( size_t q = 0; q < m_linesPositions.size(); q++ )
        {
            m_linesRenderBufferPositions[ q % 1024 ] = m_linesPositions[q];
            m_linesRenderBufferColors[ q % 1024 ] = m_linesColors[q];

            if ( ( q + 1 ) % DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE == 0 )
            {
                _renderLinesBatch( DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE );
            }
        }

        int _remainingCountLines = m_linesPositions.size() % DEBUG_DRAWER_LINES_BUFFER_COUNT_SIZE;

        if ( _remainingCountLines != 0 )
        {
            // If not exactly the buffer size, then there is still something ...
            // to be drawn in the lines buffers

            _renderLinesBatch( _remainingCountLines );
        }

        m_linesPositions.clear();
        m_linesColors.clear();

        glLineWidth( 1.0f );
    }

    void LDebugDrawer::_renderLinesBatch( int count )
    {
        m_linesVAO->bind();

        m_linesPositionsVBO->updateData( count * sizeof( LDLinePositions ), ( GLfloat* ) m_linesRenderBufferPositions.data() );
        m_linesColorsVBO->updateData( count * sizeof( LDLinePositionsColors ), ( GLfloat* ) m_linesRenderBufferColors.data() );

        //m_linesPositionsVBO->setData( count * sizeof( LDLinePositions ), 3, ( GLfloat* ) m_linesRenderBufferPositions.data() );
        //m_linesColorsVBO->setData( count * sizeof( LDLinePositionsColors ), 3, ( GLfloat* ) m_linesRenderBufferColors.data() );

        m_shaderLinesRef->bind();
        m_shaderLinesRef->setViewMatrix( m_viewMat );
        m_shaderLinesRef->setProjectionMatrix( m_projMat );

        glDrawArrays( GL_LINES, 0, count * 2 );

        m_shaderLinesRef->unbind();

        m_linesVAO->unbind();
    }

    void LDebugDrawer::drawLine( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        LDLinePositions _linePos;
        _linePos.vStart = start;
        _linePos.vEnd = end;

        m_linesPositions.push_back( _linePos );

        LDLinePositionsColors _lineCol;
        _lineCol.cStart = color;
        _lineCol.cEnd = color;

        m_linesColors.push_back( _lineCol );
    }

    void LDebugDrawer::drawArrow( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        // Draw core of the arrow
        drawLine( start, end, color );

        // Draw side parts of the arrow
        CVec3 _arrowVec = end - start;
        float _length = _arrowVec.length();

        CVec3 _uf = CVec3::normalize( _arrowVec );
        CVec3 _ur = CVec3::cross( _uf, CVec3( 0, 1, 0 ) );
        CVec3 _uu = CVec3::cross( _ur, _uf );

        float _sidesLength = _length / 10.0f;

        _uf.scale( _sidesLength, _sidesLength, _sidesLength );
        _ur.scale( _sidesLength, _sidesLength, _sidesLength );
        _uu.scale( _sidesLength, _sidesLength, _sidesLength );

        CVec3 _p0 = end + _ur + _uu - _uf;
        CVec3 _p1 = end + _ur - _uu - _uf;
        CVec3 _p2 = end + _ur + _uu - _uf;
        CVec3 _p3 = end + _ur - _uu - _uf;

        drawLine( end, _p0, color );
        drawLine( end, _p1, color );
        drawLine( end, _p2, color );
        drawLine( end, _p3, color );
    }

    void LDebugDrawer::drawClipVolume( const CMat4& clipMatrix, const CVec3& color )
    {
        CMat4 _invClipMatrix = clipMatrix.inverse();

        CVec3 _frustumPointsClipSpace[8] = {
            // near plane
            CVec3( -1.0f, -1.0f, -1.0f ), 
            CVec3( 1.0f, -1.0f, -1.0f ),
            CVec3( 1.0f,  1.0f, -1.0f ),
            CVec3( -1.0f,  1.0f, -1.0f ),
            // far plane
            CVec3( -1.0f, -1.0f, 1.0f ), 
            CVec3( 1.0f, -1.0f, 1.0f ),
            CVec3( 1.0f,  1.0f, 1.0f ),
            CVec3( -1.0f,  1.0f, 1.0f )
        };

        vector< engine::CVec3 > _points3d;
        for ( size_t q = 0; q < 8; q++ )
        {
            CVec4 _pointFrustum = _invClipMatrix * CVec4( _frustumPointsClipSpace[q], 1.0f );
            CVec3 _pointFrustumNormalized = CVec3( _pointFrustum.x / _pointFrustum.w,
                                                   _pointFrustum.y / _pointFrustum.w,
                                                   _pointFrustum.z / _pointFrustum.w );

            _points3d.push_back( _pointFrustumNormalized );
        }

        // near face
        drawLine( _points3d[0], _points3d[1] );
        drawLine( _points3d[1], _points3d[2] );
        drawLine( _points3d[2], _points3d[3] );
        drawLine( _points3d[3], _points3d[0] );

        // far face
        drawLine( _points3d[4], _points3d[5] );
        drawLine( _points3d[5], _points3d[6] );
        drawLine( _points3d[6], _points3d[7] );
        drawLine( _points3d[7], _points3d[4] );

        // side lines
        drawLine( _points3d[0], _points3d[4] );
        drawLine( _points3d[1], _points3d[5] );
        drawLine( _points3d[2], _points3d[6] );
        drawLine( _points3d[3], _points3d[7] );
    }

    void LDebugDrawer::drawTrailPoints( const vector< CVec3 >& trailpoints, const CVec3& color )
    {
        if ( trailpoints.size() < 1 )
        {
            return;
        }

        for ( size_t q = 0; q < trailpoints.size() - 1; q++ )
        {
            drawLine( trailpoints[q], trailpoints[ q + 1 ], color );
        }
    }

    void LDebugDrawer::drawLinesBatch( const vector< CLine >& linesBatch, const CVec3& color )
    {
        if ( linesBatch.size() < 1 )
        {
            return;
        }

        for ( size_t q = 0; q < linesBatch.size(); q++ )
        {
            drawLine( linesBatch[q].start, linesBatch[q].end, color );
        }
    }

    void LDebugDrawer::drawAABB( const CVec3& aabbMin, 
                                 const CVec3& aabbMax, 
                                 const CMat4& aabbWorldTransform, 
                                 const CVec3& color )
    {
        auto _vmin2max = aabbMax - aabbMin;
        auto _dx = CVec3::dot( _vmin2max, aabbWorldTransform.getBasisVectorX() );
        auto _dy = CVec3::dot( _vmin2max, aabbWorldTransform.getBasisVectorY() );
        auto _dz = CVec3::dot( _vmin2max, aabbWorldTransform.getBasisVectorZ() );
        auto _origin = aabbWorldTransform.getPosition();
        auto _sidex = aabbWorldTransform.getBasisVectorX();
        auto _sidey = aabbWorldTransform.getBasisVectorY();
        auto _sidez = aabbWorldTransform.getBasisVectorZ();

        _sidex.scale( 0.5 * _dx, 0.5 * _dx, 0.5 * _dx );
        _sidey.scale( 0.5 * _dy, 0.5 * _dy, 0.5 * _dy );
        _sidez.scale( 0.5 * _dz, 0.5 * _dz, 0.5 * _dz );

        /*
        *      p8 ___________ p7 -> max
        *        /|         /|
        *       / |        / |      z    y
        *      /__|______ /  |      |  /
        *     |p4 |_ _ _ |p3_|      | /
        *     |  /p5     |  / p6    |/_ _ _ x
        *     | /        | / 
        *     |/_________|/ 
        *  p1 -> min      p2
        */

        auto _p1 = _origin - _sidex - _sidey - _sidez;
        auto _p2 = _origin + _sidex - _sidey - _sidez;
        auto _p3 = _origin + _sidex - _sidey + _sidez;
        auto _p4 = _origin - _sidex - _sidey + _sidez;

        auto _p5 = _origin - _sidex + _sidey - _sidez;
        auto _p6 = _origin + _sidex + _sidey - _sidez;
        auto _p7 = _origin + _sidex + _sidey + _sidez;
        auto _p8 = _origin - _sidex + _sidey + _sidez;

        drawLine( _p1, _p2, color ); drawLine( _p2, _p3, color );
        drawLine( _p3, _p4, color ); drawLine( _p4, _p1, color );

        drawLine( _p2, _p6, color ); drawLine( _p6, _p7, color );
        drawLine( _p7, _p3, color ); drawLine( _p3, _p2, color );

        drawLine( _p4, _p3, color ); drawLine( _p3, _p7, color );
        drawLine( _p7, _p8, color ); drawLine( _p8, _p4, color );

        drawLine( _p4, _p8, color ); drawLine( _p8, _p5, color );
        drawLine( _p5, _p1, color ); drawLine( _p1, _p4, color );

        drawLine( _p1, _p2, color ); drawLine( _p2, _p6, color );
        drawLine( _p6, _p5, color ); drawLine( _p5, _p1, color );

        drawLine( _p5, _p6, color ); drawLine( _p6, _p7, color );
        drawLine( _p7, _p8, color ); drawLine( _p8, _p5, color );
    }
}
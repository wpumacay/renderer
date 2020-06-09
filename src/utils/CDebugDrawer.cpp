
#include <utils/CDebugDrawer.h>

namespace engine
{

    CDebugDrawer* CDebugDrawer::s_instance = nullptr;

    CDebugDrawer::CDebugDrawer()
    {
        m_shaderLinesPtr = CShaderManager::GetCachedShader( "debug_drawing_3d" );
        ENGINE_CORE_ASSERT( m_shaderLinesPtr, "Could not load debug_drawing_3d shader required for drawing lines" );

        m_linesRenderBufferPositions = std::vector< CLinePositions >( DEBUG_DRAWER_BATCH_SIZE );
        m_linesRenderBufferColors = std::vector< CLineColors >( DEBUG_DRAWER_BATCH_SIZE );

        auto _linesPositionsVBOlayout = CVertexBufferLayout( { { "position", eElementType::Float3, false } } );
        auto _linesColorsVBOlayout = CVertexBufferLayout( { { "color", eElementType::Float3, false } } );

        const uint32 _linesPositionsVBOsize = sizeof( CLinePositions ) * m_linesRenderBufferPositions.size();
        const uint32 _linesColorsVBOsize = sizeof( CLineColors ) * m_linesRenderBufferColors.size();

        float32* _linesPositionsVBOdata = (float32*) m_linesRenderBufferPositions.data();
        float32* _linesColorsVBOdata = (float32*) m_linesRenderBufferColors.data();

        auto _linesPositionsVBO = std::make_unique< CVertexBuffer >( _linesPositionsVBOlayout, eBufferUsage::DYNAMIC,
                                                                     _linesPositionsVBOsize, _linesPositionsVBOdata );
        auto _linesColorsVBO = std::make_unique< CVertexBuffer >( _linesPositionsVBOlayout, eBufferUsage::DYNAMIC,
                                                                  _linesPositionsVBOsize, _linesColorsVBOdata  );

        // keep some references for local manipulation
        m_linesPositionsVBOref = _linesPositionsVBO.get();
        m_linesColorsVBOref = _linesColorsVBO.get();

        m_linesVAO = std::make_unique< CVertexArray >();
        m_linesVAO->addVertexBuffer( std::move( _linesPositionsVBO ) );
        m_linesVAO->addVertexBuffer( std::move( _linesColorsVBO ) );

        /* setup resources for rendering solid objects */
        m_shaderSolidLightingPtr = CShaderManager::GetCachedShader( "engine_debug_drawing_3d_solid_lighting" );
        ENGINE_CORE_ASSERT( m_shaderSolidLightingPtr, 
                            "Could not load engine_debug_drawing_3d_solid_lighting shader required for drawing solid cubes with lighting" );

        m_shaderSolidNoLightingPtr = CShaderManager::GetCachedShader( "engine_debug_drawing_3d_solid_no_lighting" );
        ENGINE_CORE_ASSERT( m_shaderSolidNoLightingPtr, 
                            "Could not load engine_debug_drawing_3d_solid_no_lighting shader required for drawing solid cubes without lighting" );

        // create resources to handle instanced rendering of solid primitives
        _createInstancedBuffers( DD_PRIMITIVE_BOX );
        _createInstancedBuffers( DD_PRIMITIVE_SPHERE );
        _createInstancedBuffers( DD_PRIMITIVE_CYLINDER_X );
        _createInstancedBuffers( DD_PRIMITIVE_CYLINDER_Y );
        _createInstancedBuffers( DD_PRIMITIVE_CYLINDER_Z );
        _createInstancedBuffers( DD_PRIMITIVE_CAPSULE_X );
        _createInstancedBuffers( DD_PRIMITIVE_CAPSULE_Y );
        _createInstancedBuffers( DD_PRIMITIVE_CAPSULE_Z );
        _createInstancedBuffers( DD_PRIMITIVE_ARROW_X );
        _createInstancedBuffers( DD_PRIMITIVE_ARROW_Y );
        _createInstancedBuffers( DD_PRIMITIVE_ARROW_Z );

        m_active = true;
    }

    void CDebugDrawer::Init()
    {
        if ( CDebugDrawer::s_instance )
        {
            ENGINE_CORE_WARN( "Attempting to initialize debug-drawer twice" );
            return;
        }

        CDebugDrawer::s_instance = new CDebugDrawer();
    }

    void CDebugDrawer::Release()
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Tried to release debug-drawer more than once" );

        delete CDebugDrawer::s_instance;
        CDebugDrawer::s_instance = nullptr;
    }

    void CDebugDrawer::SetEnabled( bool enabled )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Tried to release debug-drawer more than once" );

        CDebugDrawer::s_instance->_setEnabled( enabled );
    }

    void CDebugDrawer::Render( CICamera* camera )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_render( camera );
    }

    void CDebugDrawer::Render( CICamera* camera, CILight* light )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_render( camera, light );
    }

    void CDebugDrawer::DrawLine( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawLine( start, end, color );
    }

    void CDebugDrawer::DrawArrow( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawArrow( start, end, color );
    }

    void CDebugDrawer::DrawBox( const CVec3& size, const CMat4& transform, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawBox( size, transform, color );
    }

    void CDebugDrawer::DrawSphere( float32 radius, const CMat4& transform )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawSphere( radius, transform, CVec3( 0.0f, 0.0f, 1.0f ), CVec3( 1.0f, 0.0f, 0.0f ), CVec3( 0.0f, 1.0f, 0.0f ) );
    }

    void CDebugDrawer::DrawSphere( float32 radius, const CMat4& transform, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawSphere( radius, transform, color, color, color );
    }

    void CDebugDrawer::DrawClipVolume( const CMat4& clipMatrix, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawClipVolume( clipMatrix, color );
    }

    void CDebugDrawer::DrawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawTrailPoints( trailpoints, color );
    }

    void CDebugDrawer::DrawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawLinesBatch( linesBatch, color );
    }

    void CDebugDrawer::DrawAABB( const CVec3& aabbMin, const CVec3& aabbMax, const CMat4& worldTransform, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawAABB( aabbMin, aabbMax, worldTransform, color );
    }

    void CDebugDrawer::DrawNormals( CMesh* meshPtr, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawNormals( meshPtr, color );
    }

    void CDebugDrawer::DrawAxes( const CVec3& xAxis, const CVec3& yAxis, const CVec3& zAxis, const CVec3& position, float32 size )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawAxes( xAxis, yAxis, zAxis, position, size );
    }

    void CDebugDrawer::DrawFrame( const CMat4& frame, float32 size )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawFrame( frame, size );
    }

    void CDebugDrawer::DrawPlane( const CPlane& plane, const CVec2& size, const CVec3& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawPlane( plane, size, color );
    }

    void CDebugDrawer::DrawSolidBox( const CVec3& size, const CMat4& transform, const CVec4& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawSolidBox( size, transform, color );
    }

    void CDebugDrawer::DrawSolidSphere( float32 radius, const CMat4& transform, const CVec4& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawSolidSphere( radius, transform, color );
    }

    void CDebugDrawer::DrawSolidCylinder( float32 radius, float32 height, const eAxis& axis, const CMat4& transform, const CVec4& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawSolidCylinder( radius, height, axis, transform, color );
    }

    void CDebugDrawer::DrawSolidCapsule( float32 radius, float32 height, const eAxis& axis, const CMat4& transform, const CVec4& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawSolidCapsule( radius, height, axis, transform, color );
    }

    void CDebugDrawer::DrawSolidArrow( float32 length, const eAxis& axis, const CMat4& transform, const CVec4& color )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawSolidArrow( length, axis, transform, color );
    }

    void CDebugDrawer::DrawSolidAxes( float32 length, const CMat4& transform, float32 alpha )
    {
        ENGINE_CORE_ASSERT( CDebugDrawer::s_instance, "Must initialize debug-drawer before using it" );

        CDebugDrawer::s_instance->_drawSolidAxes( length, transform, alpha );
    }


    CDebugDrawer::~CDebugDrawer()
    {
        m_linesPositions.clear();
        m_linesRenderBufferPositions.clear();

        m_linesColors.clear();
        m_linesRenderBufferColors.clear();

        m_linesVAO = nullptr;
        m_linesPositionsVBOref = nullptr;
        m_linesColorsVBOref = nullptr;
    }

    void CDebugDrawer::_setEnabled( bool enabled )
    {
        m_active = enabled;
    }

    void CDebugDrawer::_createInstancedBuffers( int primitive )
    {
        // get the vertices|normals|indices required for this primitive
        auto _vertices = std::vector< CVec3 >();
        auto _normals = std::vector< CVec3 >();
        auto _indices = std::vector< CInd3 >();

        // create dummy meshes to grab the geometry (vertices, ...)
        std::unique_ptr<CMesh> _dummyMesh;
        if ( primitive == DD_PRIMITIVE_BOX )
            _dummyMesh = CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
        else if ( primitive == DD_PRIMITIVE_SPHERE )
            _dummyMesh = CMeshBuilder::createSphere( 1.0f );
        else if ( primitive == DD_PRIMITIVE_CYLINDER_X )
            _dummyMesh = CMeshBuilder::createCylinder( 1.0f, 1.0f, eAxis::X );
        else if ( primitive == DD_PRIMITIVE_CYLINDER_Y )
            _dummyMesh = CMeshBuilder::createCylinder( 1.0f, 1.0f, eAxis::Y );
        else if ( primitive == DD_PRIMITIVE_CYLINDER_Z )
            _dummyMesh = CMeshBuilder::createCylinder( 1.0f, 1.0f, eAxis::Z );
        else if ( primitive == DD_PRIMITIVE_CAPSULE_X )
            _dummyMesh = CMeshBuilder::createCapsule( 1.0f, 1.0f, eAxis::X );
        else if ( primitive == DD_PRIMITIVE_CAPSULE_Y )
            _dummyMesh = CMeshBuilder::createCapsule( 1.0f, 1.0f, eAxis::Y );
        else if ( primitive == DD_PRIMITIVE_CAPSULE_Z )
            _dummyMesh = CMeshBuilder::createCapsule( 1.0f, 1.0f, eAxis::Z );
        else if ( primitive == DD_PRIMITIVE_ARROW_X )
            _dummyMesh = CMeshBuilder::createArrow( 1.0f, eAxis::X );
        else if ( primitive == DD_PRIMITIVE_ARROW_Y )
            _dummyMesh = CMeshBuilder::createArrow( 1.0f, eAxis::Y );
        else if ( primitive == DD_PRIMITIVE_ARROW_Z )
            _dummyMesh = CMeshBuilder::createArrow( 1.0f, eAxis::Z );
        else
            ENGINE_CORE_ASSERT( false, "Invalid primitive id given while constructing instanced buffers" );

        // copy vertex data from the primitive
        _vertices = _dummyMesh->vertices();
        _normals = _dummyMesh->normals();
        _indices = _dummyMesh->indices();
        _dummyMesh = nullptr;

        // Create the VBOs that hold fixed data of the shape and normals of the primitives (these 
        // don't change during updates, unlike their instanced counterparts)

        auto _vboPositionsLayout = CVertexBufferLayout( { { "position", eElementType::Float3, false } } );
        auto _vboNormalsLayout = CVertexBufferLayout( { { "normal", eElementType::Float3, true } } );

        uint32 _vboPositionsSize = sizeof( CVec3 ) * _vertices.size();
        uint32 _vboNormalsSize = sizeof( CVec3 ) * _normals.size();

        float32* _vboPositionsData = (float32*) _vertices.data();
        float32* _vboNormalsData = (float32*) _normals.data();

        auto _vboPositions = std::make_unique< CVertexBuffer>( _vboPositionsLayout, eBufferUsage::STATIC,
                                                               _vboPositionsSize, _vboPositionsData );

        auto _vboNormals = std::make_unique<CVertexBuffer>( _vboNormalsLayout, eBufferUsage::STATIC,
                                                            _vboNormalsSize, _vboNormalsData );
        //******************************************************************************************

        // Create the EBO that holds fixed data of the shape (faces) of the primitives
        auto _ibo = std::make_unique<CIndexBuffer>( eBufferUsage::STATIC, 3 * _indices.size(), (uint32*) _indices.data() );
        //******************************************************************************************

        // Create the VBOs that hold dynamic data of the colors, world-transforms and normals-
        // transforms (these change during updates by placing new data into the buffers)

        auto _vboInstancesColorsLayout = CVertexBufferLayout( { { "color", eElementType::Float4, false } } );
        auto _vboInstancesModelMatsLayout = CVertexBufferLayout( { { "modelMatrix-col0", eElementType::Float4, false },
                                                                   { "modelMatrix-col1", eElementType::Float4, false },
                                                                   { "modelMatrix-col2", eElementType::Float4, false },
                                                                   { "modelMatrix-col3", eElementType::Float4, false } } );
        auto _vboInstancesNormalMatsLayout = CVertexBufferLayout( { { "normalMatrix-col0", eElementType::Float4, false },
                                                                    { "normalMatrix-col1", eElementType::Float4, false },
                                                                    { "normalMatrix-col2", eElementType::Float4, false },
                                                                    { "normalMatrix-col3", eElementType::Float4, false } } );

        uint32 _vboInstancesColorsSize = DEBUG_DRAWER_BATCH_SIZE * engine::sizeOfElement( eElementType::Float4 );
        uint32 _vboInstancesModelMatsSize = DEBUG_DRAWER_BATCH_SIZE * 4 * engine::sizeOfElement( eElementType::Float4 );
        uint32 _vboInstancesNormalMatsSize = DEBUG_DRAWER_BATCH_SIZE * 4 * engine::sizeOfElement( eElementType::Float4 );

        auto _vboInstancesColors = std::make_unique<CVertexBuffer>( _vboInstancesColorsLayout, eBufferUsage::DYNAMIC,
                                                                    _vboInstancesColorsSize, (float32*) NULL );

        auto _vboInstancesModelMats = std::make_unique<CVertexBuffer>( _vboInstancesModelMatsLayout, eBufferUsage::DYNAMIC,
                                                                       _vboInstancesModelMatsSize, (float32*) NULL );

        auto _vboInstancesNormalMats = std::make_unique<CVertexBuffer>( _vboInstancesNormalMatsLayout, eBufferUsage::DYNAMIC,
                                                                        _vboInstancesNormalMatsSize, (float32*) NULL );
        //******************************************************************************************

        // keep some pointer references to use locally
        auto _vboPositionsRef = _vboPositions.get();
        auto _vboNormalsRef = _vboNormals.get();
        auto _vboInstancesColorsRef = _vboInstancesColors.get();
        auto _vboInstancesModelMatsRef = _vboInstancesModelMats.get();
        auto _vboInstancesNormalMatsRef = _vboInstancesNormalMats.get();
        auto _iboRef = _ibo.get();

        auto _vao = std::make_unique<CVertexArray>();
        _vao->addVertexBuffer( std::move( _vboPositions ) );
        _vao->addVertexBuffer( std::move( _vboNormals ) );
        _vao->addVertexBuffer( std::move( _vboInstancesColors ), true );
        _vao->addVertexBuffer( std::move( _vboInstancesModelMats ), true );
        _vao->addVertexBuffer( std::move( _vboInstancesNormalMats ), true );
        _vao->setIndexBuffer( std::move( _ibo ) );

        m_primitivesVAO[primitive] = std::move( _vao );

        m_primitivesIBOrefs[primitive] = _iboRef;
        m_primitivesVBOpositionsRefs[primitive] = _vboPositionsRef;
        m_primitivesVBOnormalsRefs[primitive] = _vboNormalsRef;
        m_primitivesVBOinstancesColorsRefs[primitive] = _vboInstancesColorsRef;
        m_primitivesVBOinstancesModelMatsRefs[primitive] = _vboInstancesModelMatsRef;
        m_primitivesVBOinstancesNormalMatsRefs[primitive] = _vboInstancesNormalMatsRef;
    }

    void CDebugDrawer::_render( CICamera* camera )
    {
        if ( !m_active )
            return;

        _renderSolidPrimitives( camera );
        _renderLines( camera );
    }

    void CDebugDrawer::_render( CICamera* camera, CILight* light )
    {
        if ( !m_active )
            return;

        _renderSolidPrimitives( camera, light );
        _renderLines( camera );
    }

    void CDebugDrawer::_renderLines( CICamera* camera )
    {
        if ( m_linesPositions.size() < 1 )
            return;

        /* setup render state */
        m_shaderLinesPtr->bind();
        m_shaderLinesPtr->setMat4( "u_tView", camera->mat_view() );
        m_shaderLinesPtr->setMat4( "u_tProj", camera->mat_proj() );
        glLineWidth( 2.0f );

        // render lines in batches *****************************************************************
        for ( size_t q = 0; q < m_linesPositions.size(); q++ )
        {
            m_linesRenderBufferPositions[ q % DEBUG_DRAWER_BATCH_SIZE ] = m_linesPositions[q];
            m_linesRenderBufferColors[ q % DEBUG_DRAWER_BATCH_SIZE ] = m_linesColors[q];

            if ( ( q + 1 ) % DEBUG_DRAWER_BATCH_SIZE == 0 )
                _renderBatchOfLines( DEBUG_DRAWER_BATCH_SIZE );
        }

        int _remainingCountLines = m_linesPositions.size() % DEBUG_DRAWER_BATCH_SIZE;

        // Draw remaining lines (the ones that didn't get a batch)
        if ( _remainingCountLines != 0 )
            _renderBatchOfLines( _remainingCountLines );

        // clear our containers for later usage
        m_linesPositions.clear();
        m_linesColors.clear();
        //******************************************************************************************

        glLineWidth( 1.0f );
        m_shaderLinesPtr->unbind();
    }

    void CDebugDrawer::_renderBatchOfLines( int numLines )
    {
        m_linesVAO->bind();

        m_linesPositionsVBOref->updateData( numLines * sizeof( CLinePositions ), ( float32* ) m_linesRenderBufferPositions.data() );
        m_linesColorsVBOref->updateData( numLines * sizeof( CLineColors ), ( float32* ) m_linesRenderBufferColors.data() );

        glDrawArrays( GL_LINES, 0, numLines * 2 );

        m_linesVAO->unbind();
    }

    void CDebugDrawer::_renderSolidPrimitives( CICamera* camera )
    {
        int _numPrimitives = 0;
        for ( size_t prim_id = 0; prim_id < DEBUG_DRAWER_PRIMITIVE_TYPES; prim_id++ )
        {
            _numPrimitives += m_primitivesOpaqueColors[prim_id].size();
            _numPrimitives += m_primitivesTransparentColors[prim_id].size();
        }

        if ( _numPrimitives < 1 )
            return;

        m_shaderSolidNoLightingPtr->bind();
        m_shaderSolidNoLightingPtr->setMat4( "u_viewProjMatrix", camera->mat_proj() * camera->mat_view() );
        glEnable( GL_CULL_FACE );

        // render opaque primitives first
        for ( size_t prim_id = 0; prim_id < DEBUG_DRAWER_PRIMITIVE_TYPES; prim_id++ )
        {
            // render primitives in batches ************************************************************
            for ( size_t q = 0; q < m_primitivesOpaqueColors[prim_id].size(); q++ )
            {
                m_renderBufferPrimitivesColors[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesOpaqueColors[prim_id][q];
                m_renderBufferPrimitivesModelMats[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesOpaqueModelMats[prim_id][q];
                m_renderBufferPrimitivesNormalMats[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesOpaqueNormalMats[prim_id][q];

                if ( ( q + 1 ) % DEBUG_DRAWER_BATCH_SIZE == 0 )
                    _renderBatchOfSolidPrimitives( prim_id, DEBUG_DRAWER_BATCH_SIZE, true );
            }

            int _remainingCountPrimitives = m_primitivesOpaqueColors[prim_id].size() % DEBUG_DRAWER_BATCH_SIZE;

            // Draw remaining boxes (the ones that didn't get a batch)
            if ( _remainingCountPrimitives != 0 )
                _renderBatchOfSolidPrimitives( prim_id, _remainingCountPrimitives, true );

            // clear our containers for later usage
            m_primitivesOpaqueColors[prim_id].clear();
            m_primitivesOpaqueModelMats[prim_id].clear();
            m_primitivesOpaqueNormalMats[prim_id].clear();
            //******************************************************************************************
        }

        // render transparent primitives (without depth-writing, not perfect, I know :( )
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glDepthMask( GL_FALSE );

        for ( size_t prim_id = 0; prim_id < DEBUG_DRAWER_PRIMITIVE_TYPES; prim_id++ )
        {
            // render primitives in batches ************************************************************
            for ( size_t q = 0; q < m_primitivesTransparentColors[prim_id].size(); q++ )
            {
                m_renderBufferPrimitivesColors[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesTransparentColors[prim_id][q];
                m_renderBufferPrimitivesModelMats[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesTransparentModelMats[prim_id][q];
                m_renderBufferPrimitivesNormalMats[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesTransparentNormalMats[prim_id][q];

                if ( ( q + 1 ) % DEBUG_DRAWER_BATCH_SIZE == 0 )
                    _renderBatchOfSolidPrimitives( prim_id, DEBUG_DRAWER_BATCH_SIZE, true );
            }

            int _remainingCountPrimitives = m_primitivesTransparentColors[prim_id].size() % DEBUG_DRAWER_BATCH_SIZE;

            // Draw remaining boxes (the ones that didn't get a batch)
            if ( _remainingCountPrimitives != 0 )
                _renderBatchOfSolidPrimitives( prim_id, _remainingCountPrimitives, true );

            // clear our containers for later usage
            m_primitivesTransparentColors[prim_id].clear();
            m_primitivesTransparentModelMats[prim_id].clear();
            m_primitivesTransparentNormalMats[prim_id].clear();
            //******************************************************************************************
        }

        glDepthMask( GL_TRUE );
        glDisable( GL_BLEND );

        glDisable( GL_CULL_FACE );
        m_shaderSolidNoLightingPtr->unbind();
    }

    void CDebugDrawer::_renderSolidPrimitives( CICamera* camera, CILight* light )
    {
        int _numPrimitives = 0;
        for ( size_t prim_id = 0; prim_id < DEBUG_DRAWER_PRIMITIVE_TYPES; prim_id++ )
        {
            _numPrimitives += m_primitivesOpaqueColors[prim_id].size();
            _numPrimitives += m_primitivesTransparentColors[prim_id].size();
        }

        if ( _numPrimitives < 1 )
            return;
        
        m_shaderSolidLightingPtr->bind();
        m_shaderSolidLightingPtr->setMat4( "u_viewProjMatrix", camera->mat_proj() * camera->mat_view() );
        // disable all light-types
        m_shaderSolidLightingPtr->setInt( "u_directionalLight.enabled", 0 );
        m_shaderSolidLightingPtr->setInt( "u_pointLight.enabled", 0 );
        m_shaderSolidLightingPtr->setInt( "u_spotLight.enabled", 0 );

        // setup appropriate light uniforms given the light-type
        if ( light->type() == eLightType::DIRECTIONAL )
        {
            m_shaderSolidLightingPtr->setInt( "u_directionalLight.enabled", 1 );
            m_shaderSolidLightingPtr->setVec3( "u_directionalLight.ambient", light->ambient );
            m_shaderSolidLightingPtr->setVec3( "u_directionalLight.diffuse", light->diffuse );
            //// m_shaderSolidLightingPtr->setVec3( "u_directionalLight.specular", light->specular );
            m_shaderSolidLightingPtr->setFloat( "u_directionalLight.intensity", light->intensity );
            m_shaderSolidLightingPtr->setVec3( "u_directionalLight.direction", light->direction );
        }
        else if ( light->type() == eLightType::POINT )
        {
            m_shaderSolidLightingPtr->setInt( "u_pointLight.enabled", 1 );
            m_shaderSolidLightingPtr->setVec3( "u_pointLight.ambient", light->ambient );
            m_shaderSolidLightingPtr->setVec3( "u_pointLight.diffuse", light->diffuse );
            //// m_shaderSolidLightingPtr->setVec3( "u_pointLight.specular", light->specular );
            m_shaderSolidLightingPtr->setFloat( "u_pointLight.intensity", light->intensity );
            m_shaderSolidLightingPtr->setVec3( "u_pointLight.position", light->position );
            m_shaderSolidLightingPtr->setFloat( "u_pointLight.attnk0", light->atnConstant );
            m_shaderSolidLightingPtr->setFloat( "u_pointLight.attnk1", light->atnLinear );
            m_shaderSolidLightingPtr->setFloat( "u_pointLight.attnk2", light->atnQuadratic );
        }
        else if ( light->type() == eLightType::SPOT )
        {
            m_shaderSolidLightingPtr->setInt( "u_spotLight.enabled", 1 );
            m_shaderSolidLightingPtr->setVec3( "u_spotLight.ambient", light->ambient );
            m_shaderSolidLightingPtr->setVec3( "u_spotLight.diffuse", light->diffuse );
            //// m_shaderSolidLightingPtr->setVec3( "u_spotLight.specular", light->specular );
            m_shaderSolidLightingPtr->setFloat( "u_spotLight.intensity", light->intensity );
            m_shaderSolidLightingPtr->setVec3( "u_spotLight.position", light->position );
            m_shaderSolidLightingPtr->setFloat( "u_spotLight.attnk0", light->atnConstant );
            m_shaderSolidLightingPtr->setFloat( "u_spotLight.attnk1", light->atnLinear );
            m_shaderSolidLightingPtr->setFloat( "u_spotLight.attnk2", light->atnQuadratic );
            m_shaderSolidLightingPtr->setVec3( "u_spotLight.direction", light->direction );
            m_shaderSolidLightingPtr->setFloat( "u_spotLight.innerCutoffCos", std::cos( light->innerCutoff ) );
            m_shaderSolidLightingPtr->setFloat( "u_spotLight.outerCutoffCos", std::cos( light->outerCutoff ) );
        }
        glEnable( GL_CULL_FACE );

        // render opaque primitives first
        for ( size_t prim_id = 0; prim_id < DEBUG_DRAWER_PRIMITIVE_TYPES; prim_id++ )
        {
            // render primitives in batches ************************************************************
            for ( size_t q = 0; q < m_primitivesOpaqueColors[prim_id].size(); q++ )
            {
                m_renderBufferPrimitivesColors[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesOpaqueColors[prim_id][q];
                m_renderBufferPrimitivesModelMats[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesOpaqueModelMats[prim_id][q];
                m_renderBufferPrimitivesNormalMats[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesOpaqueNormalMats[prim_id][q];

                if ( ( q + 1 ) % DEBUG_DRAWER_BATCH_SIZE == 0 )
                    _renderBatchOfSolidPrimitives( prim_id, DEBUG_DRAWER_BATCH_SIZE, true );
            }

            int _remainingCountPrimitives = m_primitivesOpaqueColors[prim_id].size() % DEBUG_DRAWER_BATCH_SIZE;

            // Draw remaining boxes (the ones that didn't get a batch)
            if ( _remainingCountPrimitives != 0 )
                _renderBatchOfSolidPrimitives( prim_id, _remainingCountPrimitives, true );

            // clear our containers for later usage
            m_primitivesOpaqueColors[prim_id].clear();
            m_primitivesOpaqueModelMats[prim_id].clear();
            m_primitivesOpaqueNormalMats[prim_id].clear();
            //******************************************************************************************
        }

        // render transparent primitives (without depth-writing, not perfect, I know :( )
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glDepthMask( GL_FALSE );

        for ( size_t prim_id = 0; prim_id < DEBUG_DRAWER_PRIMITIVE_TYPES; prim_id++ )
        {
            // render primitives in batches ************************************************************
            for ( size_t q = 0; q < m_primitivesTransparentColors[prim_id].size(); q++ )
            {
                m_renderBufferPrimitivesColors[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesTransparentColors[prim_id][q];
                m_renderBufferPrimitivesModelMats[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesTransparentModelMats[prim_id][q];
                m_renderBufferPrimitivesNormalMats[prim_id][ q % DEBUG_DRAWER_BATCH_SIZE ] = m_primitivesTransparentNormalMats[prim_id][q];

                if ( ( q + 1 ) % DEBUG_DRAWER_BATCH_SIZE == 0 )
                    _renderBatchOfSolidPrimitives( prim_id, DEBUG_DRAWER_BATCH_SIZE, true );
            }

            int _remainingCountPrimitives = m_primitivesTransparentColors[prim_id].size() % DEBUG_DRAWER_BATCH_SIZE;

            // Draw remaining boxes (the ones that didn't get a batch)
            if ( _remainingCountPrimitives != 0 )
                _renderBatchOfSolidPrimitives( prim_id, _remainingCountPrimitives, true );

            // clear our containers for later usage
            m_primitivesTransparentColors[prim_id].clear();
            m_primitivesTransparentModelMats[prim_id].clear();
            m_primitivesTransparentNormalMats[prim_id].clear();
            //******************************************************************************************
        }

        glDepthMask( GL_TRUE );
        glDisable( GL_BLEND );

        glDisable( GL_CULL_FACE );
        m_shaderSolidLightingPtr->unbind();
    }

    void CDebugDrawer::_renderBatchOfSolidPrimitives( int primitive, int numPrimitives, bool updateNormals )
    {
        //// std::cout << "primitive: " << primitive << std::endl;
        ENGINE_CORE_ASSERT( ( primitive >= 0 ) && ( primitive < DEBUG_DRAWER_PRIMITIVE_TYPES ), "Invalid primitive id given for rendering" );

        m_primitivesVAO[primitive]->bind();

        m_primitivesVBOinstancesColorsRefs[primitive]->updateData( numPrimitives * sizeof ( CVec4 ), (float32*) m_renderBufferPrimitivesColors[primitive].data() );
        m_primitivesVBOinstancesModelMatsRefs[primitive]->updateData( numPrimitives * sizeof( CMat4 ), (float32*) m_renderBufferPrimitivesModelMats[primitive].data() );
        m_primitivesVBOinstancesNormalMatsRefs[primitive]->updateData( numPrimitives * sizeof( CMat4 ), (float32*) m_renderBufferPrimitivesNormalMats[primitive].data() );

        glDrawElementsInstanced( GL_TRIANGLES, m_primitivesIBOrefs[primitive]->count(), GL_UNSIGNED_INT, 0, numPrimitives );

        m_primitivesVAO[primitive]->unbind();
    }

    void CDebugDrawer::_drawLine( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        CLinePositions _linePos;
        _linePos.vStart = start;
        _linePos.vEnd = end;

        m_linesPositions.push_back( _linePos );

        CLineColors _lineCol;
        _lineCol.cStart = color;
        _lineCol.cEnd = color;

        m_linesColors.push_back( _lineCol );
    }

    void CDebugDrawer::_drawArrow( const CVec3& start, const CVec3& end, const CVec3& color )
    {
        // Draw core of the arrow
        _drawLine( start, end, color );

        // Draw side parts of the arrow
        CVec3 _arrowVec = end - start;
        float _length = _arrowVec.length();

        CVec3 _uf = _arrowVec.normalized();
        CVec3 _ur = tinymath::cross( _uf, CVec3( 0, 1, 0 ) );
        CVec3 _uu = tinymath::cross( _ur, _uf );

        float _sidesLength = _length / 10.0f;

        _uf.scale( CVec3( _sidesLength, _sidesLength, _sidesLength ) );
        _ur.scale( CVec3( _sidesLength, _sidesLength, _sidesLength ) );
        _uu.scale( CVec3( _sidesLength, _sidesLength, _sidesLength ) );

        CVec3 _p0 = end + _ur + _uu - _uf;
        CVec3 _p1 = end + _ur - _uu - _uf;
        CVec3 _p2 = end + _ur + _uu - _uf;
        CVec3 _p3 = end + _ur - _uu - _uf;

        _drawLine( end, _p0, color );
        _drawLine( end, _p1, color );
        _drawLine( end, _p2, color );
        _drawLine( end, _p3, color );
    }

    void CDebugDrawer::_drawBox( const CVec3& size, const CMat4& transform, const CVec3& color )
    {
        auto _cornersInWorld = engine::computeBoxCorners( { size, transform } );

        _drawLine( _cornersInWorld[0], _cornersInWorld[1], color ); _drawLine( _cornersInWorld[1], _cornersInWorld[2], color );
        _drawLine( _cornersInWorld[2], _cornersInWorld[3], color ); _drawLine( _cornersInWorld[3], _cornersInWorld[0], color );

        _drawLine( _cornersInWorld[1], _cornersInWorld[5], color ); _drawLine( _cornersInWorld[5], _cornersInWorld[6], color );
        _drawLine( _cornersInWorld[6], _cornersInWorld[2], color ); _drawLine( _cornersInWorld[2], _cornersInWorld[1], color );

        _drawLine( _cornersInWorld[5], _cornersInWorld[6], color ); _drawLine( _cornersInWorld[6], _cornersInWorld[7], color );
        _drawLine( _cornersInWorld[7], _cornersInWorld[4], color ); _drawLine( _cornersInWorld[4], _cornersInWorld[5], color );

        _drawLine( _cornersInWorld[4], _cornersInWorld[7], color ); _drawLine( _cornersInWorld[7], _cornersInWorld[3], color );
        _drawLine( _cornersInWorld[3], _cornersInWorld[0], color ); _drawLine( _cornersInWorld[0], _cornersInWorld[4], color );

        _drawLine( _cornersInWorld[2], _cornersInWorld[6], color ); _drawLine( _cornersInWorld[6], _cornersInWorld[7], color );
        _drawLine( _cornersInWorld[7], _cornersInWorld[3], color ); _drawLine( _cornersInWorld[3], _cornersInWorld[2], color );

        _drawLine( _cornersInWorld[1], _cornersInWorld[5], color ); _drawLine( _cornersInWorld[5], _cornersInWorld[4], color );
        _drawLine( _cornersInWorld[4], _cornersInWorld[0], color ); _drawLine( _cornersInWorld[0], _cornersInWorld[1], color );
    }

    void CDebugDrawer::_drawSphere( float32 radius, const CMat4& transform, const CVec3& colorXY, const CVec3& colorYZ, const CVec3& colorXZ )
    {
        for ( size_t i = 0; i < DEBUG_DRAWER_SPHERE_DIVISIONS; i++ )
        {
            float32 _dtheta = ( 2.0f * ENGINE_PI / DEBUG_DRAWER_SPHERE_DIVISIONS );
            // x-y plane
            CVec3 _pxy0 = CVec3( radius * std::cos( i * _dtheta ), radius * std::sin( i * _dtheta ), 0.0f );
            CVec3 _pxy1 = CVec3( radius * std::cos( ( i + 1 ) * _dtheta ), radius * std::sin( ( i + 1 ) * _dtheta ), 0.0f );
            // y-z plane
            CVec3 _pyz0 = CVec3( 0.0f, radius * std::cos( i * _dtheta ), radius * std::sin( i * _dtheta ) );
            CVec3 _pyz1 = CVec3( 0.0f, radius * std::cos( ( i + 1 ) * _dtheta ), radius * std::sin( ( i + 1 ) * _dtheta ) );
            // x-z plane
            CVec3 _pxz0 = CVec3( radius * std::sin( i * _dtheta ), 0.0f, radius * std::cos( i * _dtheta ) );
            CVec3 _pxz1 = CVec3( radius * std::sin( ( i + 1 ) * _dtheta ), 0.0f, radius * std::cos( ( i + 1 ) * _dtheta ) );

            // draw the steps using the points after applying world-space transformation
            _drawLine( CVec3( transform * CVec4( _pxy0, 1.0f ) ), 
                       CVec3( transform * CVec4( _pxy1, 1.0f ) ), colorXY );
            _drawLine( CVec3( transform * CVec4( _pyz0, 1.0f ) ), 
                       CVec3( transform * CVec4( _pyz1, 1.0f ) ), colorYZ );
            _drawLine( CVec3( transform * CVec4( _pxz0, 1.0f ) ), 
                       CVec3( transform * CVec4( _pxz1, 1.0f ) ), colorXZ );
        }
    }

    void CDebugDrawer::_drawClipVolume( const CMat4& clipMatrix, const CVec3& color )
    {
        CMat4 _invClipMatrix = tinymath::inverse( clipMatrix );

        CVec3 _frustumPointsClipSpace[8] = {
            /*      near plane      */
            CVec3( -1.0f, -1.0f, -1.0f ), 
            CVec3( 1.0f, -1.0f, -1.0f ),
            CVec3( 1.0f,  1.0f, -1.0f ),
            CVec3( -1.0f,  1.0f, -1.0f ),
            /*      far plane       */
            CVec3( -1.0f, -1.0f, 1.0f ), 
            CVec3( 1.0f, -1.0f, 1.0f ),
            CVec3( 1.0f,  1.0f, 1.0f ),
            CVec3( -1.0f,  1.0f, 1.0f )
        };

        std::vector< engine::CVec3 > _points3d;
        for ( size_t q = 0; q < 8; q++ )
        {
            CVec4 _pointFrustum = _invClipMatrix * CVec4( _frustumPointsClipSpace[q], 1.0f );
            CVec3 _pointFrustumNormalized = CVec3( _pointFrustum.x() / _pointFrustum.w(),
                                                   _pointFrustum.y() / _pointFrustum.w(),
                                                   _pointFrustum.z() / _pointFrustum.w() );

            _points3d.push_back( _pointFrustumNormalized );
        }

        // near face
        _drawLine( _points3d[0], _points3d[1], color );
        _drawLine( _points3d[1], _points3d[2], color );
        _drawLine( _points3d[2], _points3d[3], color );
        _drawLine( _points3d[3], _points3d[0], color );

        // far face
        _drawLine( _points3d[4], _points3d[5], color );
        _drawLine( _points3d[5], _points3d[6], color );
        _drawLine( _points3d[6], _points3d[7], color );
        _drawLine( _points3d[7], _points3d[4], color );

        // side lines
        _drawLine( _points3d[0], _points3d[4], color );
        _drawLine( _points3d[1], _points3d[5], color );
        _drawLine( _points3d[2], _points3d[6], color );
        _drawLine( _points3d[3], _points3d[7], color );
    }

    void CDebugDrawer::_drawTrailPoints( const std::vector< CVec3 >& trailpoints, const CVec3& color )
    {
        if ( trailpoints.size() < 1 )
            return;

        for ( size_t q = 0; q < trailpoints.size() - 1; q++ )
            _drawLine( trailpoints[q], trailpoints[ q + 1 ], color );
    }

    void CDebugDrawer::_drawLinesBatch( const std::vector< CLine >& linesBatch, const CVec3& color )
    {
        if ( linesBatch.size() < 1 )
            return;

        for ( size_t q = 0; q < linesBatch.size(); q++ )
            _drawLine( linesBatch[q].start, linesBatch[q].end, color );
    }

    void CDebugDrawer::_drawAABB( const CVec3& aabbMin, 
                                  const CVec3& aabbMax, 
                                  const CMat4& worldTransform, 
                                  const CVec3& color )
    {
        auto _vmin2max = aabbMax - aabbMin;
        auto _dx = _vmin2max.dot( CVec3( worldTransform.col( 0 ) ) );
        auto _dy = _vmin2max.dot( CVec3( worldTransform.col( 1 ) ) );
        auto _dz = _vmin2max.dot( CVec3( worldTransform.col( 2 ) ) );
        auto _origin = CVec3( worldTransform.col( 3 ) );
        auto _sidex = CVec3( worldTransform.col( 0 ) );
        auto _sidey = CVec3( worldTransform.col( 1 ) );
        auto _sidez = CVec3( worldTransform.col( 2 ) );

        _sidex.scale( CVec3( 0.5 * _dx, 0.5 * _dx, 0.5 * _dx ) );
        _sidey.scale( CVec3( 0.5 * _dy, 0.5 * _dy, 0.5 * _dy ) );
        _sidez.scale( CVec3( 0.5 * _dz, 0.5 * _dz, 0.5 * _dz ) );

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

        _drawLine( _p1, _p2, color ); _drawLine( _p2, _p3, color );
        _drawLine( _p3, _p4, color ); _drawLine( _p4, _p1, color );

        _drawLine( _p2, _p6, color ); _drawLine( _p6, _p7, color );
        _drawLine( _p7, _p3, color ); _drawLine( _p3, _p2, color );

        _drawLine( _p4, _p3, color ); _drawLine( _p3, _p7, color );
        _drawLine( _p7, _p8, color ); _drawLine( _p8, _p4, color );

        _drawLine( _p4, _p8, color ); _drawLine( _p8, _p5, color );
        _drawLine( _p5, _p1, color ); _drawLine( _p1, _p4, color );

        _drawLine( _p1, _p2, color ); _drawLine( _p2, _p6, color );
        _drawLine( _p6, _p5, color ); _drawLine( _p5, _p1, color );

        _drawLine( _p5, _p6, color ); _drawLine( _p6, _p7, color );
        _drawLine( _p7, _p8, color ); _drawLine( _p8, _p5, color );
    }

    void CDebugDrawer::_drawNormals( CMesh* meshPtr, const CVec3& color )
    {
        if ( !meshPtr )
            return;

        auto _vertices = meshPtr->vertices();
        auto _normals = meshPtr->normals();

        auto _modelMatrix = meshPtr->matModel();
        auto _normalMatrix = tinymath::inverse( _modelMatrix ).transpose();

        for ( size_t i = 0; i < _vertices.size(); i++ )
        {
            auto _wposition = _modelMatrix * CVec4( _vertices[i], 1.0f );
            auto _wnormal = _normalMatrix * CVec4( _normals[i], 0.0f );

            CVec3 _position3d = CVec3( _wposition.x(), _wposition.y(), _wposition.z() );
            CVec3 _normal3d = CVec3( _wnormal.x(), _wnormal.y(), _wnormal.z() ).normalized();

            _drawArrow( _position3d, _position3d + 0.1f * _normal3d, color );
        }
    }

    void CDebugDrawer::_drawAxes( const CVec3& xAxis, const CVec3& yAxis, const CVec3& zAxis, const CVec3& position, float32 size )
    {
        _drawArrow( position, position + size * xAxis, { 1.0f, 0.0f, 0.0f } );
        _drawArrow( position, position + size * yAxis, { 0.0f, 1.0f, 0.0f } );
        _drawArrow( position, position + size * zAxis, { 0.0f, 0.0f, 1.0f } );
    }

    void CDebugDrawer::_drawFrame( const CMat4& frame, float32 size )
    {
        auto _xAxis = CVec3( frame.col( 0 ) );
        auto _yAxis = CVec3( frame.col( 1 ) );
        auto _zAxis = CVec3( frame.col( 2 ) );
        auto _position = CVec3( frame.col( 3 ) );

        _drawAxes( _xAxis, _yAxis, _zAxis, _position, size );
    }

    void CDebugDrawer::_drawPlane( const CPlane& plane, const CVec2& size, const CVec3& color )
    {
        CVec3 _axis1, _axis2, _axis3;
        _axis1 = plane.normal;
        computeFrameAxes( _axis1, _axis2, _axis3, { 0.0f, 1.0f, 0.0f } );

        auto _p0 = plane.position - 0.5f * size.x() * _axis2 - 0.5f * size.y() * _axis3;
        auto _p1 = plane.position - 0.5f * size.x() * _axis2 + 0.5f * size.y() * _axis3;
        auto _p2 = plane.position + 0.5f * size.x() * _axis2 + 0.5f * size.y() * _axis3;
        auto _p3 = plane.position + 0.5f * size.x() * _axis2 - 0.5f * size.y() * _axis3;

        _drawLine( _p0, _p1, color );
        _drawLine( _p1, _p2, color );
        _drawLine( _p2, _p3, color );
        _drawLine( _p3, _p0, color );
    }

    void CDebugDrawer::_drawSolidBox( const CVec3& size, const CMat4& transform, const CVec4& color )
    {
        // keep the cube-information for later rendering
        if ( color.w() < 1.0f ) // transparent are stored separately
        {
            m_primitivesTransparentModelMats[DD_PRIMITIVE_BOX].push_back( transform * engine::scale( size ) );
            m_primitivesTransparentNormalMats[DD_PRIMITIVE_BOX].push_back( tinymath::inverse( transform ).transpose() );
            m_primitivesTransparentColors[DD_PRIMITIVE_BOX].push_back( color );
        }
        else
        {
            m_primitivesOpaqueModelMats[DD_PRIMITIVE_BOX].push_back( transform * engine::scale( size ) );
            m_primitivesOpaqueNormalMats[DD_PRIMITIVE_BOX].push_back( tinymath::inverse( transform ).transpose() );
            m_primitivesOpaqueColors[DD_PRIMITIVE_BOX].push_back( color );
        }
    }

    void CDebugDrawer::_drawSolidSphere( float32 radius, const CMat4& transform, const CVec4& color )
    {
        if ( color.w() < 1.0f )
        {
            m_primitivesTransparentModelMats[DD_PRIMITIVE_SPHERE].push_back( transform * engine::scale( { radius, radius, radius } ) );
            m_primitivesTransparentNormalMats[DD_PRIMITIVE_SPHERE].push_back( tinymath::inverse( transform ).transpose() );
            m_primitivesTransparentColors[DD_PRIMITIVE_SPHERE].push_back( color );
        }
        else
        {
            m_primitivesOpaqueModelMats[DD_PRIMITIVE_SPHERE].push_back( transform * engine::scale( { radius, radius, radius } ) );
            m_primitivesOpaqueNormalMats[DD_PRIMITIVE_SPHERE].push_back( tinymath::inverse( transform ).transpose() );
            m_primitivesOpaqueColors[DD_PRIMITIVE_SPHERE].push_back( color );
        }
    }

    void CDebugDrawer::_drawSolidCylinder( float32 radius, float32 height, const eAxis& axis, const CMat4& transform, const CVec4& color )
    {
        if ( color.w() < 1.0f )
        {
            if ( axis == eAxis::X )
            {
                m_primitivesTransparentModelMats[DD_PRIMITIVE_CYLINDER_X].push_back( transform * engine::scale( { height, radius, radius } ) );
                m_primitivesTransparentNormalMats[DD_PRIMITIVE_CYLINDER_X].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesTransparentColors[DD_PRIMITIVE_CYLINDER_X].push_back( color );
            }
            else if ( axis == eAxis::Y )
            {
                m_primitivesTransparentModelMats[DD_PRIMITIVE_CYLINDER_Y].push_back( transform * engine::scale( { radius, height, radius } ) );
                m_primitivesTransparentNormalMats[DD_PRIMITIVE_CYLINDER_Y].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesTransparentColors[DD_PRIMITIVE_CYLINDER_Y].push_back( color );
            }
            else if ( axis == eAxis::Z )
            {
                m_primitivesTransparentModelMats[DD_PRIMITIVE_CYLINDER_Z].push_back( transform * engine::scale( { radius, radius, height } ) );
                m_primitivesTransparentNormalMats[DD_PRIMITIVE_CYLINDER_Z].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesTransparentColors[DD_PRIMITIVE_CYLINDER_Z].push_back( color );
            }
        }
        else
        {
            if ( axis == eAxis::X )
            {
                m_primitivesOpaqueModelMats[DD_PRIMITIVE_CYLINDER_X].push_back( transform * engine::scale( { height, radius, radius } ) );
                m_primitivesOpaqueNormalMats[DD_PRIMITIVE_CYLINDER_X].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesOpaqueColors[DD_PRIMITIVE_CYLINDER_X].push_back( color );
            }
            else if ( axis == eAxis::Y )
            {
                m_primitivesOpaqueModelMats[DD_PRIMITIVE_CYLINDER_Y].push_back( transform * engine::scale( { radius, height, radius } ) );
                m_primitivesOpaqueNormalMats[DD_PRIMITIVE_CYLINDER_Y].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesOpaqueColors[DD_PRIMITIVE_CYLINDER_Y].push_back( color );
            }
            else if ( axis == eAxis::Z )
            {
                m_primitivesOpaqueModelMats[DD_PRIMITIVE_CYLINDER_Z].push_back( transform * engine::scale( { radius, radius, height } ) );
                m_primitivesOpaqueNormalMats[DD_PRIMITIVE_CYLINDER_Z].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesOpaqueColors[DD_PRIMITIVE_CYLINDER_Z].push_back( color );
            }
        }
    }

    void CDebugDrawer::_drawSolidCapsule( float32 radius, float32 height, const eAxis& axis, const CMat4& transform, const CVec4& color )
    {
        _drawSolidCylinder( radius, height, axis, transform, color );

        CVec3 _trans;
        if ( axis == eAxis::X ) _trans = CVec3( 0.5f * height, 0.0f, 0.0f );
        if ( axis == eAxis::Y ) _trans = CVec3( 0.0f, 0.5f * height, 0.0f );
        if ( axis == eAxis::Z ) _trans = CVec3( 0.0f, 0.0f, 0.5f * height );

        _drawSolidSphere( radius, transform * engine::translation( _trans ), color );
        _drawSolidSphere( radius, transform * engine::translation( -_trans ), color );
    }

    void CDebugDrawer::_drawSolidArrow( float32 length, const eAxis& axis, const CMat4& transform, const CVec4& color )
    {
        if ( color.w() < 1.0f )
        {
            if ( axis == eAxis::X )
            {
                m_primitivesTransparentModelMats[DD_PRIMITIVE_ARROW_X].push_back( transform * engine::scale( { length, length, length } ) );
                m_primitivesTransparentNormalMats[DD_PRIMITIVE_ARROW_X].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesTransparentColors[DD_PRIMITIVE_ARROW_X].push_back( color );
            }
            else if ( axis == eAxis::Y )
            {
                m_primitivesTransparentModelMats[DD_PRIMITIVE_ARROW_Y].push_back( transform * engine::scale( { length, length, length } ) );
                m_primitivesTransparentNormalMats[DD_PRIMITIVE_ARROW_Y].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesTransparentColors[DD_PRIMITIVE_ARROW_Y].push_back( color );
            }
            else if ( axis == eAxis::Z )
            {
                m_primitivesTransparentModelMats[DD_PRIMITIVE_ARROW_Z].push_back( transform * engine::scale( { length, length, length } ) );
                m_primitivesTransparentNormalMats[DD_PRIMITIVE_ARROW_Z].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesTransparentColors[DD_PRIMITIVE_ARROW_Z].push_back( color );
            }
        }
        else
        {
            if ( axis == eAxis::X )
            {
                m_primitivesOpaqueModelMats[DD_PRIMITIVE_ARROW_X].push_back( transform * engine::scale( { length, length, length } ) );
                m_primitivesOpaqueNormalMats[DD_PRIMITIVE_ARROW_X].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesOpaqueColors[DD_PRIMITIVE_ARROW_X].push_back( color );
            }
            else if ( axis == eAxis::Y )
            {
                m_primitivesOpaqueModelMats[DD_PRIMITIVE_ARROW_Y].push_back( transform * engine::scale( { length, length, length } ) );
                m_primitivesOpaqueNormalMats[DD_PRIMITIVE_ARROW_Y].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesOpaqueColors[DD_PRIMITIVE_ARROW_Y].push_back( color );
            }
            else if ( axis == eAxis::Z )
            {
                m_primitivesOpaqueModelMats[DD_PRIMITIVE_ARROW_Z].push_back( transform * engine::scale( { length, length, length } ) );
                m_primitivesOpaqueNormalMats[DD_PRIMITIVE_ARROW_Z].push_back( tinymath::inverse( transform ).transpose() );
                m_primitivesOpaqueColors[DD_PRIMITIVE_ARROW_Z].push_back( color );
            }
        }
    }

    void CDebugDrawer::_drawSolidAxes( float32 length, const CMat4& transform, float32 alpha )
    {
        _drawSolidArrow( length, eAxis::X, transform, CVec4( CVec3( 1.0f, 0.0f, 0.0f ), alpha ) );
        _drawSolidArrow( length, eAxis::Y, transform, CVec4( CVec3( 0.0f, 1.0f, 0.0f ), alpha ) );
        _drawSolidArrow( length, eAxis::Z, transform, CVec4( CVec3( 0.0f, 0.0f, 1.0f ), alpha ) );
        _drawSolidSphere( 0.2 * length, transform, { 0.3f, 0.3f, 0.3f, alpha } );
    }


}
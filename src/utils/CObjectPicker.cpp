
#include <utils/CObjectPicker.h>


namespace engine
{

    std::string toString( const ePickerMode& mode )
    {
        if ( mode == ePickerMode::NORMAL ) return "normal";
        if ( mode == ePickerMode::VISUALIZE ) return "visualize";
        if ( mode == ePickerMode::STOPPED ) return "stopped";

        ENGINE_CORE_ASSERT( false, "Invalid ePickerMode enum given" );

        return "undefined";
    }

    CObjectPicker::CObjectPicker( int viewportWidth, int viewportHeight )
    {
        m_viewportWidth = viewportWidth;
        m_viewportHeight = viewportHeight;

        engine::CAttachmentConfig _fbColorConfig;
        _fbColorConfig.name                 = "color_attachment";
        _fbColorConfig.attachment           = engine::eFboAttachment::COLOR;
        _fbColorConfig.texInternalFormat    = engine::eTextureFormat::RGB;
        _fbColorConfig.texFormat            = engine::eTextureFormat::RGB;
        _fbColorConfig.texPixelDataType     = engine::ePixelDataType::UINT_8;
        _fbColorConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbColorConfig.texWrapV             = engine::eTextureWrap::REPEAT;

        engine::CAttachmentConfig _fbDepthConfig;
        _fbDepthConfig.name                 = "depth_attachment";
        _fbDepthConfig.attachment           = engine::eFboAttachment::DEPTH;
        _fbDepthConfig.texInternalFormat    = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texFormat            = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texPixelDataType     = engine::ePixelDataType::UINT_32;
        _fbDepthConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbDepthConfig.texWrapV             = engine::eTextureWrap::REPEAT;

        m_fboObjsIds = std::make_unique< CFrameBuffer >( m_viewportWidth, m_viewportHeight );
        m_fboObjsIds->addAttachment( _fbColorConfig );
        m_fboObjsIds->addAttachment( _fbDepthConfig );

        m_shaderObjIdEncoder = CShaderManager::GetCachedShader( "engine_render_semantic_view" );
        ENGINE_CORE_ASSERT( m_shaderObjIdEncoder, "Could not load required shader to encode obj-ids into colors" );

        m_mode = ePickerMode::NORMAL;
    }

    CObjectPicker::~CObjectPicker()
    {
        m_fboObjsIds = nullptr;
        m_shaderObjIdEncoder = nullptr;
    }

    void CObjectPicker::setMode( const ePickerMode& mode )
    {
        m_mode = mode;
    }

    void CObjectPicker::begin( CICamera* camera )
    {
        if ( m_mode == ePickerMode::STOPPED )
            return;

        // clear buffers for submissions
        m_renderablesInView.clear();

        // configure view-proj matrix to be used
        m_viewProjMatrix = camera->mat_proj() * camera->mat_view();

        // create view-frustum to be used for culling
        m_viewFrustum = std::unique_ptr< CFrustum >( new CFrustum( m_viewProjMatrix ) );
    }

    void CObjectPicker::submit( const std::vector< CIRenderable* >& renderables )
    {
        if ( m_mode == ePickerMode::STOPPED )
            return;

        // collect all in-view objects
        for ( auto renderablePtr : renderables )
        {
            if ( !renderablePtr->visible() )
                continue;

            if ( engine::certainlyOutsideFrustum( *m_viewFrustum, renderablePtr->bbox() ) )
                continue;

            m_renderablesInView.push_back( renderablePtr );
        }
    }

    void CObjectPicker::render()
    {
        if ( m_mode == ePickerMode::STOPPED )
            return;

        /* render the objects to the fbo with obj-ids encoded in rgb (0: background, 1->: objects) */

        // bind the target we want to use for storing the rendering results
        m_fboObjsIds->bind();

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );
        glEnable( GL_CULL_FACE ); // either front or back faces give the same result

        m_shaderObjIdEncoder->bind();
        m_shaderObjIdEncoder->setMat4( "u_viewProjMatrix", m_viewProjMatrix );

        for ( size_t i = 0; i < m_renderablesInView.size(); i++ )
        {
            CVec3 _colorEncoding = { 0.0f, 0.0f, 0.0f };
            if ( m_mode == ePickerMode::NORMAL ) 
                _colorEncoding = _createNormalEncoding( i );
            else if ( m_mode == ePickerMode::VISUALIZE ) 
                _colorEncoding = _createVisualizationEncoding( i );

            //// if ( m_mode == ePickerMode::NORMAL )
            ////     ENGINE_CORE_TRACE( "encoding {0}: {1}", i, engine::toString( _colorEncoding ) );

            m_shaderObjIdEncoder->setVec3( "u_maskColor", _colorEncoding );

            if ( m_renderablesInView[i]->type() == eRenderableType::MESH )
                _encodeMesh( dynamic_cast< CMesh* >( m_renderablesInView[i] ) );
            else if ( m_renderablesInView[i]->type() == eRenderableType::MODEL )
                _encodeModel( dynamic_cast< CModel* >( m_renderablesInView[i] ) );
        }

        m_shaderObjIdEncoder->unbind();
        glDisable( GL_CULL_FACE );

        m_fboObjsIds->unbind();

        // request to send render commands to gpu, and wait till everything has finished, which
        // makes sense if render calls are non blocking, but is it actually required?
        glFlush();
        glFinish();
    }

    CIRenderable* CObjectPicker::getObjectPicked( float x, float y, int srcViewportWidth, int srcViewportHeight )
    {
        if ( m_mode != ePickerMode::NORMAL )
            return nullptr;

        /* convert to normalized coordinates [0,1] using viewport dimensions */
        float _xnorm = x / srcViewportWidth;
        float _ynorm = 1.0f - y / srcViewportHeight;
        /* convert to viewport dimensions used by our fbo */
        float _xx = _xnorm * m_viewportWidth;
        float _yy = _ynorm * m_viewportHeight;

        //// ENGINE_CORE_TRACE( "(xnorm, ynorm) = ({0},{1}), (xx, yy) = ({2},{3})", _xnorm, _ynorm, _xx, _yy );

        m_fboObjsIds->bind();

        /* grab the pixel-encoding at the requested position */
        uint8 _encoding[3];
        glReadPixels( (int)_xx, (int)_yy, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, _encoding );

        //// ENGINE_CORE_TRACE( "encoding: ({0},{1},{2})", _encoding[0], _encoding[1], _encoding[2] );

        m_fboObjsIds->unbind();

        /* decode the rgb color into an object id */
        int _objectId = ( _encoding[0] << 0 ) + ( _encoding[1] << 8 ) + ( _encoding[2] << 16 ) - 1;

        //// ENGINE_CORE_TRACE( "object-id: {0}", _objectId );

        if ( ( _objectId >= 0 ) && ( _objectId < m_renderablesInView.size() ) )
            return m_renderablesInView[_objectId];

        return nullptr;
    }

    CFrameBuffer* CObjectPicker::getFbo()
    {
        return m_fboObjsIds.get();
    }

    CVec3 CObjectPicker::_createNormalEncoding( int objId )
    {
        return { ( ( ( objId + 1 ) & 0x000000ff ) >> 0 ) / 255.0f,
                 ( ( ( objId + 1 ) & 0x0000ff00 ) >> 8 ) / 255.0f,
                 ( ( ( objId + 1 ) & 0x00ff0000 ) >> 16 ) / 255.0f };
    }

    CVec3 CObjectPicker::_createVisualizationEncoding( int objId )
    {
        if ( m_cachedRandomColors.find( objId ) == m_cachedRandomColors.end() )
            m_cachedRandomColors[objId] = { m_randDist( m_randGen ), m_randDist( m_randGen ), m_randDist( m_randGen ) };

        return m_cachedRandomColors[objId];
    }

    void CObjectPicker::_encodeMesh( CMesh* meshPtr )
    {
        m_shaderObjIdEncoder->setMat4( "u_modelMatrix", meshPtr->matModel() );
        meshPtr->render();
    }

    void CObjectPicker::_encodeModel( CModel* modelPtr )
    {
        auto _submeshes = modelPtr->meshes();
        auto _localTransforms = modelPtr->localTransforms();
        auto _modelTransform = modelPtr->matModel();

        for ( size_t i = 0; i < _submeshes.size(); i++ )
        {
            if ( !_submeshes[i]->visible() )
                continue;

            m_shaderObjIdEncoder->setMat4( "u_modelMatrix", _modelTransform * _localTransforms[i] );
            _submeshes[i]->render();
        }
    }

}
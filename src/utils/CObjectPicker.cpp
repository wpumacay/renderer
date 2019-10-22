
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

    CObjectPicker* CObjectPicker::s_instance = nullptr;

    void CObjectPicker::Init( int viewportWidth, int viewportHeight )
    {
        if ( CObjectPicker::s_instance )
        {
            ENGINE_CORE_WARN( "Attempting to initialize object-picker twice" );
            return;
        }

        CObjectPicker::s_instance = new CObjectPicker( viewportWidth, viewportHeight );
    }

    void CObjectPicker::Release()
    {
        ENGINE_CORE_ASSERT( CObjectPicker::s_instance, "Tried to release object-picker more than once" );

        delete CObjectPicker::s_instance;
        CObjectPicker::s_instance = nullptr;
    }

    void CObjectPicker::SetMode( const ePickerMode& mode )
    {
        ENGINE_CORE_ASSERT( CObjectPicker::s_instance, "Must initialize object-picker before using it" );

        CObjectPicker::s_instance->_setMode( mode );
    }

    void CObjectPicker::Submit( const std::vector< CIRenderable* >& renderables, CICamera* camera )
    {
        ENGINE_CORE_ASSERT( CObjectPicker::s_instance, "Must initialize object-picker before using it" );

        CObjectPicker::s_instance->_submit( renderables, camera );
    }

    CIRenderable* CObjectPicker::GetObjectPicked( float x, float y, int srcViewportWidth, int srcViewportHeight )
    {
        ENGINE_CORE_ASSERT( CObjectPicker::s_instance, "Must initialize object-picker before using it" );

        return CObjectPicker::s_instance->_getObjectPicked( x, y, srcViewportWidth, srcViewportHeight );
    }

    CFrameBuffer* CObjectPicker::GetFbo()
    {
        ENGINE_CORE_ASSERT( CObjectPicker::s_instance, "Must initialize object-picker before using it" );

        return CObjectPicker::s_instance->_getFbo();
    }

    CObjectPicker::CObjectPicker( int viewportWidth, int viewportHeight )
    {
        m_viewportWidth = viewportWidth;
        m_viewportHeight = viewportHeight;

        engine::CAttachmentConfig _fbColorConfig;
        _fbColorConfig.name                 = "color_attachment";
        _fbColorConfig.attachment           = engine::eFboAttachment::COLOR;
        _fbColorConfig.width                = m_viewportWidth;
        _fbColorConfig.height               = m_viewportHeight;
        _fbColorConfig.texInternalFormat    = engine::eTextureFormat::RGB;
        _fbColorConfig.texFormat            = engine::eTextureFormat::RGB;
        _fbColorConfig.texPixelDataType     = engine::ePixelDataType::UINT_8;
        _fbColorConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbColorConfig.texWrapV             = engine::eTextureWrap::REPEAT;

        engine::CAttachmentConfig _fbDepthConfig;
        _fbDepthConfig.name                 = "depth_attachment";
        _fbDepthConfig.attachment           = engine::eFboAttachment::DEPTH;
        _fbDepthConfig.width                = m_viewportWidth;
        _fbDepthConfig.height               = m_viewportHeight;
        _fbDepthConfig.texInternalFormat    = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texFormat            = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texPixelDataType     = engine::ePixelDataType::UINT_32;
        _fbDepthConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbDepthConfig.texWrapV             = engine::eTextureWrap::REPEAT;

        m_fboObjsIds = std::unique_ptr< CFrameBuffer >( new CFrameBuffer() );
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

    void CObjectPicker::_setMode( const ePickerMode& mode )
    {
        m_mode = mode;
    }

    void CObjectPicker::_submit( const std::vector< CIRenderable* >& renderables, CICamera* camera )
    {
        if ( m_mode == ePickerMode::STOPPED )
            return;

        // create view-frustum object for frustum-culling
        CMat4 _viewProjMatrix = camera->matProj() * camera->matView();
        CFrustum _frustum( _viewProjMatrix );

        // collect all in-view objects
        m_renderablesInView.clear();
        for ( auto renderablePtr : renderables )
        {
            if ( !renderablePtr->visible() )
                continue;

            if ( engine::certainlyOutsideFrustum( _frustum, renderablePtr->bbox() ) )
                continue;

            m_renderablesInView.push_back( renderablePtr );
        }

        // render the objects to the fbo with obj-ids encoded as colors (0: background, 1->: objects)
        m_fboObjsIds->bind();
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );
        glEnable( GL_CULL_FACE ); // either front or back faces give the same result
        m_shaderObjIdEncoder->bind();
        m_shaderObjIdEncoder->setMat4( "u_viewProjMatrix", _viewProjMatrix );

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

    CIRenderable* CObjectPicker::_getObjectPicked( float x, float y, int srcViewportWidth, int srcViewportHeight )
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

        ENGINE_CORE_TRACE( "object-id: {0}", _objectId );

        if ( ( _objectId >= 0 ) && ( _objectId < m_renderablesInView.size() ) )
            return m_renderablesInView[_objectId];

        return nullptr;
    }

    CFrameBuffer* CObjectPicker::_getFbo()
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
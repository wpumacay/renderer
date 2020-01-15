
#include <renderers/CSkyboxRenderer.h>

// @todo: implement helpers for 4x4 manipulation (GetRotation, GetTranslation, RotationX, ...)

namespace engine
{

    CSkyboxRenderer::CSkyboxRenderer()
    {
        m_shaderSkyboxFog = CShaderManager::GetCachedShader( "engine_skybox_fog" );
        m_shaderSkyboxNoFog = CShaderManager::GetCachedShader( "engine_skybox_no_fog" );

        m_context.skyboxCubeVaoId = 0;
        m_context.skyboxCubemapId = 0;
    }

    CSkyboxRenderer::~CSkyboxRenderer()
    {
        m_shaderSkyboxFog = nullptr;
        m_shaderSkyboxNoFog = nullptr;
    }

    void CSkyboxRenderer::begin( const CRenderOptions& renderOptions )
    {
        if ( !renderOptions.cameraPtr )
            ENGINE_CORE_ASSERT( false, "Skybox-renderer requires a valid camera to render" );

        if ( !renderOptions.skyboxPtr )
            ENGINE_CORE_ASSERT( false, "Skybox-renderer requires a valid skybox resource to render" );

        if ( renderOptions.useFog && !renderOptions.fogPtr )
            ENGINE_CORE_WARN( "Skybox-renderer requires a valid fog resource if rendering with fog is enabled" );

        // setup view information
        CMat4 _camMatView, _camMatProj, _upAxisCorrectionMat;

        _camMatView = renderOptions.cameraPtr->matView();
        _camMatProj = renderOptions.cameraPtr->matProj();

        if ( renderOptions.cameraPtr->upAxis() == engine::eAxis::X ) 
            _upAxisCorrectionMat = CMat4( tinymath::rotationZ<float32>( -ENGINE_PI / 2.0f ), CVec3( 0.0f, 0.0f, 0.0f ) );
        else if ( renderOptions.cameraPtr->upAxis() == engine::eAxis::Z ) 
            _upAxisCorrectionMat = CMat4( tinymath::rotationX<float32>( ENGINE_PI / 2.0f ), CVec3( 0.0f, 0.0f, 0.0f ) );

        m_context.viewMatrix = CMat4( CMat3( _camMatView ), CVec3( 0.0f, 0.0f, 0.0f ) ) * _upAxisCorrectionMat;
        m_context.projMatrix = _camMatProj;

        // setup fog information
        m_context.useFog = false;
        if ( renderOptions.useFog && renderOptions.fogPtr )
        {
            m_context.useFog = true;
            m_context.fogColor = renderOptions.fogPtr->color;
            m_context.fogLowerLimit = renderOptions.skyboxFogLowerLimit;
            m_context.fogUpperLimit = renderOptions.skyboxFogUpperLimit;
        }

        // setup gl-resources information
        m_context.skyboxCubeVaoId = renderOptions.skyboxPtr->vao()->openglId();
        m_context.skyboxCubemapId = renderOptions.skyboxPtr->cubemap()->openglId();
    }

    void CSkyboxRenderer::render()
    {
        auto _shader = ( m_context.useFog ) ? m_shaderSkyboxFog : m_shaderSkyboxNoFog;

        glDepthMask( GL_FALSE );
        _shader->bind();
        _shader->setMat4( "u_viewProjMatrix", m_context.projMatrix * m_context.viewMatrix );
        if ( m_context.useFog )
        {
            _shader->setVec3( "u_fog.color", m_context.fogColor );
            _shader->setFloat( "u_fog.lowerLimit", m_context.fogLowerLimit );
            _shader->setFloat( "u_fog.upperLimit", m_context.fogUpperLimit );
        }

        glBindVertexArray( m_context.skyboxCubeVaoId );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_context.skyboxCubemapId );

        glDrawArrays( GL_TRIANGLES, 0, 36 );

        glBindVertexArray( 0 );
        glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

        _shader->unbind();
        glDepthMask( GL_TRUE );
    }

}
#pragma once

#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>
#include <core/CFrameBuffer.h>

namespace engine
{

    class CShadowMap
    {

    public :

        CShadowMap( int32 width, int32 height );
        ~CShadowMap();

        void bind();
        void unbind();

        void setupDirectionalLight( CDirectionalLight* dirLightPtr );
        void setupPointLight( CPointLight* pointLightPtr );
        void setupSpotLight( CSpotLight* spotLightPtr );

        CFrameBuffer* frameBuffer() const { return m_frameBuffer.get(); };

        int32 width() const { return m_shadowMapWidth; }
        int32 height() const { return m_shadowMapHeight; }

        CMat4 lightSpaceMatView() const { return m_lightSpaceMatView; }
        CMat4 lightSpaceMatProj() const { return m_lightSpaceMatProj; }

    private :

        /* FBO object (with only depth attachment) */
        std::unique_ptr< CFrameBuffer > m_frameBuffer;

        /* view-matrix from the point of view of the light (where to look when projecting to the depth-matp) */
        CMat4 m_lightSpaceMatView;

        /* proj-matrix from the point of view of the light (what to project into depth-map) */
        CMat4 m_lightSpaceMatProj;

        /* shadowmap dimensions (used to set the viewport) */
        int32 m_shadowMapWidth;
        int32 m_shadowMapHeight;

        /* viewport configuration to be restored after using the shadowmap */
        int32 m_previousViewportX;
        int32 m_previousViewportY;
        int32 m_previousViewportWidth;
        int32 m_previousViewportHeight;
    };

}
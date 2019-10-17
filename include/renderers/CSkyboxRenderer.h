#pragma once

#include <renderers/CRenderersCommon.h>

namespace engine
{

    struct CSkyboxRenderContext
    {
        /* view-information */
        CMat4   viewMatrix;
        CMat4   projMatrix;
        /* fog-related information */
        bool    useFog;
        CVec3   fogColor;
        float32 fogLowerLimit;
        float32 fogUpperLimit;
        /* gl-resources of the skybox */
        uint32  skyboxCubeVaoId;
        uint32  skyboxCubemapId;
    };

    class CSkyboxRenderer
    {

    public :

        CSkyboxRenderer();
        ~CSkyboxRenderer();

        void submit( const CRenderOptions& renderOptions );
        void render();

    private :

        CShader*                m_shaderSkyboxNoFog;
        CShader*                m_shaderSkyboxFog;
        CSkyboxRenderContext    m_context;

    };

}
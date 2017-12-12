

#pragma once

#include "LIRenderer.h"
#include "LScene.h"

using namespace std;

namespace engine
{

    class LSceneRenderer : public LIRenderer
    {

        private :

        bool m_lightingEnabled;
        
        GLuint m_uniformProj;
        GLuint m_uniformModel;
        GLuint m_uniformView;

        GLuint m_uniformNumDirectionalLights;
        GLuint m_uniformNumPointLights;
        GLuint m_uniformNumSpotLights;

        GLuint m_uniformViewPos;
        GLuint m_uniformGlobalLight;

        public :


        LSceneRenderer();
        ~LSceneRenderer();

        void enableLighting();
        void disableLighting();


        void renderScene( LScene* pScene ) override;
        void render( LIRenderable* pRenderable ) override;
        void begin( LScene* pScene ) override;
        void end( LScene* pScene ) override;
    };





}




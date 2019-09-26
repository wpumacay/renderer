#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>
#include <LFog.h>
#include <LSkybox.h>
#include <LIRenderable.h>

#include <camera/CICamera.h>
#include <camera/CFpsCamera.h>
#include <camera/CFixedCamera.h>

namespace engine
{

    class LScene
    {
    public :

        LScene();
        ~LScene();

        void addRenderable( LIRenderable* pRenderable );
        void addFog( LFog* pFog );
        void addLight( CILight* pLight ); 
        void addCamera( CICamera* pCamera );
        void addSkybox( LSkybox* pSkybox );

        void changeToCameraByName( const std::string& cameraId );

        void cleanScene();

        CMat4 getProjMatrix() const { return m_projMatrix; }
        CICamera* getCurrentCamera() const { return m_currentCamera; }
        std::vector< CILight* > lights() const { return m_lights; }

        LFog* getFog() const;
        LSkybox* getSkybox() const;
        std::vector< LIRenderable* > getRenderables() const;
        std::vector< CDirectionalLight* > directionalLights() const { return m_directionalLights; }
        std::vector< CPointLight* > pointLights() const { return m_pointLights; }
        std::vector< CSpotLight* > spotLights() const { return m_spotLights; }

        virtual void update( float dt );

    protected :

        std::vector< LIRenderable* > m_renderables;
        std::map< std::string, CICamera* > m_cameras;
        std::vector< CILight* > m_lights;
        std::vector< CDirectionalLight* > m_directionalLights;
        std::vector< CPointLight* > m_pointLights;
        std::vector< CSpotLight* > m_spotLights;

        LFog* m_fog;
        LSkybox* m_skybox;
        CICamera* m_currentCamera;
        CMat4 m_projMatrix;
    };

}

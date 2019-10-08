#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>

#include <graphics/CFog.h>
#include <graphics/CSkybox.h>
#include <graphics/CIRenderable.h>

#include <camera/CICamera.h>
#include <camera/CFpsCamera.h>
#include <camera/CFixedCamera.h>

namespace engine
{

    class CScene
    {

    public :

        CScene();
        ~CScene();

        void addFog( std::unique_ptr< CFog > fog );
        void addSkybox( std::unique_ptr< CSkybox > skybox );
        void addCamera( std::unique_ptr< CICamera > camera );
        void addLight( std::unique_ptr< CILight > light );
        void addRenderable( std::unique_ptr< CIRenderable > renderable );

        void clearScene();
        void changeToCamera( const std::string& name );

        void update();
        void resize( int32 width, int32 height );

        CFog* fog() const { return m_fog.get(); }
        CSkybox* skybox() const { return m_skybox.get(); }
        CICamera* currentCamera() const { return m_currentCamera; }

        std::vector< CIRenderable* > renderables() const;
        std::vector< CICamera* > cameras() const;

        std::vector< CILight* > lights() const;
        std::vector< CDirectionalLight* > directionalLights() const;
        std::vector< CPointLight* > pointLights() const;
        std::vector< CSpotLight* > spotLights() const;

    private :

        std::unique_ptr< CFog > m_fog;
        std::unique_ptr< CSkybox > m_skybox;

        CICamera*                                   m_currentCamera;
        std::vector< std::unique_ptr< CICamera > >  m_cameras;
        std::map< std::string, CICamera* >          m_camerasMap;

        std::vector< std::unique_ptr< CILight > >   m_lights;
        std::map< std::string, CILight* >           m_lightsMap;
        std::vector< CDirectionalLight* >           m_directionalLights;
        std::vector< CPointLight* >                 m_pointLights;
        std::vector< CSpotLight* >                  m_spotLights;

        std::vector< std::unique_ptr< CIRenderable > >  m_renderables;
    };

}
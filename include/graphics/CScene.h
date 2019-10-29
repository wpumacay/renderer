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
#include <camera/CFixedCamera.h>
#include <camera/COrbitCamera.h>
#include <camera/CFpsCamera.h>

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
        void changeToCamera( const std::string& name ); // change current working camera
        void changeMainLight( const std::string& name ); // change current working light

        void update();
        void resize( int32 width, int32 height );

        CFog* fog() const { return m_fog.get(); }
        CSkybox* skybox() const { return m_skybox.get(); }
        CICamera* currentCamera() const { return m_currentCamera; }
        CILight* mainLight() const { return m_mainLight; }

        std::vector< CICamera* > cameras() const;
        bool hasCamera( const std::string& name ) const;
        CICamera* getCamera( const std::string& name );

        std::vector< CIRenderable* > renderables() const;
        bool hasRenderable( const std::string& name ) const;
        CIRenderable* getRenderable( const std::string& name );

        template< typename T >
        std::vector< T* > collectTypedRenderables() const
        {
            std::vector< T* > _typedRenderables;
            for ( auto& renderable : m_renderables )
                if ( renderable->type() == T::GetStaticType() )
                    _typedRenderables.push_back( dynamic_cast< T* >( renderable.get() ) );

            return _typedRenderables;
        }

        std::vector< CILight* > lights() const;
        bool hasLight( const std::string& name ) const;
        CILight* getLight( const std::string& name );

        std::vector< CDirectionalLight* > directionalLights() const;
        std::vector< CPointLight* > pointLights() const;
        std::vector< CSpotLight* > spotLights() const;

    private :

        std::unique_ptr< CFog > m_fog;
        std::unique_ptr< CSkybox > m_skybox;

        CICamera*                                       m_currentCamera;
        std::vector< std::unique_ptr< CICamera > >      m_cameras;
        std::unordered_map< std::string, CICamera* >    m_camerasMap;

        CILight*                                    m_mainLight;
        std::vector< std::unique_ptr< CILight > >   m_lights;
        std::map< std::string, CILight* >           m_lightsMap;
        std::vector< CDirectionalLight* >           m_directionalLights;
        std::vector< CPointLight* >                 m_pointLights;
        std::vector< CSpotLight* >                  m_spotLights;

        std::vector< std::unique_ptr< CIRenderable > >      m_renderables;
        std::unordered_map< std::string, CIRenderable* >    m_renderablesMap;
    };

}
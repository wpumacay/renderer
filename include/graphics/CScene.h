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

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// @note: We're returning raw pointers (*) instead of refs (&) to avoid inconsistencies, because so
//        far the user has to keep in mind two usage syntaxes: auto& (when returning refs), and auto
//        (when using pointers). The problem arises if the user uses auto instead of auto&, resulting
//        in a copy of the object (if has copy constructor),
//        An options could be to delete all copy|assign constructors and related operator to avoid
//        these issues at compile time (in case a plain 'auto' is used instead of 'auto&'), but for
//        now we'll skip to the following approach:
//        "Ownership is handled using smart pointers (almost always unique_ptr), and references are
//         passed via raw pointers (*), either as arguments to functions, or as references for the
//         user to use. The user|developer should keep this in mind: just avoid using using the given
//         raw pointer references as true owned objects (don't try to manage their lifetime)".
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace engine
{
    class CScene
    {
    public :

        CScene();
        ~CScene();

        CFog* SetFog( std::unique_ptr< CFog > fog );
        CSkybox* SetSkybox( std::unique_ptr< CSkybox > skybox );
        CICamera* AddCamera( std::unique_ptr< CICamera > camera );
        CILight* AddLight( std::unique_ptr< CILight > light );
        CIRenderable* AddRenderable( std::unique_ptr< CIRenderable > renderable );

        void ChangeCurrentCamera( const std::string& name );
        void ChangeCurrentLight( const std::string& name );

        void Update();
        void Resize( int32 width, int32 height );
        void RemoveAllRenderables();

        const CFog* GetCurrentFog() const;
        const CSkybox* GetCurrentSkybox() const;
        const CICamera* GetCurrentCamera() const;
        const CILight* GetCurrentLight() const;

        CFog* GetCurrentFog();
        CSkybox* GetCurrentSkybox();
        CICamera* GetCurrentCamera();
        CILight* GetCurrentLight();

        const CICamera* GetCameraByName( const std::string& name ) const;
        const CILight* GetLightByName( const std::string& name ) const;
        const CIRenderable* GetRenderableByName( const std::string& name ) const;

        CICamera* GetCameraByName( const std::string& name );
        CILight* GetLightByName( const std::string& name );
        CIRenderable* GetRenderableByName( const std::string& name );

        size_t GetNumCameras() const;
        size_t GetNumLights() const;
        size_t GetNumRenderables() const;

        bool HasCurrentFog() const;
        bool HasCurrentSkybox() const;
        bool HasCurrentCamera() const;
        bool HasCurrentLight() const;

        bool HasCameraNamed( const std::string& name ) const;
        bool HasLightNamed( const std::string& name ) const;
        bool HasRenderableNamed( const std::string& name ) const;

        std::vector<const CICamera*> GetCamerasList() const;
        std::vector<const CILight*> GetLightsList() const;
        std::vector<const CIRenderable*> GetRenderablesList() const;

        std::vector<CICamera*> GetCamerasList();
        std::vector<CILight*> GetLightsList();
        std::vector<CIRenderable*> GetRenderablesList();

    private :

        const CICamera* _get_camera( ssize_t index ) const;
        const CILight* _get_light( ssize_t index ) const;
        const CIRenderable* _get_renderable( ssize_t index ) const;

        CICamera* _get_mutable_camera( ssize_t index );
        CILight* _get_mutable_light( ssize_t index );
        CIRenderable* _get_mutable_renderable( ssize_t index );

    private :

        std::unique_ptr< CFog > m_fog;
        std::unique_ptr< CSkybox > m_skybox;

        std::vector< std::unique_ptr< CICamera > > m_cameras;
        std::vector< std::unique_ptr< CILight > > m_lights;
        std::vector< std::unique_ptr< CIRenderable > > m_renderables;

        ssize_t m_currentCameraIndex;
        ssize_t m_currentLightIndex;

        std::unordered_map< std::string, ssize_t > m_camerasMap;
        std::unordered_map< std::string, ssize_t > m_lightsMap;
        std::unordered_map< std::string, ssize_t > m_renderablesMap;
    };
}
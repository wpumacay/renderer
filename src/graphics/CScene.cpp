
#include <graphics/CScene.h>

namespace engine
{

    CScene::CScene()
    {
        m_fog = nullptr;
        m_skybox = nullptr;
        m_currentCamera = nullptr;
    }

    CScene::~CScene()
    {
        m_currentCamera = nullptr;

        m_camerasMap.clear();
        m_lightsMap.clear();
        m_directionalLights.clear();
        m_pointLights.clear();
        m_spotLights.clear();
    }

    void CScene::addFog( std::unique_ptr< CFog > fog )
    {
        if ( !fog )
            ENGINE_CORE_WARN( "Tried to add a null-fog to the scene" );
        else
            m_fog = std::move( fog );
    }

    void CScene::addSkybox( std::unique_ptr< CSkybox > skybox )
    {
        if ( !skybox )
            ENGINE_CORE_WARN( "Tried to add a null-skybox to the scene" );
        else
            m_skybox = std::move( skybox );
    }

    void CScene::addCamera( std::unique_ptr< CICamera > camera )
    {
        if ( !camera )
        {
            ENGINE_CORE_WARN( "Tried to add a null-camera to the scene" );
            return;
        }

        auto _cameraPtr = camera.get();

        // keep ownership of this camera
        m_cameras.push_back( std::move( camera ) );
        
        // keep access through its name
        m_camerasMap[_cameraPtr->name()] = _cameraPtr;
        // setup as current if there's no camera yet
        if ( !m_currentCamera )
            m_currentCamera = _cameraPtr;
    }

    void CScene::addLight( std::unique_ptr< CILight > light )
    {
        if ( !light )
        {
            ENGINE_CORE_WARN( "Tried to add a null-light to the scene" );
            return;
        }

        auto _lightPtr = light.get();

        // keep ownership of this light
        m_lights.push_back( std::move( light ) );

        // keep access through its name
        m_lightsMap[_lightPtr->name()] = _lightPtr;
        // setup as main if there's no lights yet
        if ( !m_mainLight )
            m_mainLight = _lightPtr;

        // keep access through its type
        if ( _lightPtr->type() == eLightType::DIRECTIONAL )
            m_directionalLights.push_back( dynamic_cast< CDirectionalLight* >( _lightPtr ) );

        else if ( _lightPtr->type() == eLightType::POINT )
            m_pointLights.push_back( dynamic_cast< CPointLight* >( _lightPtr ) );

        else if ( _lightPtr->type() == eLightType::SPOT )
            m_spotLights.push_back( dynamic_cast< CSpotLight* >( _lightPtr ) );
    }

    void CScene::addRenderable( std::unique_ptr< CIRenderable > renderable )
    {
        m_renderables.push_back( std::move( renderable ) );
    }

    void CScene::clearScene()
    {
        m_renderables.clear();
    }

    void CScene::changeToCamera( const std::string& name )
    {
        if ( m_camerasMap.find( name ) == m_camerasMap.end() )
            ENGINE_CORE_WARN( "Camera with name {0} couldn't be found", name );
        else
            m_currentCamera = m_camerasMap[name];
    }

    void CScene::changeMainLight( const std::string& name )
    {
        if ( m_lightsMap.find( name ) == m_lightsMap.end() )
            ENGINE_CORE_WARN( "Light with name {0} couldn't be found", name );
        else
            m_mainLight = m_lightsMap[name];
    }

    void CScene::update()
    {
        for ( auto& cameraPtr : m_cameras )
            cameraPtr->update();
    }

    void CScene::resize( int32 width, int32 height )
    {
        for ( auto& cameraPtr : m_cameras )
            cameraPtr->resize( width, height );
    }

    std::vector< CIRenderable* > CScene::renderables() const
    {
        std::vector< CIRenderable* > _renderablesPtrs;
        for ( auto& _renderable : m_renderables )
            _renderablesPtrs.push_back( _renderable.get() );

        return _renderablesPtrs;
    }

    std::vector< CICamera* > CScene::cameras() const
    {
        std::vector< CICamera* > _camerasPtrs;
        for ( auto& _camera : m_cameras )
            _camerasPtrs.push_back( _camera.get() );

        return _camerasPtrs;
    }

    std::vector< CILight* > CScene::lights() const
    {
        std::vector< CILight* > _lightsPtrs;
        for ( auto& _light : m_lights )
            _lightsPtrs.push_back( _light.get() );

        return _lightsPtrs;
    }

    std::vector< CDirectionalLight* > CScene::directionalLights() const
    {
        return m_directionalLights;
    }

    std::vector< CPointLight* > CScene::pointLights() const
    {
        return m_pointLights;
    }

    std::vector< CSpotLight* > CScene::spotLights() const
    {
        return m_spotLights;
    }

}
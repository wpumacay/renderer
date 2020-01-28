
#include <graphics/CScene.h>

namespace engine
{

    CScene::CScene()
    {
        m_fog = nullptr;
        m_skybox = nullptr;
        m_currentCamera = nullptr;
        m_mainLight = nullptr;
    }

    CScene::~CScene()
    {
        m_fog = nullptr;
        m_skybox = nullptr;
        m_currentCamera = nullptr;
        m_mainLight = nullptr;

        m_camerasMap.clear();
        m_lightsMap.clear();
        m_directionalLights.clear();
        m_pointLights.clear();
        m_spotLights.clear();
    }

    CFog* CScene::addFog( std::unique_ptr< CFog > fog )
    {
        if ( !fog )
            ENGINE_CORE_WARN( "Tried to add a null-fog to the scene" );
        else
            m_fog = std::move( fog );
        return m_fog.get();
    }

    CSkybox* CScene::addSkybox( std::unique_ptr< CSkybox > skybox )
    {
        if ( !skybox )
            ENGINE_CORE_WARN( "Tried to add a null-skybox to the scene" );
        else
            m_skybox = std::move( skybox );
        return m_skybox.get();
    }

    CICamera* CScene::addCamera( std::unique_ptr< CICamera > camera )
    {
        if ( !camera )
        {
            ENGINE_CORE_WARN( "Tried to add a null-camera to the scene" );
            return nullptr;
        }

        if ( m_camerasMap.find( camera->name() ) != m_camerasMap.end() )
            ENGINE_CORE_WARN( "Adding a camera with the same name. Deleting the previous one" );

        auto _cameraRef = camera.get();

        // keep ownership of this camera
        m_cameras.push_back( std::move( camera ) );
        
        // keep access through its name
        m_camerasMap[_cameraRef->name()] = _cameraRef;
        // setup as current if there's no camera yet
        if ( !m_currentCamera )
            m_currentCamera = _cameraRef;
        return _cameraRef;
    }

    CILight* CScene::addLight( std::unique_ptr< CILight > light )
    {
        if ( !light )
        {
            ENGINE_CORE_WARN( "Tried to add a null-light to the scene" );
            return nullptr;
        }

        if ( m_lightsMap.find( light->name() ) != m_lightsMap.end() )
            ENGINE_CORE_WARN( "Adding a light with the same name. Deleting the previous one" );

        auto _lightRef = light.get();

        // keep ownership of this light
        m_lights.push_back( std::move( light ) );

        // keep access through its name
        m_lightsMap[_lightRef->name()] = _lightRef;
        // setup as main if there's no lights yet
        if ( !m_mainLight )
            m_mainLight = _lightRef;

        // keep access through its type
        if ( _lightRef->type() == eLightType::DIRECTIONAL )
            m_directionalLights.push_back( dynamic_cast< CDirectionalLight* >( _lightRef ) );

        else if ( _lightRef->type() == eLightType::POINT )
            m_pointLights.push_back( dynamic_cast< CPointLight* >( _lightRef ) );

        else if ( _lightRef->type() == eLightType::SPOT )
            m_spotLights.push_back( dynamic_cast< CSpotLight* >( _lightRef ) );

        return _lightRef;
    }

    CIRenderable* CScene::addRenderable( std::unique_ptr< CIRenderable > renderable )
    {
        if ( m_renderablesMap.find( renderable->name() ) != m_renderablesMap.end() )
            ENGINE_CORE_WARN( "Adding a renderable with the same name. Deleting the previous one" );

        m_renderables.push_back( std::move( renderable ) );
        m_renderablesMap[ m_renderables.back()->name() ] = m_renderables.back().get();

        return m_renderables.back().get();
    }

    void CScene::clearScene()
    {
        m_renderables.clear();
        m_renderablesMap.clear();
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
        if ( m_currentCamera )
            m_currentCamera->update();
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

    bool CScene::hasRenderable( const std::string& name ) const
    {
        return m_renderablesMap.find( name ) != m_renderablesMap.end();
    }

    CIRenderable* CScene::getRenderable( const std::string& name )
    {
        if ( m_renderablesMap.find( name ) == m_renderablesMap.end() )
        {
            ENGINE_CORE_ERROR( "Couldn't find renderable with name {0} in scene", name );
            return nullptr;
        }

        return m_renderablesMap[name];
    }

    std::vector< CICamera* > CScene::cameras() const
    {
        std::vector< CICamera* > _camerasPtrs;
        for ( auto& _camera : m_cameras )
            _camerasPtrs.push_back( _camera.get() );

        return _camerasPtrs;
    }

    bool CScene::hasCamera( const std::string& name ) const
    {
        return m_camerasMap.find( name ) != m_camerasMap.end();
    }

    CICamera* CScene::getCamera( const std::string& name )
    {
        if ( m_camerasMap.find( name ) == m_camerasMap.end() )
        {
            ENGINE_CORE_ERROR( "Couldn't find camera with name {0} in scene", name );
            return nullptr;
        }

        return m_camerasMap[name];
    }

    std::vector< CILight* > CScene::lights() const
    {
        std::vector< CILight* > _lightsPtrs;
        for ( auto& _light : m_lights )
            _lightsPtrs.push_back( _light.get() );

        return _lightsPtrs;
    }

    bool CScene::hasLight( const std::string& name ) const
    {
        return m_lightsMap.find( name ) != m_lightsMap.end();
    }

    CILight* CScene::getLight( const std::string& name )
    {
        if ( m_lightsMap.find( name ) == m_lightsMap.end() )
        {
            ENGINE_CORE_ERROR( "Couldn't find light with name {0} in scene", name );
            return nullptr;
        }

        return m_lightsMap[name];
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
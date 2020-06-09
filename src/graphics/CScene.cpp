
#include <graphics/CScene.h>

namespace engine
{
    CScene::CScene()
    {
        m_fog = nullptr;
        m_skybox = nullptr;
        m_currentCameraIndex = -1;
        m_currentLightIndex = -1;
    }

    CScene::~CScene()
    {
        m_fog = nullptr;
        m_skybox = nullptr;
        m_cameras.clear();
        m_lights.clear();
        m_renderables.clear();
    }

    CFog* CScene::SetFog( std::unique_ptr< CFog > fog )
    {
        ENGINE_CORE_ASSERT( fog, "CScene::SetFog >>> Tried to add a null fog to the scene" );

        m_fog = std::move( fog );
        return m_fog.get();
    }

    CSkybox* CScene::SetSkybox( std::unique_ptr< CSkybox > skybox )
    {
        ENGINE_CORE_ASSERT( skybox, "CScene::SetSkybox >>> Tried to add a null skybox to the scene" );

        m_skybox = std::move( skybox );
        return m_skybox.get();
    }

    CICamera* CScene::AddCamera( std::unique_ptr< CICamera > camera )
    {
        ENGINE_CORE_ASSERT( camera, "CScene::AddCamera >>> Tried to add a null camera to the scene" );

        if ( HasCameraNamed( camera->name() ) )
            ENGINE_CORE_WARN( "CScene::AddCamera >>> Adding a camera with the same name {0}. Deleting \
                               the previous one", camera->name() );

        m_camerasMap[camera->name()] = m_cameras.size();
        m_cameras.push_back( std::move( camera ) );

        // Set as current camera if there's no camera yet
        if ( m_currentCameraIndex < 0 )
            m_currentCameraIndex = m_cameras.size() - 1;
        return m_cameras.back().get();
    }

    CILight* CScene::AddLight( std::unique_ptr< CILight > light )
    {
        ENGINE_CORE_ASSERT( light, "CScene::AddLight >>> Tried to add a null light to the scene" );

        if ( HasLightNamed( light->name() ) )
            ENGINE_CORE_WARN( "CScene::AddLight >>> Adding a light with the same name {0}. Deleting \
                               the previous one", light->name() );

        m_lightsMap[light->name()] = m_lights.size();
        m_lights.push_back( std::move( light ) );

        if ( m_currentLightIndex < 0 )
            m_currentLightIndex = m_lights.size() - 1;
        return m_lights.back().get();
    }

    CIRenderable* CScene::AddRenderable( std::unique_ptr< CIRenderable > renderable )
    {
        ENGINE_CORE_ASSERT( renderable, "CScene::AddRenderable >>> Tried to add a null renderable to the scene" );

        if ( HasRenderableNamed( renderable->name() ) )
            ENGINE_CORE_WARN( "CScene::AddRenderable >>> Adding a renderable with the same name {0}. Deleting \
                               the previous one", renderable->name() );

        m_renderablesMap[renderable->name()] = m_renderables.size();
        m_renderables.push_back( std::move( renderable ) );

        return m_renderables.back().get();
    }

    void CScene::ChangeCurrentCamera( const std::string& name )
    {
        if ( !HasCameraNamed( name ) )
            ENGINE_CORE_WARN( "CScene::ChangeCurrentCamera >>> Camera with name {0} couldn't be found", name );
        else
            m_currentCameraIndex = m_camerasMap[name];
    }

    void CScene::ChangeCurrentLight( const std::string& name )
    {
        if ( !HasLightNamed( name ) )
            ENGINE_CORE_WARN( "CScene::ChangeCurrentLight >>> Light with name {0} couldn't be found", name );
        else
            m_currentLightIndex = m_lightsMap[name];
    }

    void CScene::Update()
    {
        if ( m_currentCameraIndex > -1 )
            m_cameras[m_currentCameraIndex]->Update();
    }

    void CScene::Resize( int32 width, int32 height )
    {
        for ( auto& camera : m_cameras )
            camera->Resize( width, height );
    }

    void CScene::RemoveAllRenderables()
    {
        m_renderables.clear();
        m_renderablesMap.clear();
    }

    const CFog* CScene::GetCurrentFog() const
    {
        ENGINE_CORE_ASSERT( m_fog, "CScene::GetCurrentFog >>> Tried to grab null fog reference" );
        return m_fog.get();
    }

    const CSkybox* CScene::GetCurrentSkybox() const
    {
        ENGINE_CORE_ASSERT( m_skybox, "CScene::GetCurrentSkybox >>> Tried to grab null skybox reference" );
        return m_skybox.get();
    }

    const CICamera* CScene::GetCurrentCamera() const
    {
        ENGINE_CORE_ASSERT( m_currentCameraIndex > -1, "CScene::GetCurrentCamera >>> There's no current camera at the moment" );
        return m_cameras[m_currentCameraIndex].get();
    }

    const CILight* CScene::GetCurrentLight() const
    {
        ENGINE_CORE_ASSERT( m_currentLightIndex > -1, "CScene::GetCurrentLight >>> There's no current light at the moment" );
        return m_lights[m_currentLightIndex].get();
    }

    CFog* CScene::GetCurrentFog()
    {
        ENGINE_CORE_ASSERT( m_fog, "CScene::GetCurrentFog >>> Tried to grab null fog reference" );
        return m_fog.get();
    }

    CSkybox* CScene::GetCurrentSkybox()
    {
        ENGINE_CORE_ASSERT( m_skybox, "CScene::GetCurrentSkybox >>> Tried to grab null skybox reference" );
        return m_skybox.get();
    }

    CICamera* CScene::GetCurrentCamera()
    {
        ENGINE_CORE_ASSERT( m_currentCameraIndex > -1, "CScene::GetCurrentCamera >>> There's no current camera at the moment" );
        return m_cameras[m_currentCameraIndex].get();
    }

    CILight* CScene::GetCurrentLight()
    {
        ENGINE_CORE_ASSERT( m_currentLightIndex > -1, "CScene::GetCurrentLight >>> There's no current light at the moment" );
        return m_lights[m_currentLightIndex].get();
    }

    const CICamera* CScene::GetCameraByName( const std::string& name ) const
    {
        ENGINE_CORE_ASSERT( HasCameraNamed( name ), "CScene::GetCameraByName >>> camera with name {0} not found", name );
        return _get_camera( m_camerasMap.at( name ) );
    }

    const CILight* CScene::GetLightByName( const std::string& name ) const
    {
        ENGINE_CORE_ASSERT( HasLightNamed( name ), "CScene::GetLightByName >>> camera with name {0} not found", name );
        return _get_light( m_lightsMap.at( name ) );
    }

    const CIRenderable* CScene::GetRenderableByName( const std::string& name ) const
    {
        ENGINE_CORE_ASSERT( HasRenderableNamed( name ), "CScene::GetRenderableByName >>> camera with name {0} not found", name );
        return _get_renderable( m_renderablesMap.at( name ) );
    }

    CICamera* CScene::GetCameraByName( const std::string& name )
    {
        ENGINE_CORE_ASSERT( HasCameraNamed( name ), "CScene::GetCameraByName >>> camera with name {0} not found", name );
        return _get_mutable_camera( m_camerasMap[name] );
    }

    CILight* CScene::GetLightByName( const std::string& name )
    {
        ENGINE_CORE_ASSERT( HasLightNamed( name ), "CScene::GetLightByName >>> camera with name {0} not found", name );
        return _get_mutable_light( m_lightsMap[name] );
    }

    CIRenderable* CScene::GetRenderableByName( const std::string& name )
    {
        ENGINE_CORE_ASSERT( HasRenderableNamed( name ), "CScene::GetRenderableByName >>> camera with name {0} not found", name );
        return _get_mutable_renderable( m_renderablesMap[name] );
    }

    size_t CScene::GetNumCameras() const
    {
        return m_cameras.size();
    }
    size_t CScene::GetNumRenderables() const
    {
        return m_renderables.size();
    }
    size_t CScene::GetNumLights() const
    {
        return m_lights.size();
    }

    bool CScene::HasCurrentFog() const
    {
        return m_fog != nullptr;
    }

    bool CScene::HasCurrentSkybox() const
    {
        return m_skybox != nullptr;
    }

    bool CScene::HasCurrentCamera() const
    {
        return m_currentCameraIndex > -1;
    }

    bool CScene::HasCurrentLight() const
    {
        return m_currentLightIndex > -1;
    }

    bool CScene::HasCameraNamed( const std::string& name ) const
    {
        return m_camerasMap.find( name ) != m_camerasMap.end();
    }

    bool CScene::HasLightNamed( const std::string& name ) const
    {
        return m_lightsMap.find( name ) != m_lightsMap.end();
    }

    bool CScene::HasRenderableNamed( const std::string& name ) const
    {
        return m_renderablesMap.find( name ) != m_renderablesMap.end();
    }

    std::vector<const CICamera*> CScene::GetCamerasList() const
    {
        std::vector<const CICamera*> _camerasList;
        for ( auto& camera : m_cameras )
            _camerasList.push_back( camera.get() );
        return _camerasList;
    }

    std::vector<const CILight*> CScene::GetLightsList() const
    {
        std::vector<const CILight*> _lightsList;
        for ( auto& light : m_lights )
            _lightsList.push_back( light.get() );
        return _lightsList;
    }

    std::vector<const CIRenderable* > CScene::GetRenderablesList() const
    {
        std::vector<const CIRenderable*> _renderablesList;
        for ( auto& renderable : m_renderables )
            _renderablesList.push_back( renderable.get() );
        return _renderablesList;
    }

    std::vector<CICamera*> CScene::GetCamerasList()
    {
        std::vector<CICamera*> _camerasList;
        for ( auto& camera : m_cameras )
            _camerasList.push_back( camera.get() );
        return _camerasList;
    }

    std::vector<CILight*> CScene::GetLightsList()
    {
        std::vector<CILight*> _lightsList;
        for ( auto& light : m_lights )
            _lightsList.push_back( light.get() );
        return _lightsList;
    }

    std::vector<CIRenderable* > CScene::GetRenderablesList()
    {
        std::vector<CIRenderable*> _renderablesList;
        for ( auto& renderable : m_renderables )
            _renderablesList.push_back( renderable.get() );
        return _renderablesList;
    }

    const CICamera* CScene::_get_camera( ssize_t index ) const
    {
        ENGINE_CORE_ASSERT( ( index >= 0 && index < m_cameras.size() ), "CScene::_get_camera >>> index {0} \
                            out of range [0,...,{1}]", index, m_cameras.size() - 1 );

        return m_cameras[index].get();
    }

    const CILight* CScene::_get_light( ssize_t index ) const
    {
        ENGINE_CORE_ASSERT( ( index >= 0 && index < m_lights.size() ), "CScene::_get_light >>> index {0} \
                            out of range [0,...,{1}]", index, m_lights.size() - 1 );

        return m_lights[index].get();
    }

    const CIRenderable* CScene::_get_renderable( ssize_t index ) const
    {
        ENGINE_CORE_ASSERT( ( index >= 0 && index < m_renderables.size() ), "CScene::_get_renderable >>> index {0} \
                            out of range [0,...,{1}]", index, m_renderables.size() - 1 );

        return m_renderables[index].get();
    }

    CICamera* CScene::_get_mutable_camera( ssize_t index )
    {
        ENGINE_CORE_ASSERT( ( index >= 0 && index < m_cameras.size() ), "CScene::_get_mutable_camera >>> index {0} \
                            out of range [0,...,{1}]", index, m_cameras.size() - 1 );

        return m_cameras[index].get();
    }

    CILight* CScene::_get_mutable_light( ssize_t index )
    {
        ENGINE_CORE_ASSERT( ( index >= 0 && index < m_lights.size() ), "CScene::_get_mutable_light >>> index {0} \
                            out of range [0,...,{1}]", index, m_lights.size() - 1 );

        return m_lights[index].get();
    }

    CIRenderable* CScene::_get_mutable_renderable( ssize_t index )
    {
        ENGINE_CORE_ASSERT( ( index >= 0 && index < m_renderables.size() ), "CScene::_get_mutable_renderable >>> index {0} \
                            out of range [0,...,{1}]", index, m_renderables.size() - 1 );

        return m_renderables[index].get();
    }
}
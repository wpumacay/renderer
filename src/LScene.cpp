
#include <LScene.h>

using namespace std;

namespace engine
{

    LScene::LScene()
    {
        m_currentCamera = nullptr;
        m_fog = nullptr;
        m_skybox = nullptr;
    }

    LScene::~LScene()
    {
        for ( size_t i = 0; i < m_renderables.size(); i++ )
        {
            delete m_renderables[i];
        }
        m_renderables.clear();

        map<string, CICamera*>::iterator _it;
        for ( _it = m_cameras.begin(); _it != m_cameras.end(); _it++ )
        {
            delete _it->second;
        }
        m_cameras.clear();

        for ( CILight* _light : m_lights )
        {
            delete _light;
        }
        m_lights.clear();

        if ( m_fog != nullptr )
        {
            delete m_fog;
            m_fog = nullptr;
        }

        if ( m_skybox != nullptr )
        {
            delete m_skybox;
            m_skybox = nullptr;
        }

        m_currentCamera = nullptr;
    }

    void LScene::update()
    {
        map<string, CICamera*>::iterator _it;
        for ( _it = m_cameras.begin(); _it != m_cameras.end(); _it++ )
        {
            auto _camera = _it->second;
            if ( _camera != nullptr )
                _camera->update();
        }
    }

    void LScene::resize( int width, int height )
    {
        for ( auto it : m_cameras )
            if ( it.second )
                it.second->resize( width, height );
    }

    void LScene::addRenderable( LIRenderable* pRenderable )
    {
        if ( pRenderable )
        {
            m_renderables.push_back( pRenderable );
        }
        else
        {
            std::cout << "WARNING> Tried to pass a nullptr reference" << std::endl;
        }
    }

    void LScene::addFog( LFog* pFog )
    {
        if ( m_fog != nullptr )
        {
            delete m_fog;
            std::cout << "LOG> Deleting current fog in the current scene" << std::endl;
        }

        m_fog = pFog;
    }
    
    void LScene::addLight( CILight* pLight )
    {
        m_lights.push_back( pLight );

        if ( pLight->type() == eLightType::DIRECTIONAL ) 
            m_directionalLights.push_back( (CDirectionalLight*) pLight );

        if ( pLight->type() == eLightType::POINT ) 
            m_pointLights.push_back( (CPointLight*) pLight );

        if ( pLight->type() == eLightType::SPOT ) 
            m_spotLights.push_back( (CSpotLight*) pLight );
    }

    void LScene::addCamera( CICamera* pCamera )
    {
        m_cameras[ pCamera->name() ] = pCamera;

        if ( !m_currentCamera )
        {
            m_currentCamera = pCamera;
            m_currentCamera->setActiveMode( true );
        }
    }

    void LScene::changeToCameraByName( const string& cameraId )
    {
        if ( m_cameras.find( cameraId ) == m_cameras.end() )
        {
            std::cout << "WARNING> Tried to change to a non-existent camera" << std::endl;
            return;
        }

        m_currentCamera = m_cameras[ cameraId ];
    }
    
    void LScene::cleanScene()
    {
        for ( size_t i = 0; i < m_renderables.size(); i++ )
            delete m_renderables[i];

        m_renderables.clear();
    }

    void LScene::addSkybox( LSkybox* pSkybox )
    {   
        if ( m_skybox != nullptr )
        {
            delete m_skybox;
            std::cout << "LOG> Removing previous skybox from current scene" << std::endl;
        }
        m_skybox = pSkybox;
    }

    LFog* LScene::getFog() const
    {
        return m_fog;
    }

    LSkybox* LScene::getSkybox() const
    {
        return m_skybox;
    }

    vector< LIRenderable* > LScene::getRenderables() const
    {
        return m_renderables;
    }

}


#include <LApp.h>
#include <LScene.h>
#include <LFixedCamera3d.h>

using namespace std;

namespace engine
{


    LScene::LScene()
    {
        m_currentCamera = NULL;
        m_fog = NULL;
        m_skybox = NULL;
    }

    LScene::~LScene()
    {
        for ( size_t i = 0; i < m_renderables.size(); i++ )
        {
            delete m_renderables[i];
        }
        m_renderables.clear();

        map<string, LICamera*>::iterator _it;
        for ( _it = m_cameras.begin(); _it != m_cameras.end(); _it++ )
        {
            delete _it->second;
        }
        m_cameras.clear();

        for ( LILight* _light : m_lights )
        {
            delete _light;
        }
        m_lights.clear();

        if ( m_fog != NULL )
        {
            delete m_fog;
            m_fog = NULL;
        }

        if ( m_skybox != NULL )
        {
            delete m_skybox;
            m_skybox = NULL;
        }

        m_currentCamera = NULL;
    }

    void LScene::update( float dt )
    {
        map<string, LICamera*>::iterator _it;
        for ( _it = m_cameras.begin(); _it != m_cameras.end(); _it++ )
        {
            auto _camera = _it->second;
            if ( _camera != NULL )
            {
                _camera->update( dt );
            }
        }
    }

    void LScene::addRenderable( LIRenderable* pRenderable )
    {
        if ( pRenderable )
        {
            m_renderables.push_back( pRenderable );
        }
        else
        {
            std::cout << "WARNING> Tried to pass a NULL reference" << std::endl;
        }
    }

    void LScene::addFog( LFog* pFog )
    {
        if ( m_fog != NULL )
        {
            delete m_fog;
			std::cout << "LOG> Deleting current fog in the current scene" << std::endl;
        }

        m_fog = pFog;
    }
	
	void LScene::addLight( LILight* pLight )
	{
		m_lights.push_back( pLight );
	}

	void LScene::addCamera( LICamera* pCamera )
	{
		m_cameras[ pCamera->name() ] = pCamera;
		if ( m_currentCamera == NULL )
		{
			m_currentCamera = pCamera;

            _checkCameraType();
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

        _checkCameraType();
    }
	
    void LScene::_checkCameraType()
    {
        // check the type of camera, so that we can toggle the cursor mode
        auto _window = LApp::GetInstance()->window();
        if ( m_currentCamera->type() == LFpsCamera::GetStaticType() )
        {
            // std::cout << "INFO> Disabling cursor for fps camera" << std::endl;
            _window->disableCursor();
        }
        else
        {
            // std::cout << "INFO> Enabling cursor for non-fps camera" << std::endl;
            _window->enableCursor();
        }
    }

	void LScene::addSkybox( LSkybox* pSkybox )
	{	
		if ( m_skybox != NULL )
		{
			delete m_skybox;
			std::cout << "LOG> Removing previous skybox from current scene" << std::endl;
		}
		m_skybox = pSkybox;
	}

	LFog* LScene::getFog()
	{
		return m_fog;
	}

	LSkybox* LScene::getSkybox()
	{
		return m_skybox;
	}

    vector< LIRenderable* > LScene::getRenderables()
    {
        return m_renderables;
    }

}






#include "LScene.h"
#include "LFixedCamera3d.h"

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
        for ( LICamera* _cam : m_cameras )
        {
            delete _cam;
        }

        for ( LILight* _light : m_lights )
        {
            delete _light;
        }

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
        for ( LICamera* _camera : m_cameras )
        {
            _camera->update( dt );
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
		m_cameras.push_back( pCamera );
		if ( m_currentCamera == NULL )
		{
			m_currentCamera = pCamera;
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


}





#pragma once

#include "LCommon.h"
#include "LILight.h"
#include "LLightDirectional.h"
#include "LLightDirectional.h"
#include "LLightDirectional.h"
#include "LEntity.h"
#include "LICamera.h"
#include "LFog.h"
#include "LTerrainGenerator.h"
#include "LSkybox.h"

using namespace std;

// TODO : Decouple the terrain rendering from the scene rendering
// Maybe use unity-like way of using gameobjects -> onStart and entities ...
// and make the terrain generator and some other stuff game objects

namespace engine
{

    class LTerrainGenerator;

    class LScene
    {

        protected :

        vector<LILight*> m_lights;
        vector<LEntity*> m_entities;
        vector<LICamera*> m_cameras;

        LICamera* m_currentCamera;

        glm::mat4 m_projMatrix;

        LFog* m_fog;

        LSkybox* m_skybox;

        LTerrainGenerator* m_terrainGenerator;

        public :


        LScene();
        ~LScene();

        void addLight( LILight* pLight ) { m_lights.push_back( pLight ); }
        void addFog( LFog* pFog );
        void addEntity( LEntity* pEntity ) { m_entities.push_back( pEntity ); }
        void addCamera( LICamera* pCamera ) 
        { 
            m_cameras.push_back( pCamera );
            if ( m_currentCamera == NULL )
            {
                m_currentCamera = pCamera;
            }
        }

        void addTerrainGenerator( LTerrainGenerator* pTerrainGenerator )
        {
            if ( m_terrainGenerator != NULL )
            {
                delete m_terrainGenerator;
            }

            m_terrainGenerator = pTerrainGenerator;
        }

        LTerrainGenerator* getTerrainGenerator() { return m_terrainGenerator; }

        glm::mat4 getProjMatrix() { return m_projMatrix; }
        LICamera* getCurrentCamera() { return m_currentCamera; }
        vector<LILight*>& getLights() { return m_lights; }
        vector<LEntity*>& getEntities() { return m_entities; }

        template< class T >
        vector<T*> getLights()
        {
            auto _type = T::getStaticType();

            vector<T*> _lights;

            for ( LILight* _light : m_lights )
            {
                if ( _light->getType() == _type )
                {
                    _lights.push_back( ( T* ) _light );
                }
            }

            return _lights;
        }

        LFog* getFog() { return m_fog; }

        void addSkybox( LSkybox* pSkybox )
        {
            if ( m_skybox != NULL )
            {
                delete m_skybox;
            }

            m_skybox = pSkybox;
        }

        LSkybox* getSkybox()
        {
            return m_skybox;
        }

        virtual void update( float dt );

    };





}
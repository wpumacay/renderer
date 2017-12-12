
#pragma once

#include "LCommon.h"
#include "LILight.h"
#include "LEntity.h"
#include "LICamera.h"

using namespace std;

namespace engine
{


    class LScene
    {

        protected :

        vector<LILight*> m_lights;
        vector<LEntity*> m_entities;
        vector<LICamera*> m_cameras;

        LICamera* m_currentCamera;

        glm::mat4 m_projMatrix;

        public :


        LScene();
        ~LScene();

        void addLight( LILight* pLight ) { m_lights.push_back( pLight ); }
        void addEntity( LEntity* pEntity ) { m_entities.push_back( pEntity ); }
        void addCamera( LICamera* pCamera ) 
        { 
            m_cameras.push_back( pCamera );
            if ( m_currentCamera == NULL )
            {
                m_currentCamera = pCamera;
            }
        }


        void enableLighting();
        void disableLighting();

        glm::mat4 getProjMatrix() { return m_projMatrix; }
        LICamera* getCurrentCamera() { return m_currentCamera; }
        vector<LILight*>& getLights() { return m_lights; }
        vector<LEntity*>& getEntities() { return m_entities; }

        void update( float dt );

        void setShadingMode( bool useSmoothShading );

    };





}
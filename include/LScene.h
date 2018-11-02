
#pragma once

#include "LCommon.h"
#include "LILight.h"
#include "LLightDirectional.h"
#include "LLightDirectional.h"
#include "LLightDirectional.h"
#include "LICamera.h"
#include "LFog.h"
#include "LSkybox.h"
#include "LIRenderable.h"

using namespace std;

namespace engine
{

    class LScene
    {

        protected :

        vector<LIRenderable*> m_renderables;
        vector<LILight*> m_lights;
        map<string, LICamera*> m_cameras;
        LFog* m_fog;
        LSkybox* m_skybox;
        
		LICamera* m_currentCamera;

        LMat4 m_projMatrix;

        void _checkCameraType();

        public :


        LScene();
        ~LScene();

        void addRenderable( LIRenderable* pRenderable );
        void addFog( LFog* pFog );
        void addLight( LILight* pLight ); 
		void addCamera( LICamera* pCamera );
        void addSkybox( LSkybox* pSkybox );

        void changeToCameraByName( const string& cameraId );

        LMat4 getProjMatrix() { return m_projMatrix; }
        LICamera* getCurrentCamera() { return m_currentCamera; }
        vector<LILight*>& getLights() { return m_lights; }

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

        LFog* getFog();
        LSkybox* getSkybox();
        vector<LIRenderable*> getRenderables();

        virtual void update( float dt );

    };





}

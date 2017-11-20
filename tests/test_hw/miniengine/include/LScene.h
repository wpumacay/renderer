
#pragma once

#include "../LMiniCommon.h"

#include "LMesh.h"
#include "LLightSource.h"
#include "LCamera3D.h"

using namespace std;

namespace miniengine
{


    class LScene
    {

        protected :

        vector<LMesh*> m_meshes;
        vector<LLightSource*> m_lights;

        LCamera3D* m_camera;

        public :

        LScene();
        ~LScene();

        void addObject( LMesh* pObject );
        void addLight( LLightSource* pLight );

        void update( float dt );
        void render();

        vector<LLightSource*> lights() { return m_lights; }

        virtual void onKeyDown( int pKey )
        {
            if ( m_camera != NULL )
            {
                m_camera->onKeyDown( pKey );
            }
        }

        virtual void onKeyUp( int pKey )
        {
            if ( m_camera != NULL )
            {
                m_camera->onKeyUp( pKey );
            }
        }

        virtual void onMouseDown( float x, float y )
        {
            if ( m_camera != NULL )
            {
                m_camera->onMouseDown( x, y );
            }
        }

        virtual void onMouseUp( float x, float y )
        {
            if ( m_camera != NULL )
            {
                m_camera->onMouseUp( x, y );
            }
        }

        virtual void onMouseMove( double x, double y )
        {
            if ( m_camera != NULL )
            {
                m_camera->onMouseMove( x, y );
            }
        }

        virtual void onMouseScroll( double xOff, double yOff )
        {
            if ( m_camera != NULL )
            {
                m_camera->fov -= yOff;
                m_camera->fov = min( max( m_camera->fov, 1.0f ), 45.0f );
            }
        }

    };











}
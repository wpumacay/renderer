
#pragma once

#include "../LMiniCommon.h"

#include "LMeshBuilder.h"

#define LIGHT_DEFAULT_AMBIENT  LVec3( 0.2f, 0.2f, 0.2f )
#define LIGHT_DEFAULT_DIFFUSE  LVec3( 0.5f, 0.5f, 0.5f )
#define LIGHT_DEFAULT_SPECULAR LVec3( 0.2f, 0.2f, 0.2f )

namespace miniengine
{

    namespace lightType
    {
        enum
        {

        };
    }


    struct LLightSource
    {

        protected :

        LVec3 m_pos;
        LVec3 m_dir;

        LMesh* m_lamp;

        public :

        LVec3 ambient;
        LVec3 diffuse;
        LVec3 specular;


        LLightSource()
        {
            ambient  = LIGHT_DEFAULT_AMBIENT;
            diffuse  = LIGHT_DEFAULT_DIFFUSE;
            specular = LIGHT_DEFAULT_SPECULAR;

            LBuildParams _params;
            _params.b_width  = 0.25f;
            _params.b_height = 0.25f;
            _params.b_depth  = 0.25f;

            m_lamp = LMeshBuilder::createMeshObject( meshType::BOX,
                                                     _params );
            m_lamp->disableLighting();
            m_lamp->material().ambient = LVec3( 1.0f, 1.0f, 1.0f );
        }

        LLightSource( const LVec3& pos )
        {
            ambient  = LIGHT_DEFAULT_AMBIENT;
            diffuse  = LIGHT_DEFAULT_DIFFUSE;
            specular = LIGHT_DEFAULT_SPECULAR;

            m_pos = pos;

            LBuildParams _params;
            _params.b_width  = 0.25f;
            _params.b_height = 0.25f;
            _params.b_depth  = 0.25f;

            m_lamp = LMeshBuilder::createMeshObject( meshType::BOX,
                                                     _params );
            m_lamp->pos = pos;
            m_lamp->disableLighting();
            m_lamp->material().ambient = LVec3( 1.0f, 1.0f, 1.0f );
        }

        ~LLightSource()
        {
            if ( m_lamp != NULL )
            {
                delete m_lamp;
                m_lamp = NULL;            
            }
        }

        void setPosition( const LVec3& pos )
        {
            m_pos = pos;
            m_lamp->pos = pos;
        }

        LVec3 getPosition() { return m_pos; }
        LVec3 getDirection() { return m_dir; }
        LMesh* getLamp() { return m_lamp; }

    };


}
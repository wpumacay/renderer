

#pragma once

#include <shaders/LShaderBasic3d.h>
#include <LLightDirectional.h>
#include <LMaterial.h>

using namespace std;

namespace engine
{

    class LShaderEntitiesLightingShadows : public LShaderBasic3d
    {

        protected :

        LLDirUniforms m_ulDir;
        LMaterialUniforms m_uMaterial;
        GLuint m_uGlobalAmbient;
        GLuint m_uViewPos;

        GLuint m_uLightSpaceViewMatrix;
        GLuint m_uLightSpaceProjMatrix;
        GLuint m_uLightPos;

        public :

        LShaderEntitiesLightingShadows( GLuint id ) : LShaderBasic3d( id )
        {
            bind();

            m_uGlobalAmbient    = glGetUniformLocation( m_id, "u_globalAmbientLight" );
            m_uViewPos          = glGetUniformLocation( m_id, "u_viewPos" );

            m_uMaterial.ambient   = glGetUniformLocation( m_id, "u_material.ambient" );
            m_uMaterial.diffuse   = glGetUniformLocation( m_id, "u_material.diffuse" );
            m_uMaterial.specular  = glGetUniformLocation( m_id, "u_material.specular" );
            m_uMaterial.shininess = glGetUniformLocation( m_id, "u_material.shininess" );

            m_ulDir.ambient     = glGetUniformLocation( m_id, "u_directionalLight.ambient" );
            m_ulDir.diffuse     = glGetUniformLocation( m_id, "u_directionalLight.diffuse" );
            m_ulDir.specular    = glGetUniformLocation( m_id, "u_directionalLight.specular" );
            m_ulDir.direction   = glGetUniformLocation( m_id, "u_directionalLight.direction" );

            m_uLightSpaceViewMatrix = glGetUniformLocation( m_id, "u_tLightSpaceViewMatrix" );
            m_uLightSpaceProjMatrix = glGetUniformLocation( m_id, "u_tLightSpaceProjMatrix" );
            m_uLightPos = glGetUniformLocation( m_id, "u_lightPos" );

            unbind();
        }

        void setLightDirectional( LLightDirectional* pLightDirectional )
        {
            _setVec3( m_ulDir.ambient, pLightDirectional->ambient );
            _setVec3( m_ulDir.diffuse, pLightDirectional->diffuse );
            _setVec3( m_ulDir.specular, pLightDirectional->specular );
            _setVec3( m_ulDir.direction, pLightDirectional->direction );
            _setInt( m_ulDir.isActive, pLightDirectional->isActive );
        }

        void setGlobalAmbientLight( const LVec3& vec )
        {
            _setVec3( m_uGlobalAmbient, vec );
        }

        void setViewPosition( const LVec3& vec )
        {
            _setVec3( m_uViewPos, vec );
        }

        void setMaterial( LMaterial* pMaterial )
        {
            _setVec3( m_uMaterial.ambient, pMaterial->ambient );
            _setVec3( m_uMaterial.diffuse, pMaterial->diffuse );
            _setVec3( m_uMaterial.specular, pMaterial->specular );
            _setFloat( m_uMaterial.shininess, pMaterial->shininess );
        }

        void setLightSpaceViewMatrix( const LMat4& mat )
        {
            _setMat4( m_uLightSpaceViewMatrix, mat );
        }

        void setLightSpaceProjectionMatrix( const LMat4& mat )
        {
            _setMat4( m_uLightSpaceProjMatrix, mat );
        }

        void setLightPosition( const LVec3& vec )
        {
            _setVec3( m_uLightPos, vec );
        }
    };



}
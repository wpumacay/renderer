
#pragma once

#include <shaders/LShader.h>

using namespace std;

namespace engine
{

    struct LUniformsDirLight
    {
        GLuint direction;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
    };

    struct LUniformsMaterial
    {
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint shininess;
    };

    class LShaderTerrainPatch : public LShader
    {
        protected :

        GLuint m_uView;
        GLuint m_uProj;

        LUniformsDirLight m_ulDir[2];
        GLuint m_uNumLightsDir;

        LUniformsMaterial m_uMaterial[7];

        GLuint m_uGlobalAmbientLight;
        GLuint m_uViewPos;

        GLuint m_uTextures[7];
        GLuint m_uTerrainVariation;

        public :

        LShaderTerrainPatch( GLuint id ) : LShader( id )
        {
            bind();

            // Load uniforms
            m_uView  = glGetUniformLocation( m_id, "u_tView" );
            m_uProj  = glGetUniformLocation( m_id, "u_tProj" );

            m_uGlobalAmbientLight = glGetUniformLocation( m_id, "u_globalAmbientLight" );
            m_uViewPos = glGetUniformLocation( m_id, "u_viewPos" );

            m_uNumLightsDir = glGetUniformLocation( m_id, "u_numDirectionalLights" );

            m_uTerrainVariation = glGetUniformLocation( m_id, "u_variation" );

            string _u_variant;

            for ( int q = 0; q < 2; q++ )
            {
                string _u_location = "u_directionalLight";
                _u_location += "[";
                _u_location += to_string( q );
                _u_location += "]";

                _u_variant = _u_location + ".ambient";
                m_ulDir[q].ambient = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".diffuse";
                m_ulDir[q].diffuse = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".specular";
                m_ulDir[q].specular = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".direction";
                m_ulDir[q].direction = glGetUniformLocation( m_id, _u_variant.c_str() );

            }

            for ( int q = 0; q < 7; q++ )
            {
                string _u_location = "u_material";
                _u_location += "[";
                _u_location += to_string( q );
                _u_location += "]";

                _u_variant = _u_location + ".ambient";
                m_uMaterial[q].ambient = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".diffuse";
                m_uMaterial[q].diffuse = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".specular";
                m_uMaterial[q].specular = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".shininess";
                m_uMaterial[q].shininess = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_location = "u_textures";
                _u_location += "[";
                _u_location += to_string( q );
                _u_location += "]";

                m_uTextures[q] = glGetUniformLocation( m_id, _u_location.c_str() );
            }

            unbind();
        }

        void setViewMatrix( const glm::mat4& mat )
        {
            _setMat4( m_uView, mat );
        }

        void setProjectionMatrix( const glm::mat4& mat )
        {
            _setMat4( m_uProj, mat );
        }

        void setNumberDirectionalLights( int num )
        {
            _setInt( m_uNumLightsDir, num );
        }

        void setLightDirectional( LLightDirectional* pLightDirectional, int indx )
        {
            assert( indx < 2 && indx >= 0 );

            _setVec3( m_ulDir[indx].ambient, pLightDirectional->ambient );
            _setVec3( m_ulDir[indx].diffuse, pLightDirectional->diffuse );
            _setVec3( m_ulDir[indx].specular, pLightDirectional->specular );
            _setVec3( m_ulDir[indx].direction, pLightDirectional->direction );
        }

        void setGlobalAmbientLight( const LVec3& vec )
        {
            _setVec3( m_uGlobalAmbientLight, vec );
        }

        void setViewPosition( const LVec3& vec )
        {
            _setVec3( m_uViewPos, vec );
        }

        void setMaterial( LMaterial* pMaterial, int indx )
        {
            assert( indx < 7 && indx >= 0 );

            _setVec3( m_uMaterial[indx].ambient, pMaterial->ambient );
            _setVec3( m_uMaterial[indx].diffuse, pMaterial->diffuse );
            _setVec3( m_uMaterial[indx].specular, pMaterial->specular );
            _setFloat( m_uMaterial[indx].shininess, pMaterial->shininess );
        }

        void setTexture( LTexture* pTexture, int indx )
        {
            assert( indx < 7 && indx >= 0 );

            _setInt( m_uTextures[indx], pTexture->getTextureIndx() );
        }

        void setTerrainVariation( int variationIndx )
        {
            assert( variationIndx < 4 && variationIndx >= 0 );

            _setInt( m_uTerrainVariation, variationIndx );
        }

    };



}
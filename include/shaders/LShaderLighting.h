

#pragma once

#include <shaders/LShaderBasic3d.h>
#include <LFog.h>
#include <LLightDirectional.h>
#include <LLightPoint.h>
#include <LLightSpot.h>
#include <LMaterial.h>

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POSITIONAL_LIGHTS 5
#define MAX_SPOT_LIGHTS 5

using namespace std;

namespace engine
{

    struct LFogUniforms
    {
        GLuint type;
        GLuint color;
        GLuint density;
        GLuint start;
        GLuint end;
        GLuint isActive;
    };

    struct LLDirUniforms
    {
        GLuint direction;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint isActive;
    };

    struct LLPointUniforms
    {
        GLuint position;
        GLuint constant;
        GLuint linear;
        GLuint quadratic;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint isActive;
    };

    struct LLSpotUniforms
    {
        GLuint position;
        GLuint direction;
        GLuint cutOff;
        GLuint outerCutOff;
        GLuint constant;
        GLuint linear;
        GLuint quadratic;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint isActive;
    };

    struct LMaterialUniforms
    {
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint shininess;
    };

    class LShaderLighting : public LShaderBasic3d
    {

        protected :

        LFogUniforms m_uFog;

        LLDirUniforms m_ulDir[MAX_DIRECTIONAL_LIGHTS];
        LLPointUniforms m_ulPoint[MAX_POSITIONAL_LIGHTS];
        LLSpotUniforms m_ulSpot[MAX_SPOT_LIGHTS];

        GLuint m_uViewPos;
        
        GLuint m_uGlobalAmbient;
        GLuint m_uNumLightsDir;
        GLuint m_uNumLightsPos;
        GLuint m_uNumLightsSpot;

        LMaterialUniforms m_uMaterial;

        public :

        LShaderLighting( GLuint id ) : LShaderBasic3d( id )
        {
            bind();

            m_uGlobalAmbient    = glGetUniformLocation( m_id, "u_globalAmbientLight" );
            m_uNumLightsDir     = glGetUniformLocation( m_id, "u_numDirectionalLights" );
            m_uNumLightsPos     = glGetUniformLocation( m_id, "u_numPointLights" );
            m_uNumLightsSpot    = glGetUniformLocation( m_id, "u_numSpotLights" );
            m_uViewPos          = glGetUniformLocation( m_id, "u_viewPos" );

            m_uMaterial.ambient   = glGetUniformLocation( m_id, "u_material.ambient" );
            m_uMaterial.diffuse   = glGetUniformLocation( m_id, "u_material.diffuse" );
            m_uMaterial.specular  = glGetUniformLocation( m_id, "u_material.specular" );
            m_uMaterial.shininess = glGetUniformLocation( m_id, "u_material.shininess" );

            m_uFog.type       = glGetUniformLocation( m_id, "u_fog.type" );
            m_uFog.color      = glGetUniformLocation( m_id, "u_fog.color" );
            m_uFog.density    = glGetUniformLocation( m_id, "u_fog.density" );
            m_uFog.start      = glGetUniformLocation( m_id, "u_fog.start" );
            m_uFog.end        = glGetUniformLocation( m_id, "u_fog.end" );
            m_uFog.isActive   = glGetUniformLocation( m_id, "u_fog.isActive" );

            for ( int q = 0; q < MAX_DIRECTIONAL_LIGHTS; q++ )
            {
                string _u_location = "u_directionalLights";
                _u_location += "[";
                _u_location += to_string( q );
                _u_location += "]";

                string _u_variant = "";

                _u_variant = _u_location + ".ambient";
                m_ulDir[q].ambient = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".diffuse";
                m_ulDir[q].diffuse = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".specular";
                m_ulDir[q].specular = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".direction";
                m_ulDir[q].direction = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".isActive";
                m_ulDir[q].isActive = glGetUniformLocation( m_id, _u_variant.c_str() );
            }

            for ( int q = 0; q < MAX_POSITIONAL_LIGHTS; q++ )
            {
                string _u_location = "u_pointLights";
                _u_location += "[";
                _u_location += to_string( q );
                _u_location += "]";

                string _u_variant = "";

                _u_variant = _u_location + ".ambient";
                m_ulPoint[q].ambient = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".diffuse";
                m_ulPoint[q].diffuse = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".specular";
                m_ulPoint[q].specular = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".position";
                m_ulPoint[q].position = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".constant";
                m_ulPoint[q].constant = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".linear";
                m_ulPoint[q].linear = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".quadratic";
                m_ulPoint[q].quadratic = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".isActive";
                m_ulPoint[q].isActive = glGetUniformLocation( m_id, _u_variant.c_str() );
            }

            for ( int q = 0; q < MAX_SPOT_LIGHTS; q++ )
            {
                string _u_location = "u_spotLights";
                _u_location += "[";
                _u_location += to_string( q );
                _u_location += "]";

                string _u_variant = "";

                _u_variant = _u_location + ".ambient";
                m_ulSpot[q].ambient = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".diffuse";
                m_ulSpot[q].diffuse = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".specular";
                m_ulSpot[q].specular = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".position";
                m_ulSpot[q].position = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".direction";
                m_ulSpot[q].direction = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".constant";
                m_ulSpot[q].constant = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".linear";
                m_ulSpot[q].linear = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".quadratic";
                m_ulSpot[q].quadratic = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".cutOff";
                m_ulSpot[q].cutOff = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".outerCutOff";
                m_ulSpot[q].outerCutOff = glGetUniformLocation( m_id, _u_variant.c_str() );

                _u_variant = _u_location + ".isActive";
                m_ulSpot[q].isActive = glGetUniformLocation( m_id, _u_variant.c_str() );
            }

            unbind();
        }

        void setFog( LFog* pFog )
        {
            _setInt( m_uFog.type, pFog->type );
            _setVec4( m_uFog.color, pFog->color );
            _setFloat( m_uFog.density, pFog->density );
            _setFloat( m_uFog.start, pFog->start );
            _setFloat( m_uFog.end, pFog->end );
            _setInt( m_uFog.isActive, pFog->isActive );
        }

        void setLightDirectional( LLightDirectional* pLightDirectional, int indx )
        {
            _setVec3( m_ulDir[indx].ambient, pLightDirectional->ambient );
            _setVec3( m_ulDir[indx].diffuse, pLightDirectional->diffuse );
            _setVec3( m_ulDir[indx].specular, pLightDirectional->specular );
            _setVec3( m_ulDir[indx].direction, pLightDirectional->direction );
            _setInt( m_ulDir[indx].isActive, pLightDirectional->isActive );
        }

        void setLightPoint( LLightPoint* pLightPoint, int indx )
        {
            _setVec3( m_ulPoint[indx].ambient, pLightPoint->ambient );
            _setVec3( m_ulPoint[indx].diffuse, pLightPoint->diffuse );
            _setVec3( m_ulPoint[indx].specular, pLightPoint->specular );
            _setVec3( m_ulPoint[indx].position, pLightPoint->position );
            _setFloat( m_ulPoint[indx].constant, pLightPoint->attenuation_constant );
            _setFloat( m_ulPoint[indx].linear, pLightPoint->attenuation_linear );
            _setFloat( m_ulPoint[indx].quadratic, pLightPoint->attenuation_quadratic );
            _setInt( m_ulPoint[indx].isActive, pLightPoint->isActive );
        }

        void setLightSpot( LLightSpot* pLightSpot, int indx )
        {
            _setVec3( m_ulSpot[indx].ambient, pLightSpot->ambient );
            _setVec3( m_ulSpot[indx].diffuse, pLightSpot->diffuse );
            _setVec3( m_ulSpot[indx].specular, pLightSpot->specular );
            _setVec3( m_ulSpot[indx].position, pLightSpot->position );
            _setVec3( m_ulSpot[indx].direction, pLightSpot->direction );
            _setFloat( m_ulSpot[indx].constant, pLightSpot->attenuation_constant );
            _setFloat( m_ulSpot[indx].linear, pLightSpot->attenuation_linear );
            _setFloat( m_ulSpot[indx].quadratic, pLightSpot->attenuation_quadratic );
            _setFloat( m_ulSpot[indx].cutOff, pLightSpot->cutOff );
            _setFloat( m_ulSpot[indx].outerCutOff, pLightSpot->outerCutOff );
            _setInt( m_ulSpot[indx].isActive, pLightSpot->isActive );
        }

        void setNumberDirectionalLights( int num )
        {
            _setInt( m_uNumLightsDir, num );
        }

        void setNumberPointLights( int num )
        {
            _setInt( m_uNumLightsPos, num );
        }

        void setNumberSpotLights( int num )
        {
            _setInt( m_uNumLightsSpot, num );
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
    };



}